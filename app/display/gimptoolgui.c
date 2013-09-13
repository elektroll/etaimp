/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimptoolgui.c
 * Copyright (C) 2013  Michael Natterer <mitch@gimp.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <gegl.h>
#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "display-types.h"

#include "core/gimpcontext.h"
#include "core/gimpmarshal.h"
#include "core/gimptoolinfo.h"

#include "widgets/gimpdialogfactory.h"
#include "widgets/gimpoverlaybox.h"
#include "widgets/gimpoverlaydialog.h"

#include "gimpdisplayshell.h"
#include "gimptooldialog.h"
#include "gimptoolgui.h"


enum
{
  RESPONSE,
  LAST_SIGNAL
};


typedef struct _ResponseEntry ResponseEntry;

struct _ResponseEntry
{
  gint      response_id;
  gchar    *stock_id;
  gint      alternative_position;
  gboolean  sensitive;
};

typedef struct _GimpToolGuiPrivate GimpToolGuiPrivate;

struct _GimpToolGuiPrivate
{
  GimpToolInfo     *tool_info;
  gchar            *description;
  GList            *response_entries;
  gint              default_response;
  gboolean          focus_on_map;

  gboolean          overlay;

  GimpDisplayShell *shell;
  GimpViewable     *viewable;

  GtkWidget        *dialog;
  GtkWidget        *vbox;
};

#define GET_PRIVATE(gui) G_TYPE_INSTANCE_GET_PRIVATE (gui, \
                                                      GIMP_TYPE_TOOL_GUI, \
                                                      GimpToolGuiPrivate)


static void   gimp_tool_gui_dispose         (GObject       *object);
static void   gimp_tool_gui_finalize        (GObject       *object);

static void   gimp_tool_gui_create_dialog   (GimpToolGui   *gui);
static void   gimp_tool_gui_update_buttons  (GimpToolGui   *gui);
static void   gimp_tool_gui_update_shell    (GimpToolGui   *gui);
static void   gimp_tool_gui_update_viewable (GimpToolGui   *gui);

static void   gimp_tool_gui_dialog_response (GtkWidget     *dialog,
                                             gint           response_id,
                                             GimpToolGui   *gui);

static ResponseEntry * response_entry_new   (gint           response_id,
                                             const gchar   *stock_id);
static void            response_entry_free  (ResponseEntry *entry);
static ResponseEntry * response_entry_find  (GList         *entries,
                                             gint           response_id);


G_DEFINE_TYPE (GimpToolGui, gimp_tool_gui, GIMP_TYPE_OBJECT)

static guint signals[LAST_SIGNAL] = { 0, };

#define parent_class gimp_tool_gui_parent_class


static void
gimp_tool_gui_class_init (GimpToolGuiClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose  = gimp_tool_gui_dispose;
  object_class->finalize = gimp_tool_gui_finalize;

  signals[RESPONSE] =
    g_signal_new ("response",
                  G_OBJECT_CLASS_TYPE (klass),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (GimpToolGuiClass, response),
                  NULL, NULL,
                  gimp_marshal_VOID__INT,
                  G_TYPE_NONE, 1,
                  G_TYPE_INT);

  g_type_class_add_private (klass, sizeof (GimpToolGuiPrivate));
}

static void
gimp_tool_gui_init (GimpToolGui *gui)
{
  GimpToolGuiPrivate *private = GET_PRIVATE (gui);

  private->default_response = -1;
  private->focus_on_map     = TRUE;

  private->vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
  g_object_ref_sink (private->vbox);
}

static void
gimp_tool_gui_dispose (GObject *object)
{
  GimpToolGuiPrivate *private = GET_PRIVATE (object);

  if (private->tool_info)
    {
      g_object_unref (private->tool_info);
      private->tool_info = NULL;
    }

  if (private->shell)
    gimp_tool_gui_set_shell (GIMP_TOOL_GUI (object), NULL);

  if (private->vbox)
    {
      g_object_unref (private->vbox);
      private->vbox = NULL;
    }

  if (private->dialog)
    {
      if (private->overlay)
        g_object_unref (private->dialog);
      else
        gtk_widget_destroy (private->dialog);

      private->dialog = NULL;
    }

  G_OBJECT_CLASS (parent_class)->dispose (object);
}

