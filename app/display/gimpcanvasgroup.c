/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpcanvasgroup.c
 * Copyright (C) 2010 Michael Natterer <mitch@gimp.org>
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

#include "libgimpbase/gimpbase.h"
#include "libgimpmath/gimpmath.h"

#include "display-types.h"

#include "gimpcanvasgroup.h"
#include "gimpdisplayshell.h"


enum
{
  PROP_0,
  PROP_GROUP_STROKING,
  PROP_GROUP_FILLING
};


typedef struct _GimpCanvasGroupPrivate GimpCanvasGroupPrivate;

struct _GimpCanvasGroupPrivate
{
  GList    *items;
  gboolean  group_stroking;
  gboolean  group_filling;
};

#define GET_PRIVATE(group) \
        G_TYPE_INSTANCE_GET_PRIVATE (group, \
                                     GIMP_TYPE_CANVAS_GROUP, \
                                     GimpCanvasGroupPrivate)


/*  local function prototypes  */

static void             gimp_canvas_group_dispose      (GObject         *object);
static void             gimp_canvas_group_set_property (GObject         *object,
                                                        guint            property_id,
                                                        const GValue    *value,
                                                        GParamSpec      *pspec);
static void             gimp_canvas_group_get_property (GObject         *object,
                                                        guint            property_id,
                                                        GValue          *value,
                                                        GParamSpec      *pspec);
static void             gimp_canvas_group_draw         (GimpCanvasItem  *item,
                                                        cairo_t         *cr);
static cairo_region_t * gimp_canvas_group_get_extents  (GimpCanvasItem  *item);
static gboolean         gimp_canvas_group_hit          (GimpCanvasItem  *item,
                                                        gdouble          x,
                                                        gdouble          y);

static void             gimp_canvas_group_child_update (GimpCanvasItem  *item,
                                                        cairo_region_t  *region,
                                                        GimpCanvasGroup *group);


G_DEFINE_TYPE (GimpCanvasGroup, gimp_canvas_group, GIMP_TYPE_CANVAS_ITEM)

#define parent_class gimp_canvas_group_parent_class


static void
gimp_canvas_group_class_init (GimpCanvasGroupClass *klass)
{
  GObjectClass        *object_class = G_OBJECT_CLASS (klass);
  GimpCanvasItemClass *item_class   = GIMP_CANVAS_ITEM_CLASS (klass);

  object_class->dispose      = gimp_canvas_group_dispose;
  object_class->set_property = gimp_canvas_group_set_property;
  object_class->get_property = gimp_canvas_group_get_property;

  item_class->draw           = gimp_canvas_group_draw;
  item_class->get_extents    = gimp_canvas_group_get_extents;
  item_class->hit            = gimp_canvas_group_hit;

  g_object_class_install_property (object_class, PROP_GROUP_STROKING,
                                   g_param_spec_boolean ("group-stroking",
                                                         NULL, NULL,
                                                         FALSE,
                                                         GIMP_PARAM_READWRITE));

  g_object_class_install_property (object_class, PROP_GROUP_FILLING,
                                   g_param_spec_boolean ("group-filling",
                                                         NULL, NULL,
                                                         FALSE,
                                                         GIMP_PARAM_READWRITE));

  g_type_class_add_private (klass, sizeof (GimpCanvasGroupPrivate));
}

static void
gimp_canvas_group_init (GimpCanvasGroup *group)
{
}

static void
gimp_canvas_group_dispose (GObject *object)
{
  GimpCanvasGroupPrivate *private = GET_PRIVATE (object);

  if (private->items)
    {
      g_list_free_full (private->items, (GDestroyNotify) g_object_unref);
      private->items = NULL;
    }

  G_OBJECT_CLASS (parent_class)->dispose (object);
}

