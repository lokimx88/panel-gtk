/* pnl-dock.h
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

#ifndef PNL_DOCK_H
#define PNL_DOCK_H

#include "pnl-dock-types.h"

G_BEGIN_DECLS

struct _PnlDockInterface
{
  GTypeInterface parent;

  PnlDockManager *(*get_manager) (PnlDock        *self);
  void            (*set_manager) (PnlDock        *self,
                                  PnlDockManager *manager);
};

PnlDockManager *pnl_dock_get_manager (PnlDock        *self);
void            pnl_dock_set_manager (PnlDock        *self,
                                      PnlDockManager *manager);

G_END_DECLS

#endif /* PNL_DOCK_H */