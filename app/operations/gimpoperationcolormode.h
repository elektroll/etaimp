/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpoperationcolormode.h
 * Copyright (C) 2008 Michael Natterer <mitch@gimp.org>
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

#ifndef __GIMP_OPERATION_COLOR_MODE_H__
#define __GIMP_OPERATION_COLOR_MODE_H__


#include "gimpoperationpointlayermode.h"


#define GIMP_TYPE_OPERATION_COLOR_MODE            (gimp_operation_color_mode_get_type ())
#define GIMP_OPERATION_COLOR_MODE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_OPERATION_COLOR_MODE, GimpOperationColorMode))
#define GIMP_OPERATION_COLOR_MODE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GIMP_TYPE_OPERATION_COLOR_MODE, GimpOperationColorModeClass))
#define GIMP_IS_OPERATION_COLOR_MODE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_OPERATION_COLOR_MODE))
#define GIMP_IS_OPERATION_COLOR_MODE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GIMP_TYPE_OPERATION_COLOR_MODE))
#define GIMP_OPERATION_COLOR_MODE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GIMP_TYPE_OPERATION_COLOR_MODE, GimpOperationColorModeClass))


typedef struct _GimpOperationColorMode      GimpOperationColorMode;
typedef struct _GimpOperationColorModeClass GimpOperationColorModeClass;

struct _GimpOperationColorMode
{
  GimpOperationPointLayerMode  parent_instance;
};

struct _GimpOperationColorModeClass
{
  GimpOperationPointLayerModeClass  parent_class;
};


GType   gimp_operation_color_mode_get_type (void) G_GNUC_CONST;

gboolean gimp_operation_color_mode_process_pixels (gfloat              *in,
                                                   gfloat              *layer,
                                                   gfloat              *mask,
                                                   gfloat              *out,
                                                   gfloat               opacity,
                                                   glong                samples,
                                                   const GeglRectangle *roi,
                                                   gint                 level);

#endif /* __GIMP_OPERATION_COLOR_MODE_H__ */