static void
gimp_tool_gui_finalize (GObject *object)
{
  GimpToolGuiPrivate *private = GET_PRIVATE (object);

  if (private->description)
    {
      g_free (private->description);
      private->description = NULL;
    }

  if (private->response_entries)
    {
      g_list_free_full (private->response_entries,
                        (GDestroyNotify) response_entry_free);
      private->response_entries = NULL;
    }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}


/**
 * gimp_tool_gui_new:
 * @tool_info:   a #GimpToolInfo
 * @description: a string to use in the gui header or %NULL to use the help
 *               field from #GimpToolInfo
 * @...:         a %NULL-terminated valist of button parameters as described in
 *               gtk_gui_new_with_buttons().
 *
 * This function creates a #GimpToolGui using the information stored
 * in @tool_info.
 *
 * Return value: a new #GimpToolGui
 **/
GimpToolGui *
gimp_tool_gui_new (GimpToolInfo *tool_info,
                   const gchar  *description,
                   gboolean      overlay,
                   ...)
{
  GimpToolGui        *gui;
  GimpToolGuiPrivate *private;
  va_list             args;
  const gchar        *stock_id;

  g_return_val_if_fail (GIMP_IS_TOOL_INFO (tool_info), NULL);

  gui = g_object_new (GIMP_TYPE_TOOL_GUI, NULL);

  private = GET_PRIVATE (gui);

  private->tool_info   = g_object_ref (tool_info);
  private->description = g_strdup (description);
  private->overlay     = overlay;

  va_start (args, overlay);

  for (stock_id = va_arg (args, const gchar *);
       stock_id;
       stock_id = va_arg (args, const gchar *))
    {
      gint response_id = va_arg (args, gint);

      private->response_entries = g_list_append (private->response_entries,
                                                 response_entry_new (response_id,
                                                                     stock_id));
    }

  va_end (args);

  gimp_tool_gui_create_dialog (gui);

  return gui;
}

void
gimp_tool_gui_set_description (GimpToolGui *gui,
                               const gchar *description)
{
  GimpToolGuiPrivate *private;

  g_return_if_fail (GIMP_IS_TOOL_GUI (gui));

  private = GET_PRIVATE (gui);

  if (description == private->description)
    return;

  g_free (private->description);
  private->description = g_strdup (description);

  if (! description)
    description = private->tool_info->help;

  if (private->overlay)
    {
      /* TODO */
    }
  else
    {
      g_object_set (private->dialog, "description", description, NULL);
    }
}

void
gimp_tool_gui_set_shell (GimpToolGui      *gui,
                         GimpDisplayShell *shell)
{
  GimpToolGuiPrivate *private;

  g_return_if_fail (GIMP_IS_TOOL_GUI (gui));
  g_return_if_fail (shell == NULL || GIMP_IS_DISPLAY_SHELL (shell));

  private = GET_PRIVATE (gui);

  if (shell == private->shell)
    return;

  private->shell = shell;

  gimp_tool_gui_update_shell (gui);
}

void
gimp_tool_gui_set_viewable (GimpToolGui  *gui,
                            GimpViewable *viewable)
{
  GimpToolGuiPrivate *private;

  g_return_if_fail (GIMP_IS_TOOL_GUI (gui));
  g_return_if_fail (GIMP_IS_VIEWABLE (viewable));

  private = GET_PRIVATE (gui);

  if (private->viewable == viewable)
    return;

  private->viewable = viewable;

  gimp_tool_gui_update_viewable (gui);
}

GtkWidget *
gimp_tool_gui_get_dialog (GimpToolGui *gui)
{
  g_return_val_if_fail (GIMP_IS_TOOL_GUI (gui), NULL);

  return GET_PRIVATE (gui)->dialog;
}

