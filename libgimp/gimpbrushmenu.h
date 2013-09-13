/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimpbrushmenu.h
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

#if !defined (__GIMP_UI_H_INSIDE__) && !defined (GIMP_COMPILATION)
#error "Only <libgimp/gimpui.h> can be included directly."
#endif

#ifndef __GIMP_BRUSH_MENU_H__
#define __GIMP_BRUSH_MENU_H__

/*  These functions are deprecated and should not be used in newly
 *  written code.
 */

G_BEGIN_DECLS

GIMP_DEPRECATED_FOR(gimp_brush_select_button_new)
GtkWidget * gimp_brush_select_widget_new   (const gchar          *title,
                                            const gchar          *brush_name,
                                            gdouble               opacity,
                                            gint                  spacing,
                                            GimpLayerModeEffects  paint_mode,
                                            GimpRunBrushCallback  callback,
                                            gpointer              data);

GIMP_DEPRECATED_FOR(gimp_select_button_close_popup)
void        gimp_brush_select_widget_close (GtkWidget            *widget);
GIMP_DEPRECATED_FOR(gimp_brush_select_button_set_brush)
void        gimp_brush_select_widget_set   (GtkWidget            *widget,
                                            const gchar          *brush_name,
                                            gdouble               opacity,
                                            gint                  spacing,
                                            GimpLayerModeEffects  paint_mode);


G_END_DECLS

#endif /* __GIMP_BRUSH_MENU_H__ */
