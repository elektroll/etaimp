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

#ifndef __INTERNAL_PROCS_H__
#define __INTERNAL_PROCS_H__

void   internal_procs_init               (GimpPDB *pdb);

/* Forward declarations for registering PDB procs */

void   register_brush_procs              (GimpPDB *pdb);
void   register_brush_select_procs       (GimpPDB *pdb);
void   register_brushes_procs            (GimpPDB *pdb);
void   register_buffer_procs             (GimpPDB *pdb);
void   register_channel_procs            (GimpPDB *pdb);
void   register_color_procs              (GimpPDB *pdb);
void   register_context_procs            (GimpPDB *pdb);
void   register_convert_procs            (GimpPDB *pdb);
void   register_display_procs            (GimpPDB *pdb);
void   register_drawable_procs           (GimpPDB *pdb);
void   register_drawable_transform_procs (GimpPDB *pdb);
void   register_dynamics_procs           (GimpPDB *pdb);
void   register_edit_procs               (GimpPDB *pdb);
void   register_fileops_procs            (GimpPDB *pdb);
void   register_floating_sel_procs       (GimpPDB *pdb);
void   register_font_select_procs        (GimpPDB *pdb);
void   register_fonts_procs              (GimpPDB *pdb);
void   register_gimp_procs               (GimpPDB *pdb);
void   register_gimprc_procs             (GimpPDB *pdb);
void   register_gradient_procs           (GimpPDB *pdb);
void   register_gradient_select_procs    (GimpPDB *pdb);
void   register_gradients_procs          (GimpPDB *pdb);
void   register_grid_procs               (GimpPDB *pdb);
void   register_guides_procs             (GimpPDB *pdb);
void   register_help_procs               (GimpPDB *pdb);
void   register_image_procs              (GimpPDB *pdb);
void   register_image_select_procs       (GimpPDB *pdb);
void   register_item_procs               (GimpPDB *pdb);
void   register_item_transform_procs     (GimpPDB *pdb);
void   register_layer_procs              (GimpPDB *pdb);
void   register_message_procs            (GimpPDB *pdb);
void   register_paint_tools_procs        (GimpPDB *pdb);
void   register_palette_procs            (GimpPDB *pdb);
void   register_palette_select_procs     (GimpPDB *pdb);
void   register_palettes_procs           (GimpPDB *pdb);
void   register_paths_procs              (GimpPDB *pdb);
void   register_pattern_procs            (GimpPDB *pdb);
void   register_pattern_select_procs     (GimpPDB *pdb);
void   register_patterns_procs           (GimpPDB *pdb);
void   register_plug_in_procs            (GimpPDB *pdb);
void   register_plug_in_compat_procs     (GimpPDB *pdb);
void   register_procedural_db_procs      (GimpPDB *pdb);
void   register_progress_procs           (GimpPDB *pdb);
void   register_selection_procs          (GimpPDB *pdb);
void   register_selection_tools_procs    (GimpPDB *pdb);
void   register_text_layer_procs         (GimpPDB *pdb);
void   register_text_tool_procs          (GimpPDB *pdb);
void   register_transform_tools_procs    (GimpPDB *pdb);
void   register_undo_procs               (GimpPDB *pdb);
void   register_unit_procs               (GimpPDB *pdb);
void   register_vectors_procs            (GimpPDB *pdb);

#endif /* __INTERNAL_PROCS_H__ */