GtkWidget *
gimp_tool_gui_get_vbox (GimpToolGui *gui)
{
  g_return_val_if_fail (GIMP_IS_TOOL_GUI (gui), NULL);

  return GET_PRIVATE (gui)->vbox;
}

void
gimp_tool_gui_show (GimpToolGui *gui)
{
  GimpToolGuiPrivate *private;

  g_return_if_fail (GIMP_IS_TOOL_GUI (gui));

  private = GET_PRIVATE (gui);

  g_return_if_fail (private->shell != NULL);

  if (private->overlay)
    {
      if (! gtk_widget_get_parent (private->dialog))
        {
          gimp_overlay_box_add_child (GIMP_OVERLAY_BOX (private->shell->canvas),
                                      private->dialog, 1.0, 0.0);
          gtk_widget_show (private->dialog);
        }
    }
  else
    {
      if (gtk_widget_get_visible (private->dialog))
        gdk_window_show (gtk_widget_get_window (private->dialog));
      else
        gtk_widget_show (private->dialog);
    }
}

void
gimp_tool_gui_hide (GimpToolGui *gui)
{
  GimpToolGuiPrivate *private;

  g_return_if_fail (GIMP_IS_TOOL_GUI (gui));

  private = GET_PRIVATE (gui);

  if (private->overlay)
    {
      if (gtk_widget_get_parent (private->dialog))
        {
          gtk_container_remove (GTK_CONTAINER (gtk_widget_get_parent (private->dialog)),
                                private->dialog);
          gtk_widget_hide (private->dialog);
        }
    }
  else
    {
      if (gimp_dialog_factory_from_widget (private->dialog, NULL))
        {
          gimp_dialog_factory_hide_dialog (private->dialog);
        }
      else
        {
          gtk_widget_hide (private->dialog);
        }
    }
}

void
gimp_tool_gui_set_overlay (GimpToolGui *gui,
                           gboolean     overlay)
{
  GimpToolGuiPrivate *private;
  gboolean            visible;

  g_return_if_fail (GIMP_IS_TOOL_GUI (gui));

  private = GET_PRIVATE (gui);

  if (private->overlay == overlay)
    return;

  visible = gtk_widget_get_visible (private->dialog);

  if (visible)
    gimp_tool_gui_hide (gui);

  gtk_container_remove (GTK_CONTAINER (gtk_widget_get_parent (private->vbox)),
                        private->vbox);

  if (private->overlay)
    g_object_unref (private->dialog);
  else
    gtk_widget_destroy (private->dialog);

  private->overlay = overlay ? TRUE : FALSE;

  gimp_tool_gui_create_dialog (gui);

  if (visible)
    gimp_tool_gui_show (gui);
}

gboolean
gimp_tool_gui_get_overlay (GimpToolGui *gui)
{
  g_return_val_if_fail (GIMP_IS_TOOL_GUI (gui), FALSE);

  return GET_PRIVATE (gui)->overlay;
}

void
gimp_tool_gui_set_focus_on_map (GimpToolGui *gui,
                                gboolean     focus_on_map)
{
  GimpToolGuiPrivate *private;

  g_return_if_fail (GIMP_IS_TOOL_GUI (gui));

  private = GET_PRIVATE (gui);

  if (private->focus_on_map == focus_on_map)
    return;

  private->focus_on_map = focus_on_map ? TRUE : FALSE;

  if (! private->overlay)
    {
      gtk_window_set_focus_on_map (GTK_WINDOW (private->dialog),
                                   private->focus_on_map);
    }
}

gboolean
gimp_tool_gui_get_focus_on_map (GimpToolGui *gui)
{
  g_return_val_if_fail (GIMP_IS_TOOL_GUI (gui), FALSE);

  return GET_PRIVATE (gui)->focus_on_map;
}

