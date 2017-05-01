/* pnl-dock-stack.h
 *
 * Copyright (C) 2016 Christian Hergert <chergert@redhat.com>
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

#if !defined(PNL_INSIDE) && !defined(PNL_COMPILATION)
# error "Only <pnl.h> can be included directly."
#endif

#ifndef PNL_DOCK_STACK_H
#define PNL_DOCK_STACK_H

#include "pnl-dock-types.h"

G_BEGIN_DECLS

struct _PnlDockStackClass
{
  GtkBoxClass parent;

  gpointer _reserved1;
  gpointer _reserved2;
  gpointer _reserved3;
  gpointer _reserved4;
};

GtkWidget       *pnl_dock_stack_new                    (void);
GtkPositionType  pnl_dock_stack_get_edge               (PnlDockStack    *self);
void             pnl_dock_stack_set_edge               (PnlDockStack    *self,
                                                        GtkPositionType  edge);
gboolean         pnl_dock_stack_get_show_pinned_button (PnlDockStack    *self);
void             pnl_dock_stack_set_show_pinned_button (PnlDockStack    *self,
                                                        gboolean         show_pinned_button);

G_END_DECLS

#endif /* PNL_DOCK_STACK_H */
