/* pnl-dock-edges.c
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

#define G_LOG_DOMAIN "pnl-dock-edges"

#include "pnl-bin.h"
#include "pnl-dock-bar.h"
#include "pnl-dock-item.h"
#include "pnl-dock-edges.h"

typedef struct
{
  PnlBin     *center;
  PnlDockBar *left;
  PnlDockBar *right;
  PnlDockBar *bottom;
  PnlDockBar *top;
} PnlDockEdgesPrivate;

static void pnl_dock_edges_init_dock_item_iface (PnlDockItemInterface *iface);

G_DEFINE_TYPE_WITH_CODE (PnlDockEdges, pnl_dock_edges, GTK_TYPE_GRID,
                         G_ADD_PRIVATE (PnlDockEdges)
                         G_IMPLEMENT_INTERFACE (PNL_TYPE_DOCK_ITEM, pnl_dock_edges_init_dock_item_iface)
                         G_IMPLEMENT_INTERFACE (PNL_TYPE_DOCK, NULL))

enum {
  PROP_0,
  N_PROPS,

  PROP_MANAGER
};

static void
pnl_dock_edges_add (GtkContainer *container,
                    GtkWidget    *child)
{
  PnlDockEdges *self = (PnlDockEdges *)container;
  PnlDockEdgesPrivate *priv = pnl_dock_edges_get_instance_private (self);

  g_assert (PNL_IS_DOCK_EDGES (self));
  g_assert (GTK_IS_WIDGET (child));

  gtk_container_add (GTK_CONTAINER (priv->center), child);
}

static void
pnl_dock_edges_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  PnlDockEdges *self = PNL_DOCK_EDGES (object);

  switch (prop_id)
    {
    case PROP_MANAGER:
      g_value_set_object (value, pnl_dock_item_get_manager (PNL_DOCK_ITEM (self)));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
pnl_dock_edges_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  PnlDockEdges *self = PNL_DOCK_EDGES (object);

  switch (prop_id)
    {
    case PROP_MANAGER:
      pnl_dock_item_set_manager (PNL_DOCK_ITEM (self), g_value_get_object (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
pnl_dock_edges_class_init (PnlDockEdgesClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkContainerClass *container_class = GTK_CONTAINER_CLASS (klass);

  object_class->get_property = pnl_dock_edges_get_property;
  object_class->set_property = pnl_dock_edges_set_property;

  container_class->add = pnl_dock_edges_add;

  g_object_class_override_property (object_class, PROP_MANAGER, "manager");
}

static void
pnl_dock_edges_init (PnlDockEdges *self)
{
  PnlDockEdgesPrivate *priv = pnl_dock_edges_get_instance_private (self);

  priv->top = g_object_new (PNL_TYPE_DOCK_BAR,
                            "visible", TRUE,
                            NULL);
  GTK_CONTAINER_CLASS (pnl_dock_edges_parent_class)->add (GTK_CONTAINER (self), GTK_WIDGET (priv->top));
  gtk_container_child_set (GTK_CONTAINER (self), GTK_WIDGET (priv->top),
                           "top-attach", 0,
                           "left-attach", 0,
                           "width", 3,
                           NULL);

  priv->left = g_object_new (PNL_TYPE_DOCK_BAR,
                             "visible", TRUE,
                             NULL);
  GTK_CONTAINER_CLASS (pnl_dock_edges_parent_class)->add (GTK_CONTAINER (self), GTK_WIDGET (priv->left));
  gtk_container_child_set (GTK_CONTAINER (self), GTK_WIDGET (priv->left),
                           "top-attach", 1,
                           "left-attach", 0,
                           NULL);

  priv->center = g_object_new (PNL_TYPE_BIN,
                               "expand", TRUE,
                               "visible", TRUE,
                               NULL);
  GTK_CONTAINER_CLASS (pnl_dock_edges_parent_class)->add (GTK_CONTAINER (self), GTK_WIDGET (priv->center));
  gtk_container_child_set (GTK_CONTAINER (self), GTK_WIDGET (priv->center),
                           "left-attach", 1,
                           "top-attach", 1,
                           NULL);

  priv->right = g_object_new (PNL_TYPE_DOCK_BAR,
                              "visible", TRUE,
                              NULL);
  GTK_CONTAINER_CLASS (pnl_dock_edges_parent_class)->add (GTK_CONTAINER (self), GTK_WIDGET (priv->right));
  gtk_container_child_set (GTK_CONTAINER (self), GTK_WIDGET (priv->right),
                           "top-attach", 1,
                           "left-attach", 2,
                           NULL);

  priv->bottom = g_object_new (PNL_TYPE_DOCK_BAR,
                               "visible", TRUE,
                               NULL);
  GTK_CONTAINER_CLASS (pnl_dock_edges_parent_class)->add (GTK_CONTAINER (self), GTK_WIDGET (priv->bottom));
  gtk_container_child_set (GTK_CONTAINER (self), GTK_WIDGET (priv->bottom),
                           "top-attach", 2,
                           "left-attach", 0,
                           "width", 3,
                           NULL);
}

static gboolean
pnl_dock_edges_minimize (PnlDockItem     *item,
                         PnlDockItem     *child,
                         GtkPositionType *position)
{
  PnlDockEdges *self = (PnlDockEdges *)item;
  PnlDockEdgesPrivate *priv = pnl_dock_edges_get_instance_private (self);
  GtkWidget *button;
  gchar *title;

  g_assert (PNL_IS_DOCK_EDGES (self));
  g_assert (PNL_IS_DOCK_ITEM (child));
  g_assert (position != NULL);

  title = pnl_dock_item_get_title (child);

  button = g_object_new (GTK_TYPE_BUTTON,
                         "child", g_object_new (GTK_TYPE_LABEL,
                                                "visible", TRUE,
                                                "label", title,
                                                "use-underline", TRUE,
                                                "angle", 90.0,
                                                NULL),
                         "visible", TRUE,
                         NULL);

  switch (*position)
    {
    case GTK_POS_LEFT:
      gtk_container_add (GTK_CONTAINER (priv->left), button);
      break;

    case GTK_POS_RIGHT:
      gtk_container_add (GTK_CONTAINER (priv->right), button);
      break;

    case GTK_POS_TOP:
      gtk_container_add (GTK_CONTAINER (priv->top), button);
      break;

    case GTK_POS_BOTTOM:
      gtk_container_add (GTK_CONTAINER (priv->bottom), button);
      break;

    default:
      g_assert_not_reached ();
    }

  g_free (title);

  return TRUE;
}

static void
pnl_dock_edges_init_dock_item_iface (PnlDockItemInterface *iface)
{
  iface->minimize = pnl_dock_edges_minimize;
}