void
gimp_tool_gui_set_default_response (GimpToolGui *gui,
                                    gint         response_id)
{
  GimpToolGuiPrivate *private;

  g_return_if_fail (GIMP_IS_TOOL_GUI (gui));

  private = GET_PRIVATE (gui);

  g_return_if_fail (response_entry_find (private->response_entries,
                                         response_id) != NULL);

  private->default_response = response_id;

  if (private->overlay)
    {
      gimp_overlay_dialog_set_default_response (GIMP_OVERLAY_DIALOG (private->dialog),
                                                response_id);
    }
  else
    {
      gtk_dialog_set_default_response (GTK_DIALOG (private->dialog),
                                       response_id);
    }
}

void
gimp_tool_gui_set_response_sensitive (GimpToolGui *gui,
                                      gint         response_id,
                                      gboolean     sensitive)
{
  GimpToolGuiPrivate *private;
  ResponseEntry      *entry;

  g_return_if_fail (GIMP_IS_TOOL_GUI (gui));

  private = GET_PRIVATE (gui);

  entry = response_entry_find (private->response_entries, response_id);

  g_return_if_fail (entry != NULL);

  entry->sensitive = sensitive;

  if (private->overlay)
    {
      gimp_overlay_dialog_set_response_sensitive (GIMP_OVERLAY_DIALOG (private->dialog),
                                                  response_id, sensitive);
    }
  else
    {
      gtk_dialog_set_response_sensitive (GTK_DIALOG (private->dialog),
                                         response_id, sensitive);
    }
}

void
gimp_tool_gui_set_alternative_button_order (GimpToolGui *gui,
                                            ...)
{
  GimpToolGuiPrivate *private;
  va_list             args;
  gint                response_id;
  gint                i;

  g_return_if_fail (GIMP_IS_TOOL_GUI (gui));

  private = GET_PRIVATE (gui);

  va_start (args, gui);

  for (response_id = va_arg (args, gint), i = 0;
       response_id != -1;
       response_id = va_arg (args, gint), i++)
    {
      ResponseEntry *entry = response_entry_find (private->response_entries,
                                                  response_id);

      if (entry)
        entry->alternative_position = i;
    }

  va_end (args);

  gimp_tool_gui_update_buttons (gui);
}


/*  private functions  */

static void
gimp_tool_gui_create_dialog (GimpToolGui *gui)
{
  GimpToolGuiPrivate *private = GET_PRIVATE (gui);
  GList              *list;

  if (private->overlay)
    {
      private->dialog = gimp_overlay_dialog_new (private->tool_info,
                                                 private->description,
                                                 NULL);
      g_object_ref_sink (private->dialog);

      for (list = private->response_entries; list; list = g_list_next (list))
        {
          ResponseEntry *entry = list->data;

          gimp_overlay_dialog_add_button (GIMP_OVERLAY_DIALOG (private->dialog),
                                          entry->stock_id,
                                          entry->response_id);

          if (! entry->sensitive)
            gimp_overlay_dialog_set_response_sensitive (GIMP_OVERLAY_DIALOG (private->dialog),
                                                        entry->response_id,
                                                        FALSE);
        }

      if (private->default_response != -1)
        gimp_overlay_dialog_set_default_response (GIMP_OVERLAY_DIALOG (private->dialog),
                                                  private->default_response);

      gtk_container_set_border_width (GTK_CONTAINER (private->dialog), 6);

      gtk_container_set_border_width (GTK_CONTAINER (private->vbox), 0);
      gtk_container_add (GTK_CONTAINER (private->dialog), private->vbox);
      gtk_widget_show (private->vbox);
    }
  else
    {
      private->dialog = gimp_tool_dialog_new (private->tool_info,
                                              private->description,
                                              NULL);

      for (list = private->response_entries; list; list = g_list_next (list))
        {
          ResponseEntry *entry = list->data;

          gimp_dialog_add_button (GIMP_DIALOG (private->dialog),
                                  entry->stock_id,
                                  entry->response_id);

          if (! entry->sensitive)
            gtk_dialog_set_response_sensitive (GTK_DIALOG (private->dialog),
                                               entry->response_id,
                                               FALSE);
        }

      if (private->default_response != -1)
        gtk_dialog_set_default_response (GTK_DIALOG (private->dialog),
                                         private->default_response);

      gtk_window_set_focus_on_map (GTK_WINDOW (private->dialog),
                                   private->focus_on_map);

      gtk_container_set_border_width (GTK_CONTAINER (private->vbox), 6);
      gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (private->dialog))),
                          private->vbox, TRUE, TRUE, 0);
      gtk_widget_show (private->vbox);
    }

  gimp_tool_gui_update_buttons (gui);

  if (private->shell)
    gimp_tool_gui_update_shell (gui);

  if (private->viewable)
    gimp_tool_gui_update_viewable (gui);

  g_signal_connect_object (private->dialog, "response",
                           G_CALLBACK (gimp_tool_gui_dialog_response),
                           G_OBJECT (gui), 0);
}

