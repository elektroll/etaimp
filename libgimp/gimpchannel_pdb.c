/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpchannel_pdb.c
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
 * SECTION: gimpchannel
 * @title: gimpchannel
 * @short_description: Functions for manipulating channels.
 *
 * Functions for manipulating channels.
 **/


/**
 * _gimp_channel_new:
 * @image_ID: The image to which to add the channel.
 * @width: The channel width.
 * @height: The channel height.
 * @name: The channel name.
 * @opacity: The channel opacity.
 * @color: The channel compositing color.
 *
 * Create a new channel.
 *
 * This procedure creates a new channel with the specified width,
 * height, name, opacity and color. The new channel still needs to be
 * added to the image, as this is not automatic. Add the new channel
 * with gimp_image_insert_channel(). Other attributes, such as channel
 * visibility, should be set with explicit procedure calls. The
 * channel's contents are undefined initially.
 *
 * Returns: The newly created channel.
 **/
gint32
_gimp_channel_new (gint32         image_ID,
                   gint           width,
                   gint           height,
                   const gchar   *name,
                   gdouble        opacity,
                   const GimpRGB *color)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gint32 channel_ID = -1;

  return_vals = gimp_run_procedure ("gimp-channel-new",
                                    &nreturn_vals,
                                    GIMP_PDB_IMAGE, image_ID,
                                    GIMP_PDB_INT32, width,
                                    GIMP_PDB_INT32, height,
                                    GIMP_PDB_STRING, name,
                                    GIMP_PDB_FLOAT, opacity,
                                    GIMP_PDB_COLOR, color,
                                    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    channel_ID = return_vals[1].data.d_channel;

  gimp_destroy_params (return_vals, nreturn_vals);

  return channel_ID;
}

/**
 * gimp_channel_new_from_component:
 * @image_ID: The image to which to add the channel.
 * @component: The image component.
 * @name: The channel name.
 *
 * Create a new channel from a color component
 *
 * This procedure creates a new channel from a color component. The new
 * channel still needs to be added to the image, as this is not
 * automatic. Add the new channel with gimp_image_insert_channel().
 * Other attributes, such as channel visibility, should be set with
 * explicit procedure calls.
 *
 * Returns: The newly created channel.
 *
 * Since: GIMP 2.4
 **/
gint32
gimp_channel_new_from_component (gint32           image_ID,
                                 GimpChannelType  component,
                                 const gchar     *name)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gint32 channel_ID = -1;

  return_vals = gimp_run_procedure ("gimp-channel-new-from-component",
                                    &nreturn_vals,
                                    GIMP_PDB_IMAGE, image_ID,
                                    GIMP_PDB_INT32, component,
                                    GIMP_PDB_STRING, name,
                                    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    channel_ID = return_vals[1].data.d_channel;

  gimp_destroy_params (return_vals, nreturn_vals);

  return channel_ID;
}

/**
 * gimp_channel_copy:
 * @channel_ID: The channel to copy.
 *
 * Copy a channel.
 *
 * This procedure copies the specified channel and returns the copy.
 * The new channel still needs to be added to the image, as this is not
 * automatic. Add the new channel with gimp_image_insert_channel().
 *
 * Returns: The newly copied channel.
 **/
