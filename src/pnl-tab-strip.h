/* pnl-tab-strip.h
 *
 * Copyright (C) 2016 Christian Hergert <chergert@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#if !defined(PNL_INSIDE) && !defined(PNL_COMPILATION)
# error "Only <pnl.h> can be included directly."
#endif

#ifndef PNL_TAB_STRIP_H
#define PNL_TAB_STRIP_H

#include "pnl-dock-types.h"

G_BEGIN_DECLS

struct _PnlTabStripClass
{
  GtkBoxClass parent;

  gpointer _reserved1;
  gpointer _reserved2;
  gpointer _reserved3;
  gpointer _reserved4;
  gpointer _reserved5;
  gpointer _reserved6;
  gpointer _reserved7;
  gpointer _reserved8;
};

GtkWidget       *pnl_tab_strip_new             (void);
GtkStack        *pnl_tab_strip_get_stack       (PnlTabStrip     *self);
void             pnl_tab_strip_set_stack       (PnlTabStrip     *self,
                                                GtkStack        *stack);
GtkPositionType  pnl_tab_strip_get_edge        (PnlTabStrip     *self);
void             pnl_tab_strip_set_edge        (PnlTabStrip     *self,
                                                GtkPositionType  edge);
gboolean         pnl_tab_strip_get_show_labels (PnlTabStrip     *self);
void             pnl_tab_strip_set_show_labels (PnlTabStrip     *self,
                                                gboolean         show_labels);
PnlTabStyle      pnl_tab_strip_get_style       (PnlTabStrip     *self);
void             pnl_tab_strip_set_style       (PnlTabStrip     *self,
                                                PnlTabStyle      style);
void             pnl_tab_strip_add_control     (PnlTabStrip     *self,
                                                GtkWidget       *widget);

G_END_DECLS

#endif /* PNL_TAB_STRIP_H */
