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
#include "pnl-util-private.h"

G_DEFINE_TYPE (PnlBin, pnl_bin, GTK_TYPE_BIN)

static gboolean
pnl_bin_draw (GtkWidget *widget,
              cairo_t   *cr)
{
  GtkStyleContext *style_context;
  GtkWidget *child;
  GtkAllocation alloc;
  GtkStateFlags state;
  GtkBorder margin;

  g_assert (PNL_IS_BIN (widget));

  gtk_widget_get_allocation (widget, &alloc);
  alloc.x = 0;
  alloc.y = 0;

  style_context = gtk_widget_get_style_context (widget);
  state = gtk_widget_get_state_flags (widget);

  gtk_style_context_get_margin (style_context, state, &margin);
  pnl_gtk_allocation_subtract_border (&alloc, &margin);

  gtk_render_background (style_context, cr, alloc.x, alloc.y, alloc.width, alloc.height);

  child = gtk_bin_get_child (GTK_BIN (widget));
  if (child != NULL)
    gtk_container_propagate_draw (GTK_CONTAINER (widget), child, cr);

  gtk_render_frame (style_context, cr, alloc.x, alloc.y, alloc.width, alloc.height);

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
pnl_bin_size_allocate (GtkWidget     *widget,
                       GtkAllocation *alloc)
{
  PnlBin *self = (PnlBin *)widget;
  GtkAllocation child_alloc = { 0 };
  GtkWidget *child;

  g_assert (PNL_IS_BIN (self));
  g_assert (alloc != NULL);

  child = gtk_bin_get_child (GTK_BIN (self));

  if (child != NULL)
    {
      GtkStyleContext *style_context;
      GtkBorder borders;

      style_context = gtk_widget_get_style_context (widget);
      child_alloc = *alloc;

      if (gtk_widget_get_has_window (widget))
        {
          child_alloc.x = 0;
          child_alloc.y = 0;
        }

      pnl_gtk_style_context_get_borders (style_context, &borders);
      pnl_gtk_allocation_subtract_border (&child_alloc, &borders);
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
  GtkStyleContext *style_context;
  GtkWidget *child;
  GtkBorder borders;

  g_assert (PNL_IS_BIN (widget));

  *min_width = 0;
  *nat_width = 0;

  child = gtk_bin_get_child (GTK_BIN (self));
  if (child != NULL)
    gtk_widget_get_preferred_width (child, min_width, nat_width);

  style_context = gtk_widget_get_style_context (widget);
  pnl_gtk_style_context_get_borders (style_context, &borders);

  *min_width += (borders.left + borders.right);
  *nat_width += (borders.left + borders.right);
}

static void
pnl_bin_get_preferred_height (GtkWidget *widget,
                              gint      *min_height,
                              gint      *nat_height)
{
  PnlBin *self = (PnlBin *)widget;
  GtkStyleContext *style_context;
  GtkWidget *child;
  GtkBorder borders;

  g_assert (PNL_IS_BIN (widget));

  *min_height = 0;
  *nat_height = 0;

  child = gtk_bin_get_child (GTK_BIN (self));
  if (child != NULL)
    gtk_widget_get_preferred_height (child, min_height, nat_height);

  style_context = gtk_widget_get_style_context (widget);
  pnl_gtk_style_context_get_borders (style_context, &borders);

  *min_height += (borders.top + borders.bottom);
  *nat_height += (borders.top + borders.bottom);
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
