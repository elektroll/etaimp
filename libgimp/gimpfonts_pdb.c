/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpfonts_pdb.c
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/* NOTE: This file is auto-generated by pdbgen.pl */

#include "config.h"

#include "gimp.h"


/**
 * SECTION: gimpfonts
 * @title: gimpfonts
 * @short_description: Operations related to fonts.
 *
 * Operations related to fonts.
 **/


/**
 * gimp_fonts_refresh:
 *
 * Refresh current fonts. This function always succeeds.
 *
 * This procedure retrieves all fonts currently in the user's font path
 * and updates the font dialogs accordingly.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_fonts_refresh (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-fonts-refresh",
                                    &nreturn_vals,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_fonts_get_list:
 * @filter: An optional regular expression used to filter the list.
 * @num_fonts: The number of available fonts.
 *
 * Retrieve the list of loaded fonts.
 *
 * This procedure returns a list of the fonts that are currently
 * available.
 *
 * Returns: The list of font names. The returned value must be freed
 * with g_strfreev().
 **/
gchar **
gimp_fonts_get_list (const gchar *filter,
                     gint        *num_fonts)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gchar **font_list = NULL;
  gint i;

  return_vals = gimp_run_procedure ("gimp-fonts-get-list",
                                    &nreturn_vals,
                                    GIMP_PDB_STRING, filter,
                                    GIMP_PDB_END);

  *num_fonts = 0;

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    {
      *num_fonts = return_vals[1].data.d_int32;
      font_list = g_new (gchar *, *num_fonts + 1);
      for (i = 0; i < *num_fonts; i++)
        font_list[i] = g_strdup (return_vals[2].data.d_stringarray[i]);
      font_list[i] = NULL;
    }

  gimp_destroy_params (return_vals, nreturn_vals);

  return font_list;
}
