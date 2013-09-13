/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpoperationnormalmode.h
 * Copyright (C) 2012 Michael Natterer <mitch@gimp.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __GIMP_OPERATION_NORMAL_MODE_H__
#define __GIMP_OPERATION_NORMAL_MODE_H__


#include "gimpoperationpointlayermode.h"


#define GIMP_TYPE_OPERATION_NORMAL_MODE            (gimp_operation_normal_mode_get_type ())
#define GIMP_OPERATION_NORMAL_MODE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_OPERATION_NORMAL_MODE, GimpOperationNormalMode))
#define GIMP_OPERATION_NORMAL_MODE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GIMP_TYPE_OPERATION_NORMAL_MODE, GimpOperationNormalModeClass))
#define GIMP_IS_OPERATION_NORMAL_MODE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_OPERATION_NORMAL_MODE))
#define GIMP_IS_OPERATION_NORMAL_MODE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GIMP_TYPE_OPERATION_NORMAL_MODE))
#define GIMP_OPERATION_NORMAL_MODE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GIMP_TYPE_OPERATION_NORMAL_MODE, GimpOperationNormalModeClass))


typedef struct _GimpOperationNormalMode      GimpOperationNormalMode;
typedef struct _GimpOperationNormalModeClass GimpOperationNormalModeClass;

struct _GimpOperationNormalMode
{
  GimpOperationPointLayerMode  parent_instance;
};

struct _GimpOperationNormalModeClass
{
  GimpOperationPointLayerModeClass  parent_class;
};


GType   gimp_operation_normal_mode_get_type (void) G_GNUC_CONST;

extern GimpLayerModeFunction gimp_operation_normal_mode_process_pixels;

gboolean gimp_operation_normal_mode_process_pixels_core (gfloat              *in,
                                                         gfloat              *aux,
                                                         gfloat              *mask,
                                                         gfloat              *out,
                                                         gfloat               opacity,
                                                         glong                samples,
                                                         const GeglRectangle *roi,
                                                         gint                 level);

gboolean gimp_operation_normal_mode_process_pixels_sse2 (gfloat              *in,
                                                         gfloat              *aux,
                                                         gfloat              *mask,
                                                         gfloat              *out,
                                                         gfloat               opacity,
                                                         glong                samples,
                                                         const GeglRectangle *roi,
                                                         gint                 level);

gboolean gimp_operation_normal_mode_process_pixels_sse4 (gfloat              *in,
                                                         gfloat              *aux,
                                                         gfloat              *mask,
                                                         gfloat              *out,
                                                         gfloat               opacity,
                                                         glong                samples,
                                                         const GeglRectangle *roi,
                                                         gint                 level);

#endif /* __GIMP_OPERATION_NORMAL_MODE_H__ */
