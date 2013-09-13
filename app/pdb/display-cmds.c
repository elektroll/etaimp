/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995-2003 Spencer Kimball and Peter Mattis
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

/* NOTE: This file is auto-generated by pdbgen.pl. */

#include "config.h"

#include <gegl.h>

#include <gdk-pixbuf/gdk-pixbuf.h>

#include "libgimpbase/gimpbase.h"

#include "pdb-types.h"

#include "core/gimp.h"
#include "core/gimpcontainer.h"
#include "core/gimpimage.h"
#include "core/gimpparamspecs.h"

#include "gimppdb.h"
#include "gimpprocedure.h"
#include "internal-procs.h"


static GimpValueArray *
display_is_valid_invoker (GimpProcedure         *procedure,
                          Gimp                  *gimp,
                          GimpContext           *context,
                          GimpProgress          *progress,
                          const GimpValueArray  *args,
                          GError               **error)
{
  GimpValueArray *return_vals;
  GimpObject *display;
  gboolean valid = FALSE;

  display = gimp_value_get_display (gimp_value_array_index (args, 0), gimp);

  valid = (display != NULL);

  return_vals = gimp_procedure_get_return_values (procedure, TRUE, NULL);
  g_value_set_boolean (gimp_value_array_index (return_vals, 1), valid);

  return return_vals;
}

static GimpValueArray *
display_new_invoker (GimpProcedure         *procedure,
                     Gimp                  *gimp,
                     GimpContext           *context,
                     GimpProgress          *progress,
                     const GimpValueArray  *args,
                     GError               **error)
{
  gboolean success = TRUE;
  GimpValueArray *return_vals;
  GimpImage *image;
  GimpObject *display = NULL;

  image = gimp_value_get_image (gimp_value_array_index (args, 0), gimp);

  if (success)
    {
      gimp_image_flush (image);

      display = gimp_create_display (gimp, image, GIMP_UNIT_PIXEL, 1.0);

      if (display)
        {
          /* the first display takes ownership of the image */
          if (gimp_image_get_display_count (image) == 1)
            g_object_unref (image);
        }
      else
        {
          success = FALSE;
        }
    }

  return_vals = gimp_procedure_get_return_values (procedure, success,
                                                  error ? *error : NULL);

  if (success)
    gimp_value_set_display (gimp_value_array_index (return_vals, 1), display);

  return return_vals;
}

static GimpValueArray *
display_delete_invoker (GimpProcedure         *procedure,
                        Gimp                  *gimp,
                        GimpContext           *context,
                        GimpProgress          *progress,
                        const GimpValueArray  *args,
                        GError               **error)
{
  gboolean success = TRUE;
  GimpObject *display;

  display = gimp_value_get_display (gimp_value_array_index (args, 0), gimp);

  if (success)
    {
      gimp_delete_display (gimp, display);
    }

  return gimp_procedure_get_return_values (procedure, success,
                                           error ? *error : NULL);
}

static GimpValueArray *
display_get_window_handle_invoker (GimpProcedure         *procedure,
                                   Gimp                  *gimp,
                                   GimpContext           *context,
                                   GimpProgress          *progress,
                                   const GimpValueArray  *args,
                                   GError               **error)
{
  gboolean success = TRUE;
  GimpValueArray *return_vals;
  GimpObject *display;
  gint32 window = 0;

  display = gimp_value_get_display (gimp_value_array_index (args, 0), gimp);

  if (success)
    {
      window = (gint32) gimp_get_display_window_id (gimp, display);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success,
                                                  error ? *error : NULL);

  if (success)
    g_value_set_int (gimp_value_array_index (return_vals, 1), window);

  return return_vals;
}

static GimpValueArray *
displays_flush_invoker (GimpProcedure         *procedure,
                        Gimp                  *gimp,
                        GimpContext           *context,
                        GimpProgress          *progress,
                        const GimpValueArray  *args,
                        GError               **error)
{
  gimp_container_foreach (gimp->images, (GFunc) gimp_image_flush, NULL);

  return gimp_procedure_get_return_values (procedure, TRUE, NULL);
}

static GimpValueArray *
displays_reconnect_invoker (GimpProcedure         *procedure,
                            Gimp                  *gimp,
                            GimpContext           *context,
                            GimpProgress          *progress,
                            const GimpValueArray  *args,
                            GError               **error)
{
  gboolean success = TRUE;
  GimpImage *old_image;
  GimpImage *new_image;

  old_image = gimp_value_get_image (gimp_value_array_index (args, 0), gimp);
  new_image = gimp_value_get_image (gimp_value_array_index (args, 1), gimp);

  if (success)
    {
      success = (old_image != new_image    &&
                 gimp_image_get_display_count (old_image) > 0 &&
                 gimp_image_get_display_count (new_image) == 0);

      if (success)
        {
          gimp_reconnect_displays (gimp, old_image, new_image);

          /* take ownership of the image */
          if (gimp_image_get_display_count (new_image) > 0)
            g_object_unref (new_image);
        }
    }

  return gimp_procedure_get_return_values (procedure, success,
                                           error ? *error : NULL);
}

