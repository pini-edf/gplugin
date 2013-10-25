/*
 * Copyright (C) 2011-2013 Gary Kramlich <grim@reaperworld.com>
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

#include <gplugin-gtk/gplugin-gtk-plugin-info.h>

#include <gplugin-gtk/gplugin-gtk-private.h>

#include <gplugin/gplugin.h>

#define GPLUGIN_GTK_PLUGIN_INFO_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GPLUGIN_GTK_TYPE_PLUGIN_INFO, GPluginGtkPluginInfoPrivate))

/******************************************************************************
 * Structs
 *****************************************************************************/
typedef struct {
	GPluginPluginInfo *info;
} GPluginGtkPluginInfoPrivate;

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	N_PROPERTIES,
};

/******************************************************************************
 * Globals
 *****************************************************************************/

/******************************************************************************
 * GObject Stuff
 *****************************************************************************/
G_DEFINE_TYPE(GPluginGtkPluginInfo, gplugin_gtk_plugin_info, GTK_TYPE_GRID);

static void
gplugin_gtk_plugin_info_set_property(GObject *obj, guint prop_id, const GValue *value,
                              GParamSpec *pspec)
{
	GPluginGtkPluginInfo *plugin_info = GPLUGIN_GTK_PLUGIN_INFO(obj);

	switch(prop_id) {
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void
gplugin_gtk_plugin_info_get_property(GObject *obj, guint prop_id, GValue *value,
                              GParamSpec *pspec)
{
	GPluginGtkPluginInfo *plugin_info = GPLUGIN_GTK_PLUGIN_INFO(obj);

	switch(prop_id) {
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void
gplugin_gtk_plugin_info_constructed(GObject *obj) {
	G_OBJECT_CLASS(gplugin_gtk_plugin_info_parent_class)->constructed(obj);
}

static void
gplugin_gtk_plugin_info_init(GPluginGtkPluginInfo *info) {
}

static void
gplugin_gtk_plugin_info_class_init(GPluginGtkPluginInfoClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	g_type_class_add_private(obj_class, sizeof(GPluginGtkPluginInfoPrivate));

	obj_class->get_property = gplugin_gtk_plugin_info_get_property;
	obj_class->set_property = gplugin_gtk_plugin_info_set_property;

	/* properties */
}

/******************************************************************************
 * API
 *****************************************************************************/
GtkWidget *
gplugin_gtk_plugin_info_new(void) {
	GObject *ret = NULL;

	ret = g_object_new(GPLUGIN_GTK_TYPE_PLUGIN_INFO,
	                   NULL);

	return GTK_WIDGET(ret);
}

