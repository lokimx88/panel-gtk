/* pnl-dock-bar.h
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

#ifndef PNL_DOCK_BAR_H
#define PNL_DOCK_BAR_H

#include <gtk/gtk.h>

#include "pnl-dock-item.h"

G_BEGIN_DECLS

#define PNL_TYPE_DOCK_BAR (pnl_dock_bar_get_type())

G_DECLARE_DERIVABLE_TYPE (PnlDockBar, pnl_dock_bar, PNL, DOCK_BAR, GtkBin)

struct _PnlDockBarClass
{
  GtkBinClass parent_class;

  gpointer _reserved1;
  gpointer _reserved2;
  gpointer _reserved3;
  gpointer _reserved4;
};

GtkWidget       *pnl_dock_bar_new          (void);
GtkPositionType  pnl_dock_bar_get_position (PnlDockBar      *self);
void             pnl_dock_bar_set_position (PnlDockBar      *self,
                                            GtkPositionType  position);

G_END_DECLS

#endif /* PNL_DOCK_BAR_H */