void
register_display_procs (GimpPDB *pdb)
{
  GimpProcedure *procedure;

  /*
   * gimp-display-is-valid
   */
  procedure = gimp_procedure_new (display_is_valid_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-display-is-valid");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-display-is-valid",
                                     "Returns TRUE if the display is valid.",
                                     "This procedure checks if the given display ID is valid and refers to an existing display.",
                                     "Sven Neumann <sven@gimp.org>",
                                     "Sven Neumann",
                                     "2007",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_display_id ("display",
                                                           "display",
                                                           "The display to check",
                                                           pdb->gimp, FALSE,
                                                           GIMP_PARAM_READWRITE | GIMP_PARAM_NO_VALIDATE));
  gimp_procedure_add_return_value (procedure,
                                   g_param_spec_boolean ("valid",
                                                         "valid",
                                                         "Whether the display ID is valid",
                                                         FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-display-new
   */
  procedure = gimp_procedure_new (display_new_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-display-new");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-display-new",
                                     "Create a new display for the specified image.",
                                     "Creates a new display for the specified image. If the image already has a display, another is added. Multiple displays are handled transparently by GIMP. The newly created display is returned and can be subsequently destroyed with a call to 'gimp-display-delete'. This procedure only makes sense for use with the GIMP UI.",
                                     "Spencer Kimball & Peter Mattis",
                                     "Spencer Kimball & Peter Mattis",
                                     "1995-1996",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         pdb->gimp, FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_display_id ("display",
                                                               "display",
                                                               "The new display",
                                                               pdb->gimp, FALSE,
                                                               GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-display-delete
   */
  procedure = gimp_procedure_new (display_delete_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-display-delete");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-display-delete",
                                     "Delete the specified display.",
                                     "This procedure removes the specified display. If this is the last remaining display for the underlying image, then the image is deleted also. Note that the display is closed no matter if the image is dirty or not. Better save the image before calling this procedure.",
                                     "Spencer Kimball & Peter Mattis",
                                     "Spencer Kimball & Peter Mattis",
                                     "1995-1996",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_display_id ("display",
                                                           "display",
                                                           "The display to delete",
                                                           pdb->gimp, FALSE,
                                                           GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-display-get-window-handle
   */
  procedure = gimp_procedure_new (display_get_window_handle_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-display-get-window-handle");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-display-get-window-handle",
                                     "Get a handle to the native window for an image display.",
                                     "This procedure returns a handle to the native window for a given image display. For example in the X backend of GDK, a native window handle is an Xlib XID. A value of 0 is returned for an invalid display or if this function is unimplemented for the windowing system that is being used.",
                                     "Sven Neumann <sven@gimp.org>",
                                     "Sven Neumann",
                                     "2005",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_display_id ("display",
                                                           "display",
                                                           "The display to get the window handle from",
                                                           pdb->gimp, FALSE,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("window",
                                                          "window",
                                                          "The native window handle or 0",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-displays-flush
   */
  procedure = gimp_procedure_new (displays_flush_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-displays-flush");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-displays-flush",
                                     "Flush all internal changes to the user interface",
                                     "This procedure takes no arguments and returns nothing except a success status. Its purpose is to flush all pending updates of image manipulations to the user interface. It should be called whenever appropriate.",
                                     "Spencer Kimball & Peter Mattis",
                                     "Spencer Kimball & Peter Mattis",
                                     "1995-1996",
                                     NULL);
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-displays-reconnect
   */
  procedure = gimp_procedure_new (displays_reconnect_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-displays-reconnect");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-displays-reconnect",
                                     "Reconnect displays from one image to another image.",
                                     "This procedure connects all displays of the old_image to the new_image. If the old_image has no display or new_image already has a display the reconnect is not performed and the procedure returns without success. You should rarely need to use this function.",
                                     "Spencer Kimball & Peter Mattis",
                                     "Spencer Kimball & Peter Mattis",
                                     "1995-1996",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("old-image",
                                                         "old image",
                                                         "The old image (must have at least one display)",
                                                         pdb->gimp, FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("new-image",
                                                         "new image",
                                                         "The new image (must not have a display)",
                                                         pdb->gimp, FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);
}