gint32
gimp_channel_copy (gint32 channel_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gint32 channel_copy_ID = -1;

  return_vals = gimp_run_procedure ("gimp-channel-copy",
                                    &nreturn_vals,
                                    GIMP_PDB_CHANNEL, channel_ID,
                                    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    channel_copy_ID = return_vals[1].data.d_channel;

  gimp_destroy_params (return_vals, nreturn_vals);

  return channel_copy_ID;
}

/**
 * gimp_channel_combine_masks:
 * @channel1_ID: The channel1.
 * @channel2_ID: The channel2.
 * @operation: The selection operation.
 * @offx: x offset between upper left corner of channels: (second - first).
 * @offy: y offset between upper left corner of channels: (second - first).
 *
 * Combine two channel masks.
 *
 * This procedure combines two channel masks. The result is stored in
 * the first channel.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_channel_combine_masks (gint32         channel1_ID,
                            gint32         channel2_ID,
                            GimpChannelOps operation,
                            gint           offx,
                            gint           offy)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-channel-combine-masks",
                                    &nreturn_vals,
                                    GIMP_PDB_CHANNEL, channel1_ID,
                                    GIMP_PDB_CHANNEL, channel2_ID,
                                    GIMP_PDB_INT32, operation,
                                    GIMP_PDB_INT32, offx,
                                    GIMP_PDB_INT32, offy,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_channel_get_show_masked:
 * @channel_ID: The channel.
 *
 * Get the composite method of the specified channel.
 *
 * This procedure returns the specified channel's composite method. If
 * it is TRUE, then the channel is composited with the image so that
 * masked regions are shown. Otherwise, selected regions are shown.
 *
 * Returns: The channel composite method.
 **/
gboolean
gimp_channel_get_show_masked (gint32 channel_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean show_masked = FALSE;

  return_vals = gimp_run_procedure ("gimp-channel-get-show-masked",
                                    &nreturn_vals,
                                    GIMP_PDB_CHANNEL, channel_ID,
                                    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    show_masked = return_vals[1].data.d_int32;

  gimp_destroy_params (return_vals, nreturn_vals);

  return show_masked;
}

/**
 * gimp_channel_set_show_masked:
 * @channel_ID: The channel.
 * @show_masked: The new channel composite method.
 *
 * Set the composite method of the specified channel.
 *
 * This procedure sets the specified channel's composite method. If it
 * is TRUE, then the channel is composited with the image so that
 * masked regions are shown. Otherwise, selected regions are shown.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_channel_set_show_masked (gint32   channel_ID,
                              gboolean show_masked)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-channel-set-show-masked",
                                    &nreturn_vals,
                                    GIMP_PDB_CHANNEL, channel_ID,
                                    GIMP_PDB_INT32, show_masked,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_channel_get_opacity:
 * @channel_ID: The channel.
 *
 * Get the opacity of the specified channel.
 *
 * This procedure returns the specified channel's opacity.
 *
 * Returns: The channel opacity.
 **/
gdouble
gimp_channel_get_opacity (gint32 channel_ID)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gdouble opacity = 0.0;

  return_vals = gimp_run_procedure ("gimp-channel-get-opacity",
                                    &nreturn_vals,
                                    GIMP_PDB_CHANNEL, channel_ID,
                                    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    opacity = return_vals[1].data.d_float;

  gimp_destroy_params (return_vals, nreturn_vals);

  return opacity;
}

/**
 * gimp_channel_set_opacity:
 * @channel_ID: The channel.
 * @opacity: The new channel opacity.
 *
 * Set the opacity of the specified channel.
 *
 * This procedure sets the specified channel's opacity.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_channel_set_opacity (gint32  channel_ID,
                          gdouble opacity)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-channel-set-opacity",
                                    &nreturn_vals,
                                    GIMP_PDB_CHANNEL, channel_ID,
                                    GIMP_PDB_FLOAT, opacity,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_channel_get_color:
 * @channel_ID: The channel.
 * @color: The channel compositing color.
 *
 * Get the compositing color of the specified channel.
 *
 * This procedure returns the specified channel's compositing color.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_channel_get_color (gint32   channel_ID,
                        GimpRGB *color)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-channel-get-color",
                                    &nreturn_vals,
                                    GIMP_PDB_CHANNEL, channel_ID,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    *color = return_vals[1].data.d_color;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_channel_set_color:
 * @channel_ID: The channel.
 * @color: The new channel compositing color.
 *
 * Set the compositing color of the specified channel.
 *
 * This procedure sets the specified channel's compositing color.
 *
 * Returns: TRUE on success.
 **/
gboolean
gimp_channel_set_color (gint32         channel_ID,
                        const GimpRGB *color)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-channel-set-color",
                                    &nreturn_vals,
                                    GIMP_PDB_CHANNEL, channel_ID,
                                    GIMP_PDB_COLOR, color,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}
