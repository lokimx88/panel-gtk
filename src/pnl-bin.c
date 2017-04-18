/* pnl-bin.c
 *
 * Copyright (C) 2017 Christian Hergert <chergert@redhat.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#define G_LOG_DOMAIN "pnl-bin"

/**
 * SECTION:pnl-bin
 * @title: PnlBin
 *
 * This is just a #GtkBin class that also allows for various styling with
 * CSS over what can be done in GtkBin directly.
 */

#include <string.h>

#include "pnl-bin.h"

G_DEFINE_TYPE (PnlBin, pnl_bin, GTK_TYPE_BIN)

static gboolean
pnl_bin_draw (GtkWidget *widget,
              cairo_t   *cr)
{
  GtkStyleContext *style_context;
  GtkAllocation alloc;
  GtkWidget *child;
  GtkStateFlags state;
  GtkBorder margin;

  style_context = gtk_widget_get_style_context (widget);
  state = gtk_widget_get_state_flags (widget);
  gtk_style_context_get_margin (style_context, state, &margin);

  child = gtk_bin_get_child (GTK_BIN (widget));

  gtk_widget_get_allocation (widget, &alloc);

  gtk_render_background (style_context, cr,
                         margin.left,
                         margin.top,
                         alloc.width - margin.left - margin.right,
                         alloc.height - margin.top - margin.bottom);

  if (child != NULL)
    gtk_container_propagate_draw (GTK_CONTAINER (widget), child, cr);

  return FALSE;
}

static inline void
add_border (GtkBorder       *lsv,
            const GtkBorder *rsv)
{
  lsv->left += rsv->left;
  lsv->right += rsv->right;
  lsv->top += rsv->top;
  lsv->bottom += rsv->bottom;
}

static void
pnl_bin_compute_borders (PnlBin    *self,
                         GtkBorder *all_borders)
{
  GtkStyleContext *style_contxt;
  GtkStateFlags flags;
  GtkBorder tmp;

  style_contxt = gtk_widget_get_style_context (GTK_WIDGET (self));
  flags = gtk_widget_get_state_flags (GTK_WIDGET (self));

  memset (all_borders, 0, sizeof *all_borders);

  gtk_style_context_get_margin (style_contxt, flags, &tmp);
  add_border (all_borders, &tmp);

  gtk_style_context_get_border (style_contxt, flags, &tmp);
  add_border (all_borders, &tmp);

  gtk_style_context_get_padding (style_contxt, flags, &tmp);
  add_border (all_borders, &tmp);
}

static void
pnl_bin_size_allocate (GtkWidget     *widget,
                       GtkAllocation *alloc)
{
  PnlBin *self = (PnlBin *)widget;
  GtkBorder all_borders;
  GtkAllocation child_alloc;
  GtkWidget *child;

  g_assert (PNL_IS_BIN (self));
  g_assert (alloc != NULL);

  child = gtk_bin_get_child (GTK_BIN (self));

  if (child != NULL)
    {
      pnl_bin_compute_borders (self, &all_borders);

      child_alloc = *alloc;

      child_alloc.x += all_borders.left;
      child_alloc.y += all_borders.top;
      child_alloc.width -= all_borders.left;
      child_alloc.width -= all_borders.right;
      child_alloc.height -= all_borders.top;
      child_alloc.height -= all_borders.bottom;
    }

  GTK_WIDGET_CLASS (pnl_bin_parent_class)->size_allocate (widget, alloc);

  if (child != NULL)
    gtk_widget_size_allocate (child, &child_alloc);
}

static void
pnl_bin_get_preferred_width (GtkWidget *widget,
                             gint      *min_width,
                             gint      *nat_width)
{
  PnlBin *self = (PnlBin *)widget;
  GtkWidget *child;
  GtkBorder all_borders;

  g_assert (PNL_IS_BIN (widget));

  *min_width = 0;
  *nat_width = 0;

  child = gtk_bin_get_child (GTK_BIN (self));
  if (child != NULL)
    gtk_widget_get_preferred_width (child, min_width, nat_width);

  pnl_bin_compute_borders (self, &all_borders);

  *min_width += all_borders.left;
  *min_width += all_borders.right;

  *nat_width += all_borders.left;
  *nat_width += all_borders.right;
}

static void
pnl_bin_get_preferred_height (GtkWidget *widget,
                              gint      *min_height,
                              gint      *nat_height)
{
  PnlBin *self = (PnlBin *)widget;
  GtkWidget *child;
  GtkBorder all_borders;

  g_assert (PNL_IS_BIN (widget));

  *min_height = 0;
  *nat_height = 0;

  child = gtk_bin_get_child (GTK_BIN (self));
  if (child != NULL)
    gtk_widget_get_preferred_height (child, min_height, nat_height);

  pnl_bin_compute_borders (self, &all_borders);

  *min_height += all_borders.top;
  *min_height += all_borders.bottom;

  *nat_height += all_borders.top;
  *nat_height += all_borders.bottom;
}

static void
pnl_bin_class_init (PnlBinClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  widget_class->draw = pnl_bin_draw;
  widget_class->get_preferred_width = pnl_bin_get_preferred_width;
  widget_class->get_preferred_height = pnl_bin_get_preferred_height;
  widget_class->size_allocate = pnl_bin_size_allocate;

  gtk_widget_class_set_css_name (widget_class, "pnlbin");
}

static void
pnl_bin_init (PnlBin *self)
{
}

GtkWidget *
pnl_bin_new (void)
{
  return g_object_new (PNL_TYPE_BIN, NULL);
}
