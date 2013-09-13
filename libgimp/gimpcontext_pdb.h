/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpcontext_pdb.h
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

#if !defined (__GIMP_H_INSIDE__) && !defined (GIMP_COMPILATION)
#error "Only <libgimp/gimp.h> can be included directly."
#endif

#ifndef __GIMP_CONTEXT_PDB_H__
#define __GIMP_CONTEXT_PDB_H__

G_BEGIN_DECLS

/* For information look into the C source or the html documentation */


gboolean               gimp_context_push                      (void);
gboolean               gimp_context_pop                       (void);
gboolean               gimp_context_set_defaults              (void);
gboolean               gimp_context_list_paint_methods        (gint                     *num_paint_methods,
                                                               gchar                  ***paint_methods);
gchar*                 gimp_context_get_paint_method          (void);
gboolean               gimp_context_set_paint_method          (const gchar              *name);
gboolean               gimp_context_get_foreground            (GimpRGB                  *foreground);
gboolean               gimp_context_set_foreground            (const GimpRGB            *foreground);
gboolean               gimp_context_get_background            (GimpRGB                  *background);
gboolean               gimp_context_set_background            (const GimpRGB            *background);
gboolean               gimp_context_set_default_colors        (void);
gboolean               gimp_context_swap_colors               (void);
gdouble                gimp_context_get_opacity               (void);
gboolean               gimp_context_set_opacity               (gdouble                   opacity);
GimpLayerModeEffects   gimp_context_get_paint_mode            (void);
gboolean               gimp_context_set_paint_mode            (GimpLayerModeEffects      paint_mode);
gchar*                 gimp_context_get_brush                 (void);
gboolean               gimp_context_set_brush                 (const gchar              *name);
gdouble                gimp_context_get_brush_size            (void);
gboolean               gimp_context_set_brush_size            (gdouble                   size);
gboolean               gimp_context_set_brush_default_size    (void);
gdouble                gimp_context_get_brush_aspect_ratio    (void);
gboolean               gimp_context_set_brush_aspect_ratio    (gdouble                   aspect);
gdouble                gimp_context_get_brush_angle           (void);
gboolean               gimp_context_set_brush_angle           (gdouble                   angle);
gchar*                 gimp_context_get_dynamics              (void);
gboolean               gimp_context_set_dynamics              (const gchar              *name);
gchar*                 gimp_context_get_pattern               (void);
gboolean               gimp_context_set_pattern               (const gchar              *name);
gchar*                 gimp_context_get_gradient              (void);
gboolean               gimp_context_set_gradient              (const gchar              *name);
gchar*                 gimp_context_get_palette               (void);
gboolean               gimp_context_set_palette               (const gchar              *name);
gchar*                 gimp_context_get_font                  (void);
gboolean               gimp_context_set_font                  (const gchar              *name);
gboolean               gimp_context_get_antialias             (void);
gboolean               gimp_context_set_antialias             (gboolean                  antialias);
gboolean               gimp_context_get_feather               (void);
gboolean               gimp_context_set_feather               (gboolean                  feather);
gboolean               gimp_context_get_feather_radius        (gdouble                  *feather_radius_x,
                                                               gdouble                  *feather_radius_y);
gboolean               gimp_context_set_feather_radius        (gdouble                   feather_radius_x,
                                                               gdouble                   feather_radius_y);
gboolean               gimp_context_get_sample_merged         (void);
gboolean               gimp_context_set_sample_merged         (gboolean                  sample_merged);
GimpSelectCriterion    gimp_context_get_sample_criterion      (void);
gboolean               gimp_context_set_sample_criterion      (GimpSelectCriterion       sample_criterion);
gdouble                gimp_context_get_sample_threshold      (void);
gboolean               gimp_context_set_sample_threshold      (gdouble                   sample_threshold);
gint                   gimp_context_get_sample_threshold_int  (void);
gboolean               gimp_context_set_sample_threshold_int  (gint                      sample_threshold);
gboolean               gimp_context_get_sample_transparent    (void);
gboolean               gimp_context_set_sample_transparent    (gboolean                  sample_transparent);
GimpInterpolationType  gimp_context_get_interpolation         (void);
gboolean               gimp_context_set_interpolation         (GimpInterpolationType     interpolation);
GimpTransformDirection gimp_context_get_transform_direction   (void);
gboolean               gimp_context_set_transform_direction   (GimpTransformDirection    transform_direction);
GimpTransformResize    gimp_context_get_transform_resize      (void);
gboolean               gimp_context_set_transform_resize      (GimpTransformResize       transform_resize);
GIMP_DEPRECATED
gint                   gimp_context_get_transform_recursion   (void);
GIMP_DEPRECATED
gboolean               gimp_context_set_transform_recursion   (gint                      transform_recursion);
gdouble                gimp_context_get_ink_size              (void);
gboolean               gimp_context_set_ink_size              (gdouble                   size);
gdouble                gimp_context_get_ink_angle             (void);
gboolean               gimp_context_set_ink_angle             (gdouble                   angle);
gdouble                gimp_context_get_ink_size_sensitivity  (void);
gboolean               gimp_context_set_ink_size_sensitivity  (gdouble                   size);
gdouble                gimp_context_get_ink_tilt_sensitivity  (void);
gboolean               gimp_context_set_ink_tilt_sensitivity  (gdouble                   tilt);
gdouble                gimp_context_get_ink_speed_sensitivity (void);
gboolean               gimp_context_set_ink_speed_sensitivity (gdouble                   speed);
GimpInkBlobType        gimp_context_get_ink_blob_type         (void);
gboolean               gimp_context_set_ink_blob_type         (GimpInkBlobType           type);
gdouble                gimp_context_get_ink_blob_aspect_ratio (void);
gboolean               gimp_context_set_ink_blob_aspect_ratio (gdouble                   aspect);
gdouble                gimp_context_get_ink_blob_angle        (void);
gboolean               gimp_context_set_ink_blob_angle        (gdouble                   angle);


G_END_DECLS

#endif /* __GIMP_CONTEXT_PDB_H__ */
