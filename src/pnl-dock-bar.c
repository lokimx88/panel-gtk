/* pnl-dock-bar.c
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

#define G_LOG_DOMAIN "pnl-dock-bar"

#include "pnl-dock-bar.h"
#include "pnl-util-private.h"

typedef struct
{
  GtkBox *box;

  GtkPositionType position : 2;
} PnlDockBarPrivate;

typedef struct
{
  GtkPositionType position;
  gdouble         angle;
} PositionInfo;

G_DEFINE_TYPE_WITH_CODE (PnlDockBar, pnl_dock_bar, PNL_TYPE_BIN,
                         G_ADD_PRIVATE (PnlDockBar)
                         G_IMPLEMENT_INTERFACE (PNL_TYPE_DOCK_ITEM, NULL))

enum {
  PROP_0,
  PROP_POSITION,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

static void
pnl_dock_bar_update_layout_cb (GtkWidget    *widget,
                               PositionInfo *info)
{
  g_assert (GTK_IS_WIDGET (widget));
  g_assert (info != NULL);

}

static void
pnl_dock_bar_update_layout (PnlDockBar *self)
{
  PnlDockBarPrivate *priv = pnl_dock_bar_get_instance_private (self);
  PositionInfo info = { 0 };
  GtkStyleContext *style_context;
  const gchar *class_name;
  GtkOrientation orientation;

  g_assert (PNL_IS_DOCK_BAR (self));

  info.position = priv->position;

  switch (priv->position)
    {
    case GTK_POS_LEFT:
      orientation = GTK_ORIENTATION_VERTICAL;
      info.angle = 90.0;
      class_name = "left-edge";
      break;

    case GTK_POS_RIGHT:
      orientation = GTK_ORIENTATION_VERTICAL;
      info.angle = 270.0;
      class_name = "right-edge";
      break;

    case GTK_POS_BOTTOM:
      class_name = "bottom-edge";
      orientation = GTK_ORIENTATION_HORIZONTAL;
      info.angle = 0.0;
      break;

    case GTK_POS_TOP:
      class_name = "top-edge";
      orientation = GTK_ORIENTATION_HORIZONTAL;
      info.angle = 0.0;
      break;

    default:
      g_assert_not_reached ();
    }

  style_context = gtk_widget_get_style_context (GTK_WIDGET (self));
  gtk_style_context_remove_class (style_context, "left-edge");
  gtk_style_context_remove_class (style_context, "right-edge");
  gtk_style_context_remove_class (style_context, "bottom-edge");
  gtk_style_context_remove_class (style_context, "top-edge");
  gtk_style_context_add_class (style_context, class_name);

  gtk_orientable_set_orientation (GTK_ORIENTABLE (priv->box), orientation);
  gtk_container_foreach (GTK_CONTAINER (priv->box),
                         (GtkCallback)pnl_dock_bar_update_layout_cb,
                         &info);
}

static void
pnl_dock_bar_add (GtkContainer *container,
                  GtkWidget    *child)
{
  PnlDockBar *self = (PnlDockBar *)container;
  PnlDockBarPrivate *priv = pnl_dock_bar_get_instance_private (self);

  g_assert (PNL_IS_DOCK_BAR (self));
  g_assert (GTK_IS_WIDGET (child));

  gtk_container_add (GTK_CONTAINER (priv->box), child);

  pnl_dock_bar_update_layout (self);
}

static void
pnl_dock_bar_get_property (GObject    *object,
                           guint       prop_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
  PnlDockBar *self = PNL_DOCK_BAR (object);

  switch (prop_id)
    {
    case PROP_POSITION:
      g_value_set_enum (value, pnl_dock_bar_get_position (self));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
pnl_dock_bar_set_property (GObject      *object,
                           guint         prop_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
  PnlDockBar *self = PNL_DOCK_BAR (object);

  switch (prop_id)
    {
    case PROP_POSITION:
      pnl_dock_bar_set_position (self, g_value_get_enum (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
pnl_dock_bar_class_init (PnlDockBarClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  GtkContainerClass *container_class = GTK_CONTAINER_CLASS (klass);

  object_class->get_property = pnl_dock_bar_get_property;
  object_class->set_property = pnl_dock_bar_set_property;

  container_class->add = pnl_dock_bar_add;

  properties [PROP_POSITION] =
    g_param_spec_enum ("position",
                       "Position",
                       "Position",
                       GTK_TYPE_POSITION_TYPE,
                       GTK_POS_LEFT,
                       (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);

  gtk_widget_class_set_css_name (widget_class, "dockbar");
}

static void
pnl_dock_bar_init (PnlDockBar *self)
{
  PnlDockBarPrivate *priv = pnl_dock_bar_get_instance_private (self);

  priv->box = g_object_new (GTK_TYPE_BOX,
                            "orientation", GTK_ORIENTATION_HORIZONTAL,
                            "spacing", 0,
                            "visible", TRUE,
                            NULL);
  GTK_CONTAINER_CLASS (pnl_dock_bar_parent_class)->add (GTK_CONTAINER (self), GTK_WIDGET (priv->box));

  pnl_dock_bar_update_layout (self);
}

GtkPositionType
pnl_dock_bar_get_position (PnlDockBar *self)
{
  PnlDockBarPrivate *priv = pnl_dock_bar_get_instance_private (self);

  g_return_val_if_fail (PNL_IS_DOCK_BAR (self), 0);

  return priv->position;
}

void
pnl_dock_bar_set_position (PnlDockBar      *self,
                           GtkPositionType  position)
{
  PnlDockBarPrivate *priv = pnl_dock_bar_get_instance_private (self);

  g_return_if_fail (PNL_IS_DOCK_BAR (self));
  g_return_if_fail (position == GTK_POS_LEFT ||
                    position == GTK_POS_RIGHT ||
                    position == GTK_POS_TOP ||
                    position == GTK_POS_BOTTOM);

  if (position != priv->position)
    {
      priv->position = position;
      pnl_dock_bar_update_layout (self);
      g_object_notify_by_pspec (G_OBJECT (self), properties [PROP_POSITION]);
    }
}

GtkWidget *
pnl_dock_bar_new (void)
{
  return g_object_new (PNL_TYPE_DOCK_BAR, NULL);
}
