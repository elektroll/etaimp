/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimphistogram module Copyright (C) 1999 Jay Cox <jaycox@gimp.org>
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

#include <string.h>

#include <gegl.h>

#include "libgimpmath/gimpmath.h"

#include "core-types.h"

#include "gegl/gimp-babl.h"

#include "gimphistogram.h"


enum
{
  PROP_0,
  PROP_N_CHANNELS,
  PROP_N_BINS,
  PROP_VALUES
};

struct _GimpHistogramPrivate
{
  gboolean gamma_correct;
  gint     n_channels;
  gint     n_bins;
  gdouble *values;
};


/*  local function prototypes  */

static void     gimp_histogram_finalize     (GObject       *object);
static void     gimp_histogram_set_property (GObject       *object,
                                             guint          property_id,
                                             const GValue  *value,
                                             GParamSpec    *pspec);
static void     gimp_histogram_get_property (GObject       *object,
                                             guint          property_id,
                                             GValue        *value,
                                             GParamSpec    *pspec);

static gint64   gimp_histogram_get_memsize  (GimpObject    *object,
                                             gint64        *gui_size);

static void     gimp_histogram_alloc_values (GimpHistogram *histogram,
                                             gint           n_components,
                                             gint           n_bins);


G_DEFINE_TYPE (GimpHistogram, gimp_histogram, GIMP_TYPE_OBJECT)

#define parent_class gimp_histogram_parent_class


static void
gimp_histogram_class_init (GimpHistogramClass *klass)
{
  GObjectClass      *object_class      = G_OBJECT_CLASS (klass);
  GimpObjectClass   *gimp_object_class = GIMP_OBJECT_CLASS (klass);

  object_class->finalize         = gimp_histogram_finalize;
  object_class->set_property     = gimp_histogram_set_property;
  object_class->get_property     = gimp_histogram_get_property;

  gimp_object_class->get_memsize = gimp_histogram_get_memsize;

  g_object_class_install_property (object_class, PROP_N_CHANNELS,
                                   g_param_spec_int ("n-channels", NULL, NULL,
                                                     0, 5, 0,
                                                     GIMP_PARAM_READABLE));

  g_object_class_install_property (object_class, PROP_N_BINS,
                                   g_param_spec_int ("n-bins", NULL, NULL,
                                                     256, 1024, 1024,
                                                     GIMP_PARAM_READABLE));

  /* this is just for notifications */
  g_object_class_install_property (object_class, PROP_VALUES,
                                   g_param_spec_boolean ("values", NULL, NULL,
                                                         FALSE,
                                                         G_PARAM_READABLE));

  g_type_class_add_private (klass, sizeof (GimpHistogramPrivate));
}

static void
gimp_histogram_init (GimpHistogram *histogram)
{
  histogram->priv = G_TYPE_INSTANCE_GET_PRIVATE (histogram,
                                                 GIMP_TYPE_HISTOGRAM,
                                                 GimpHistogramPrivate);

  histogram->priv->n_bins = 256;
}