static void
gimp_tool_gui_update_buttons (GimpToolGui *gui)
{
  GimpToolGuiPrivate *private = GET_PRIVATE (gui);
  GList              *list;
  gint               *ids;
  gint                n_ids;
  gint                n_alternatives = 0;
  gint                i;

  n_ids = g_list_length (private->response_entries);
  ids   = g_new0 (gint, n_ids);

  for (list = private->response_entries, i = 0;
       list;
       list = g_list_next (list), i++)
    {
      ResponseEntry *entry = list->data;

      if (entry->alternative_position >= 0 &&
          entry->alternative_position < n_ids)
        {
          ids[entry->alternative_position] = entry->response_id;
          n_alternatives++;
        }
    }

  if (n_ids == n_alternatives)
    {
      if (private->overlay)
        {
          gimp_overlay_dialog_set_alternative_button_order (GIMP_OVERLAY_DIALOG (private->dialog),
                                                            n_ids, ids);
        }
      else
        {
          gtk_dialog_set_alternative_button_order_from_array (GTK_DIALOG (private->dialog),
                                                              n_ids, ids);
        }
    }

  g_free (ids);
}

static void
gimp_tool_gui_update_shell (GimpToolGui *gui)
{
  GimpToolGuiPrivate *private = GET_PRIVATE (gui);

  if (! private->overlay)
    {
      gimp_tool_dialog_set_shell (GIMP_TOOL_DIALOG (private->dialog),
                                  private->shell);
    }
}

static void
gimp_tool_gui_update_viewable (GimpToolGui *gui)
{
  GimpToolGuiPrivate *private = GET_PRIVATE (gui);

  if (! private->overlay)
    {
      gimp_viewable_dialog_set_viewable (GIMP_VIEWABLE_DIALOG (private->dialog),
                                         private->viewable,
                                         GIMP_CONTEXT (private->tool_info->tool_options));
    }
}

static void
gimp_tool_gui_dialog_response (GtkWidget   *dialog,
                               gint         response_id,
                               GimpToolGui *gui)
{
  g_signal_emit (gui, signals[RESPONSE], 0,
                 response_id);
}

static ResponseEntry *
response_entry_new (gint         response_id,
                    const gchar *stock_id)
{
  ResponseEntry *entry = g_slice_new0 (ResponseEntry);

  entry->response_id          = response_id;
  entry->stock_id             = g_strdup (stock_id);
  entry->alternative_position = -1;
  entry->sensitive            = TRUE;

  return entry;
}

static void
response_entry_free (ResponseEntry *entry)
{
  g_free (entry->stock_id);

  g_slice_free (ResponseEntry, entry);
}

static ResponseEntry *
response_entry_find (GList *entries,
                     gint   response_id)
{
  for (; entries; entries = g_list_next (entries))
    {
      ResponseEntry *entry = entries->data;

      if (entry->response_id == response_id)
        return entry;
    }

  return NULL;
}