static void
gimp_canvas_group_set_property (GObject      *object,
                                guint         property_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
  GimpCanvasGroupPrivate *private = GET_PRIVATE (object);

  switch (property_id)
    {
    case PROP_GROUP_STROKING:
      private->group_stroking = g_value_get_boolean (value);
      break;
    case PROP_GROUP_FILLING:
      private->group_filling = g_value_get_boolean (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gimp_canvas_group_get_property (GObject    *object,
                                guint       property_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
  GimpCanvasGroupPrivate *private = GET_PRIVATE (object);

  switch (property_id)
    {
    case PROP_GROUP_STROKING:
      g_value_set_boolean (value, private->group_stroking);
      break;
    case PROP_GROUP_FILLING:
      g_value_set_boolean (value, private->group_filling);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gimp_canvas_group_draw (GimpCanvasItem *item,
                        cairo_t        *cr)
{
  GimpCanvasGroupPrivate *private = GET_PRIVATE (item);
  GList                  *list;

  for (list = private->items; list; list = g_list_next (list))
    {
      GimpCanvasItem *sub_item = list->data;

      gimp_canvas_item_draw (sub_item, cr);
    }

  if (private->group_stroking)
    _gimp_canvas_item_stroke (item, cr);

  if (private->group_filling)
    _gimp_canvas_item_fill (item, cr);
}

static cairo_region_t *
gimp_canvas_group_get_extents (GimpCanvasItem *item)
{
  GimpCanvasGroupPrivate *private = GET_PRIVATE (item);
  cairo_region_t         *region  = NULL;
  GList                  *list;

  for (list = private->items; list; list = g_list_next (list))
    {
      GimpCanvasItem *sub_item   = list->data;
      cairo_region_t *sub_region = gimp_canvas_item_get_extents (sub_item);

      if (! region)
        {
          region = sub_region;
        }
      else if (sub_region)
        {
          cairo_region_union (region, sub_region);
          cairo_region_destroy (sub_region);
        }
    }

  return region;
}

static gboolean
gimp_canvas_group_hit (GimpCanvasItem *item,
                       gdouble         x,
                       gdouble         y)
{
  GimpCanvasGroupPrivate *private = GET_PRIVATE (item);
  GList                  *list;

  for (list = private->items; list; list = g_list_next (list))
    {
      if (gimp_canvas_item_hit (list->data, x, y))
        return TRUE;
    }

  return FALSE;
}

static void
gimp_canvas_group_child_update (GimpCanvasItem  *item,
                                cairo_region_t  *region,
                                GimpCanvasGroup *group)
{
  if (_gimp_canvas_item_needs_update (GIMP_CANVAS_ITEM (group)))
    _gimp_canvas_item_update (GIMP_CANVAS_ITEM (group), region);
}


/*  public functions  */

GimpCanvasItem *
gimp_canvas_group_new (GimpDisplayShell *shell)
{
  g_return_val_if_fail (GIMP_IS_DISPLAY_SHELL (shell), NULL);

  return g_object_new (GIMP_TYPE_CANVAS_GROUP,
                       "shell", shell,
                       NULL);
}

void
gimp_canvas_group_add_item (GimpCanvasGroup *group,
                            GimpCanvasItem  *item)
{
  GimpCanvasGroupPrivate *private;

  g_return_if_fail (GIMP_IS_CANVAS_GROUP (group));
  g_return_if_fail (GIMP_IS_CANVAS_ITEM (item));
  g_return_if_fail (GIMP_CANVAS_ITEM (group) != item);

  private = GET_PRIVATE (group);

  if (private->group_stroking)
    gimp_canvas_item_suspend_stroking (item);

  if (private->group_filling)
    gimp_canvas_item_suspend_filling (item);

  private->items = g_list_append (private->items, g_object_ref (item));

  if (_gimp_canvas_item_needs_update (GIMP_CANVAS_ITEM (group)))
    {
      cairo_region_t *region = gimp_canvas_item_get_extents (item);

      if (region)
        {
          _gimp_canvas_item_update (GIMP_CANVAS_ITEM (group), region);
          cairo_region_destroy (region);
        }
    }

  g_signal_connect (item, "update",
                    G_CALLBACK (gimp_canvas_group_child_update),
                    group);
}

void
gimp_canvas_group_remove_item (GimpCanvasGroup *group,
                               GimpCanvasItem  *item)
{
  GimpCanvasGroupPrivate *private;

  g_return_if_fail (GIMP_IS_CANVAS_GROUP (group));
  g_return_if_fail (GIMP_IS_CANVAS_ITEM (item));

  private = GET_PRIVATE (group);

  g_return_if_fail (g_list_find (private->items, item));

  private->items = g_list_remove (private->items, item);

  if (private->group_stroking)
    gimp_canvas_item_resume_stroking (item);

  if (private->group_filling)
    gimp_canvas_item_resume_filling (item);

  if (_gimp_canvas_item_needs_update (GIMP_CANVAS_ITEM (group)))
    {
      cairo_region_t *region = gimp_canvas_item_get_extents (item);

      if (region)
        {
          _gimp_canvas_item_update (GIMP_CANVAS_ITEM (group), region);
          cairo_region_destroy (region);
        }
    }

  g_signal_handlers_disconnect_by_func (item,
                                        gimp_canvas_group_child_update,
                                        group);

  g_object_unref (item);
}

void
gimp_canvas_group_set_group_stroking (GimpCanvasGroup *group,
                                      gboolean         group_stroking)
{
  GimpCanvasGroupPrivate *private;

  g_return_if_fail (GIMP_IS_CANVAS_GROUP (group));

  private = GET_PRIVATE (group);

  if (private->group_stroking != group_stroking)
    {
      GList *list;

      gimp_canvas_item_begin_change (GIMP_CANVAS_ITEM (group));

      g_object_set (group,
                    "group-stroking", group_stroking ? TRUE : FALSE,
                    NULL);

      for (list = private->items; list; list = g_list_next (list))
        {
          if (private->group_stroking)
            gimp_canvas_item_suspend_stroking (list->data);
          else
            gimp_canvas_item_resume_stroking (list->data);
        }

      gimp_canvas_item_end_change (GIMP_CANVAS_ITEM (group));
    }
}

void
gimp_canvas_group_set_group_filling (GimpCanvasGroup *group,
                                     gboolean         group_filling)
{
  GimpCanvasGroupPrivate *private;

  g_return_if_fail (GIMP_IS_CANVAS_GROUP (group));

  private = GET_PRIVATE (group);

  if (private->group_filling != group_filling)
    {
      GList *list;

      gimp_canvas_item_begin_change (GIMP_CANVAS_ITEM (group));

      g_object_set (group,
                    "group-filling", group_filling ? TRUE : FALSE,
                    NULL);

      for (list = private->items; list; list = g_list_next (list))
        {
          if (private->group_filling)
            gimp_canvas_item_suspend_filling (list->data);
          else
            gimp_canvas_item_resume_filling (list->data);
        }

      gimp_canvas_item_end_change (GIMP_CANVAS_ITEM (group));
    }
}