static void
gimp_histogram_finalize (GObject *object)
{
  GimpHistogram *histogram = GIMP_HISTOGRAM (object);

  gimp_histogram_clear_values (histogram);

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gimp_histogram_set_property (GObject      *object,
                             guint         property_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  switch (property_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gimp_histogram_get_property (GObject    *object,
                             guint       property_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  GimpHistogram *histogram = GIMP_HISTOGRAM (object);

  switch (property_id)
    {
    case PROP_N_CHANNELS:
      g_value_set_int (value, histogram->priv->n_channels);
      break;

    case PROP_N_BINS:
      g_value_set_int (value, histogram->priv->n_bins);
      break;

    case PROP_VALUES:
      /* return a silly boolean */
      g_value_set_boolean (value, histogram->priv->values != NULL);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static gint64
gimp_histogram_get_memsize (GimpObject *object,
                            gint64     *gui_size)
{
  GimpHistogram *histogram = GIMP_HISTOGRAM (object);
  gint64         memsize   = 0;

  if (histogram->priv->values)
    memsize += (histogram->priv->n_channels *
                histogram->priv->n_bins * sizeof (gdouble));

  return memsize + GIMP_OBJECT_CLASS (parent_class)->get_memsize (object,
                                                                  gui_size);
}

/*  public functions  */

GimpHistogram *
gimp_histogram_new (gboolean gamma_correct)
{
  GimpHistogram *histogram = g_object_new (GIMP_TYPE_HISTOGRAM, NULL);

  histogram->priv->gamma_correct = gamma_correct;

  return histogram;
}

/**
 * gimp_histogram_duplicate:
 * @histogram: a %GimpHistogram
 *
 * Creates a duplicate of @histogram. The duplicate has a reference
 * count of 1 and contains the values from @histogram.
 *
 * Return value: a newly allocated %GimpHistogram
 **/
GimpHistogram *
gimp_histogram_duplicate (GimpHistogram *histogram)
{
  GimpHistogram *dup;

  g_return_val_if_fail (GIMP_IS_HISTOGRAM (histogram), NULL);

  dup = gimp_histogram_new (histogram->priv->gamma_correct);

  dup->priv->n_channels = histogram->priv->n_channels;
  dup->priv->n_bins     = histogram->priv->n_bins;
  dup->priv->values     = g_memdup (histogram->priv->values,
                                    sizeof (gdouble) *
                                    dup->priv->n_channels *
                                    dup->priv->n_bins);

  return dup;
}

void
gimp_histogram_calculate (GimpHistogram       *histogram,
                          GeglBuffer          *buffer,
                          const GeglRectangle *buffer_rect,
                          GeglBuffer          *mask,
                          const GeglRectangle *mask_rect)
{
  GimpHistogramPrivate *priv;
  GeglBufferIterator   *iter;
  const Babl           *format;
  gint                  n_components;
  gint                  n_bins;

  g_return_if_fail (GIMP_IS_HISTOGRAM (histogram));
  g_return_if_fail (GEGL_IS_BUFFER (buffer));
  g_return_if_fail (buffer_rect != NULL);

  priv = histogram->priv;

  format = gegl_buffer_get_format (buffer);

  if (babl_format_get_type (format, 0) == babl_type ("u8"))
    n_bins = 256;
  else
    n_bins = 1024;

  if (babl_format_is_palette (format))
    {
      if (babl_format_has_alpha (format))
        format = babl_format ("R'G'B'A float");
      else
        format = babl_format ("R'G'B' float");
    }
  else
    {
      const Babl *model = babl_format_get_model (format);

      if (model == babl_model ("Y"))
        {
          if (priv->gamma_correct)
            format = babl_format ("Y' float");
          else
            format = babl_format ("Y float");
        }
      else if (model == babl_model ("Y'"))
        {
          format = babl_format ("Y' float");
        }
      else if (model == babl_model ("YA"))
        {
          if (priv->gamma_correct)
            format = babl_format ("Y'A float");
          else
            format = babl_format ("YA float");
        }
      else if (model == babl_model ("Y'A"))
        {
          format = babl_format ("Y'A float");
        }
      else if (model == babl_model ("RGB"))
        {
          if (priv->gamma_correct)
            format = babl_format ("R'G'B' float");
          else
            format = babl_format ("RGB float");
        }
      else if (model == babl_model ("R'G'B'"))
        {
          format = babl_format ("R'G'B' float");
        }
      else if (model == babl_model ("RGBA"))
        {
          if (priv->gamma_correct)
            format = babl_format ("R'G'B'A float");
          else
            format = babl_format ("RGBA float");
        }
      else if (model == babl_model ("R'G'B'A"))
        {
          format = babl_format ("R'G'B'A float");
        }
      else
        {
          g_return_if_reached ();
        }
    }

  n_components = babl_format_get_n_components (format);

  g_object_freeze_notify (G_OBJECT (histogram));

  gimp_histogram_alloc_values (histogram, n_components, n_bins);

  iter = gegl_buffer_iterator_new (buffer, buffer_rect, 0, format,
                                   GEGL_BUFFER_READ, GEGL_ABYSS_NONE);

  if (mask)
    gegl_buffer_iterator_add (iter, mask, mask_rect, 0,
                              babl_format ("Y float"),
                              GEGL_BUFFER_READ, GEGL_ABYSS_NONE);

#define VALUE(c,i) (priv->values[(c) * priv->n_bins + \
                                 (gint) (CLAMP ((i), 0.0, 1.0) * \
                                         (priv->n_bins - 0.0001))])

  while (gegl_buffer_iterator_next (iter))
    {
      const gfloat *data   = iter->data[0];
      gint          length = iter->length;
      gfloat        max;

      if (mask)
        {
          const gfloat *mask_data = iter->data[1];

          switch (n_components)
            {
            case 1:
              while (length--)
                {
                  const gdouble masked = *mask_data;

                  VALUE (0, data[0]) += masked;

                  data += n_components;
                  mask_data += 1;
                }
              break;

            case 2:
              while (length--)
                {
                  const gdouble masked = *mask_data;
                  const gdouble weight = data[1];

                  VALUE (0, data[0]) += weight * masked;
                  VALUE (1, data[1]) += masked;

                  data += n_components;
                  mask_data += 1;
                }
              break;

            case 3: /* calculate separate value values */
              while (length--)
                {
                  const gdouble masked = *mask_data;

                  VALUE (1, data[0]) += masked;
                  VALUE (2, data[1]) += masked;
                  VALUE (3, data[2]) += masked;

                  max = MAX (data[0], data[1]);
                  max = MAX (data[2], max);

                  VALUE (0, max) += masked;

                  data += n_components;
                  mask_data += 1;
                }
              break;

            case 4: /* calculate separate value values */
              while (length--)
                {
                  const gdouble masked = *mask_data;
                  const gdouble weight = data[3];

                  VALUE (1, data[0]) += weight * masked;
                  VALUE (2, data[1]) += weight * masked;
                  VALUE (3, data[2]) += weight * masked;
                  VALUE (4, data[3]) += masked;

                  max = MAX (data[0], data[1]);
                  max = MAX (data[2], max);

                  VALUE (0, max) += weight * masked;

                  data += n_components;
                  mask_data += 1;
                }
              break;
            }
        }
      else /* no mask */
        {
          switch (n_components)
            {
            case 1:
              while (length--)
                {
                  VALUE (0, data[0]) += 1.0;

                  data += n_components;
                }
              break;

            case 2:
              while (length--)
                {
                  const gdouble weight = data[1];

                  VALUE (0, data[0]) += weight;
                  VALUE (1, data[1]) += 1.0;

                  data += n_components;
                }
              break;

            case 3: /* calculate separate value values */
              while (length--)
                {
                  VALUE (1, data[0]) += 1.0;
                  VALUE (2, data[1]) += 1.0;
                  VALUE (3, data[2]) += 1.0;

                  max = MAX (data[0], data[1]);
                  max = MAX (data[2], max);

                  VALUE (0, max) += 1.0;

                  data += n_components;
                }
              break;

            case 4: /* calculate separate value values */
              while (length--)
                {
                  const gdouble weight = data[3];

                  VALUE (1, data[0]) += weight;
                  VALUE (2, data[1]) += weight;
                  VALUE (3, data[2]) += weight;
                  VALUE (4, data[3]) += 1.0;

                  max = MAX (data[0], data[1]);
                  max = MAX (data[2], max);

                  VALUE (0, max) += weight;

                  data += n_components;
                }
              break;
            }
        }
    }

  g_object_notify (G_OBJECT (histogram), "values");

  g_object_thaw_notify (G_OBJECT (histogram));

#undef VALUE
}

void
gimp_histogram_clear_values (GimpHistogram *histogram)
{
  g_return_if_fail (GIMP_IS_HISTOGRAM (histogram));

  if (histogram->priv->values)
    {
      g_free (histogram->priv->values);
      histogram->priv->values = NULL;

      g_object_notify (G_OBJECT (histogram), "values");
    }

  if (histogram->priv->n_channels)
    {
      histogram->priv->n_channels = 0;

      g_object_notify (G_OBJECT (histogram), "n-channels");
    }
}


#define HISTOGRAM_VALUE(c,i) (priv->values[(c) * priv->n_bins + (i)])


gdouble
gimp_histogram_get_maximum (GimpHistogram        *histogram,
                            GimpHistogramChannel  channel)
{
  GimpHistogramPrivate *priv;
  gdouble               max = 0.0;
  gint                  x;

  g_return_val_if_fail (GIMP_IS_HISTOGRAM (histogram), 0.0);

  priv = histogram->priv;

  /*  the gray alpha channel is in slot 1  */
  if (priv->n_channels == 3 && channel == GIMP_HISTOGRAM_ALPHA)
    channel = 1;

  if (! priv->values ||
      (channel != GIMP_HISTOGRAM_RGB && channel >= priv->n_channels))
    return 0.0;

  if (channel == GIMP_HISTOGRAM_RGB)
    {
      for (x = 0; x < priv->n_bins; x++)
        {
          max = MAX (max, HISTOGRAM_VALUE (GIMP_HISTOGRAM_RED,   x));
          max = MAX (max, HISTOGRAM_VALUE (GIMP_HISTOGRAM_GREEN, x));
          max = MAX (max, HISTOGRAM_VALUE (GIMP_HISTOGRAM_BLUE,  x));
        }
    }
  else
    {
      for (x = 0; x < priv->n_bins; x++)
        {
          max = MAX (max, HISTOGRAM_VALUE (channel, x));
        }
    }

  return max;
}

gdouble
gimp_histogram_get_value (GimpHistogram        *histogram,
                          GimpHistogramChannel  channel,
                          gint                  bin)
{
  GimpHistogramPrivate *priv;

  g_return_val_if_fail (GIMP_IS_HISTOGRAM (histogram), 0.0);

  priv = histogram->priv;

  /*  the gray alpha channel is in slot 1  */
  if (priv->n_channels == 3 && channel == GIMP_HISTOGRAM_ALPHA)
    channel = 1;

  if (! priv->values ||
      bin < 0 || bin >= priv->n_bins ||
      (channel == GIMP_HISTOGRAM_RGB && priv->n_channels < 4) ||
      (channel != GIMP_HISTOGRAM_RGB && channel >= priv->n_channels))
    return 0.0;

  if (channel == GIMP_HISTOGRAM_RGB)
    {
      gdouble min = HISTOGRAM_VALUE (GIMP_HISTOGRAM_RED, bin);

      min = MIN (min, HISTOGRAM_VALUE (GIMP_HISTOGRAM_GREEN, bin));

      return MIN (min, HISTOGRAM_VALUE (GIMP_HISTOGRAM_BLUE, bin));
    }
  else
    {
      return HISTOGRAM_VALUE (channel, bin);
    }
}

gdouble
gimp_histogram_get_component (GimpHistogram *histogram,
                              gint           component,
                              gint           bin)
{
  g_return_val_if_fail (GIMP_IS_HISTOGRAM (histogram), 0.0);

  if (histogram->priv->n_channels > 3)
    component++;

  return gimp_histogram_get_value (histogram, component, bin);
}

gint
gimp_histogram_n_channels (GimpHistogram *histogram)
{
  g_return_val_if_fail (GIMP_IS_HISTOGRAM (histogram), 0);

  return histogram->priv->n_channels - 1;
}

gint
gimp_histogram_n_bins (GimpHistogram *histogram)
{
  g_return_val_if_fail (GIMP_IS_HISTOGRAM (histogram), 0);

  return histogram->priv->n_bins;
}

gdouble
gimp_histogram_get_count (GimpHistogram        *histogram,
                          GimpHistogramChannel  channel,
                          gint                  start,
                          gint                  end)
{
  GimpHistogramPrivate *priv;
  gint                  i;
  gdouble               count = 0.0;

  g_return_val_if_fail (GIMP_IS_HISTOGRAM (histogram), 0.0);

  priv = histogram->priv;

  /*  the gray alpha channel is in slot 1  */
  if (priv->n_channels == 3 && channel == GIMP_HISTOGRAM_ALPHA)
    channel = 1;

  if (channel == GIMP_HISTOGRAM_RGB)
    return (gimp_histogram_get_count (histogram,
                                      GIMP_HISTOGRAM_RED, start, end)   +
            gimp_histogram_get_count (histogram,
                                      GIMP_HISTOGRAM_GREEN, start, end) +
            gimp_histogram_get_count (histogram,
                                      GIMP_HISTOGRAM_BLUE, start, end));

  if (! priv->values ||
      start > end ||
      channel >= priv->n_channels)
    return 0.0;

  start = CLAMP (start, 0, priv->n_bins - 1);
  end   = CLAMP (end,   0, priv->n_bins - 1);

  for (i = start; i <= end; i++)
    count += HISTOGRAM_VALUE (channel, i);

  return count;
}

gdouble
gimp_histogram_get_mean (GimpHistogram        *histogram,
                         GimpHistogramChannel  channel,
                         gint                  start,
                         gint                  end)
{
  GimpHistogramPrivate *priv;
  gint                  i;
  gdouble               mean = 0.0;
  gdouble               count;

  g_return_val_if_fail (GIMP_IS_HISTOGRAM (histogram), 0.0);

  priv = histogram->priv;

  /*  the gray alpha channel is in slot 1  */
  if (priv->n_channels == 3 && channel == GIMP_HISTOGRAM_ALPHA)
    channel = 1;

  if (! priv->values ||
      start > end ||
      (channel == GIMP_HISTOGRAM_RGB && priv->n_channels < 4) ||
      (channel != GIMP_HISTOGRAM_RGB && channel >= priv->n_channels))
    return 0.0;

  start = CLAMP (start, 0, priv->n_bins - 1);
  end   = CLAMP (end,   0, priv->n_bins - 1);

  if (channel == GIMP_HISTOGRAM_RGB)
    {
      for (i = start; i <= end; i++)
        {
          gdouble factor = (gdouble) i / (gdouble)  (priv->n_bins - 1);

          mean += (factor * HISTOGRAM_VALUE (GIMP_HISTOGRAM_RED,   i) +
                   factor * HISTOGRAM_VALUE (GIMP_HISTOGRAM_GREEN, i) +
                   factor * HISTOGRAM_VALUE (GIMP_HISTOGRAM_BLUE,  i));
        }
    }
  else
    {
      for (i = start; i <= end; i++)
        {
          gdouble factor = (gdouble) i / (gdouble)  (priv->n_bins - 1);

          mean += factor * HISTOGRAM_VALUE (channel, i);
        }
    }

  count = gimp_histogram_get_count (histogram, channel, start, end);

  if (count > 0.0)
    return mean / count;

  return mean;
}

gdouble
gimp_histogram_get_median (GimpHistogram         *histogram,
                           GimpHistogramChannel   channel,
                           gint                   start,
                           gint                   end)
{
  GimpHistogramPrivate *priv;
  gint                  i;
  gdouble               sum = 0.0;
  gdouble               count;

  g_return_val_if_fail (GIMP_IS_HISTOGRAM (histogram), -1.0);

  priv = histogram->priv;

  /*  the gray alpha channel is in slot 1  */
  if (priv->n_channels == 3 && channel == GIMP_HISTOGRAM_ALPHA)
    channel = 1;

  if (! priv->values ||
      start > end ||
      (channel == GIMP_HISTOGRAM_RGB && priv->n_channels < 4) ||
      (channel != GIMP_HISTOGRAM_RGB && channel >= priv->n_channels))
    return 0.0;

  start = CLAMP (start, 0, priv->n_bins - 1);
  end   = CLAMP (end,   0, priv->n_bins - 1);

  count = gimp_histogram_get_count (histogram, channel, start, end);

  if (channel == GIMP_HISTOGRAM_RGB)
    {
      for (i = start; i <= end; i++)
        {
          sum += (HISTOGRAM_VALUE (GIMP_HISTOGRAM_RED,   i) +
                  HISTOGRAM_VALUE (GIMP_HISTOGRAM_GREEN, i) +
                  HISTOGRAM_VALUE (GIMP_HISTOGRAM_BLUE,  i));

          if (sum * 2 > count)
            return ((gdouble) i / (gdouble)  (priv->n_bins - 1));
        }
    }
  else
    {
      for (i = start; i <= end; i++)
        {
          sum += HISTOGRAM_VALUE (channel, i);

          if (sum * 2 > count)
            return ((gdouble) i / (gdouble)  (priv->n_bins - 1));
        }
    }

  return -1.0;
}

/*
 * adapted from GNU ocrad 0.14 : page_image_io.cc : otsu_th
 *
 *  N. Otsu, "A threshold selection method from gray-level histograms,"
 *  IEEE Trans. Systems, Man, and Cybernetics, vol. 9, no. 1, pp. 62-66, 1979.
 */
gdouble
gimp_histogram_get_threshold (GimpHistogram        *histogram,
                              GimpHistogramChannel  channel,
                              gint                  start,
                              gint                  end)
{
  GimpHistogramPrivate *priv;
  gint                 i;
  gint                 maxval;
  gdouble             *hist      = NULL;
  gdouble             *chist     = NULL;
  gdouble             *cmom      = NULL;
  gdouble              hist_max  = 0.0;
  gdouble              chist_max = 0.0;
  gdouble              cmom_max  = 0.0;
  gdouble              bvar_max  = 0.0;
  gint                 threshold = 127;

  g_return_val_if_fail (GIMP_IS_HISTOGRAM (histogram), -1);

  priv = histogram->priv;

  /*  the gray alpha channel is in slot 1  */
  if (priv->n_channels == 3 && channel == GIMP_HISTOGRAM_ALPHA)
    channel = 1;

  if (! priv->values ||
      start > end ||
      (channel == GIMP_HISTOGRAM_RGB && priv->n_channels < 4) ||
      (channel != GIMP_HISTOGRAM_RGB && channel >= priv->n_channels))
    return 0;

  start = CLAMP (start, 0, priv->n_bins - 1);
  end   = CLAMP (end,   0, priv->n_bins - 1);

  maxval = end - start;

  hist  = g_newa (gdouble, maxval + 1);
  chist = g_newa (gdouble, maxval + 1);
  cmom  = g_newa (gdouble, maxval + 1);

  if (channel == GIMP_HISTOGRAM_RGB)
    {
      for (i = start; i <= end; i++)
        hist[i - start] = (HISTOGRAM_VALUE (GIMP_HISTOGRAM_RED,   i) +
                           HISTOGRAM_VALUE (GIMP_HISTOGRAM_GREEN, i) +
                           HISTOGRAM_VALUE (GIMP_HISTOGRAM_BLUE,  i));
    }
  else
    {
      for (i = start; i <= end; i++)
        hist[i - start] = HISTOGRAM_VALUE (channel, i);
    }

  hist_max = hist[0];
  chist[0] = hist[0];
  cmom[0] = 0;

  for (i = 1; i <= maxval; i++)
    {
      if (hist[i] > hist_max)
	hist_max = hist[i];

      chist[i] = chist[i-1] + hist[i];
      cmom[i] = cmom[i-1] + i * hist[i];
    }

  chist_max = chist[maxval];
  cmom_max = cmom[maxval];
  bvar_max = 0;

  for (i = 0; i < maxval; ++i)
    {
      if (chist[i] > 0 && chist[i] < chist_max)
        {
          gdouble bvar;

          bvar = (gdouble) cmom[i] / chist[i];
          bvar -= (cmom_max - cmom[i]) / (chist_max - chist[i]);
          bvar *= bvar;
          bvar *= chist[i];
          bvar *= chist_max - chist[i];

          if (bvar > bvar_max)
            {
              bvar_max = bvar;
              threshold = start + i;
            }
        }
    }

  return threshold;
}

gdouble
gimp_histogram_get_std_dev (GimpHistogram        *histogram,
                            GimpHistogramChannel  channel,
                            gint                  start,
                            gint                  end)
{
  GimpHistogramPrivate *priv;
  gint                  i;
  gdouble               dev = 0.0;
  gdouble               count;
  gdouble               mean;

  g_return_val_if_fail (GIMP_IS_HISTOGRAM (histogram), 0.0);

  priv = histogram->priv;

  /*  the gray alpha channel is in slot 1  */
  if (priv->n_channels == 3 && channel == GIMP_HISTOGRAM_ALPHA)
    channel = 1;

  if (! priv->values ||
      start > end ||
      (channel == GIMP_HISTOGRAM_RGB && priv->n_channels < 4) ||
      (channel != GIMP_HISTOGRAM_RGB && channel >= priv->n_channels))
    return 0.0;

  mean  = gimp_histogram_get_mean  (histogram, channel, start, end);
  count = gimp_histogram_get_count (histogram, channel, start, end);

  if (count == 0.0)
    count = 1.0;

  for (i = start; i <= end; i++)
    {
      gdouble value;

      if (channel == GIMP_HISTOGRAM_RGB)
        {
          value = (HISTOGRAM_VALUE (GIMP_HISTOGRAM_RED,   i) +
                   HISTOGRAM_VALUE (GIMP_HISTOGRAM_GREEN, i) +
                   HISTOGRAM_VALUE (GIMP_HISTOGRAM_BLUE,  i));
        }
      else
        {
          value = gimp_histogram_get_value (histogram, channel, i);
        }

      dev += value * SQR (((gdouble) i / (gdouble)  (priv->n_bins - 1)) - mean);
    }

  return sqrt (dev / count);
}


/*  private functions  */

static void
gimp_histogram_alloc_values (GimpHistogram *histogram,
                             gint           n_components,
                             gint           n_bins)
{
  GimpHistogramPrivate *priv = histogram->priv;

  if (n_components + 1 != priv->n_channels ||
      n_bins           != priv->n_bins)
    {
      gimp_histogram_clear_values (histogram);

      if (n_components + 1 != priv->n_channels)
        {
          priv->n_channels = n_components + 1;

          g_object_notify (G_OBJECT (histogram), "n-channels");
        }

      if (n_bins != priv->n_bins)
        {
          priv->n_bins = n_bins;

          g_object_notify (G_OBJECT (histogram), "n-bins");
        }

      priv->values = g_new0 (gdouble, priv->n_channels * priv->n_bins);
    }
  else
    {
      memset (priv->values, 0,
              priv->n_channels * priv->n_bins * sizeof (gdouble));
    }
}
