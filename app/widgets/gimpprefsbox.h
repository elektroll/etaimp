/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpprefsbox.h
 * Copyright (C) 2013 Michael Natterer <mitch@gimp.org>
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

#ifndef __GIMP_PREFS_BOX_H__
#define __GIMP_PREFS_BOX_H__


#define GIMP_TYPE_PREFS_BOX            (gimp_prefs_box_get_type ())
#define GIMP_PREFS_BOX(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_PREFS_BOX, GimpPrefsBox))
#define GIMP_PREFS_BOX_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_PREFS_BOX, GimpPrefsBoxClass))
#define GIMP_IS_PREFS_BOX(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_PREFS_BOX))
#define GIMP_IS_PREFS_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_PREFS_BOX))
#define GIMP_PREFS_BOX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_PREFS_BOX, GimpPrefsBoxClass))


typedef struct _GimpPrefsBoxClass GimpPrefsBoxClass;

struct _GimpPrefsBox
{
  GtkBox  parent_instance;
};

struct _GimpPrefsBoxClass
{
  GtkBoxClass  parent_class;
};


GType       gimp_prefs_box_get_type      (void) G_GNUC_CONST;

GtkWidget * gimp_prefs_box_new           (void);

GtkWidget * gimp_prefs_box_add_page      (GimpPrefsBox *box,
                                          const gchar  *notebook_label,
                                          GdkPixbuf    *notebook_icon,
                                          const gchar  *tree_label,
                                          GdkPixbuf    *tree_icon,
                                          const gchar  *help_id,
                                          GtkTreeIter  *parent,
                                          GtkTreeIter  *iter);

GtkWidget * gimp_prefs_box_get_tree_view (GimpPrefsBox *box);
GtkWidget * gimp_prefs_box_get_notebook  (GimpPrefsBox *box);
GtkWidget * gimp_prefs_box_get_image     (GimpPrefsBox *box);


#endif  /*  __GIMP_PREFS_BOX_H__  */
