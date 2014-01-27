/*
 * Copyright (C) 2011-2014 Gary Kramlich <grim@reaperworld.com>
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

#include "gplugin-gjs-plugin.h"

#define GPLUGIN_GJS_PLUGIN_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GPLUGIN_TYPE_GJS_PLUGIN, GPluginGjsPluginPrivate))

/******************************************************************************
 * Typedefs
 *****************************************************************************/
typedef struct {
	gint dummy;
} GPluginGjsPluginPrivate;

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	PROP_LAST,
};

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;
static GType type_real = 0;

/******************************************************************************
 * Private Stuff
 *****************************************************************************/

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
gplugin_gjs_plugin_get_property(GObject *obj, guint param_id, GValue *value,
                                   GParamSpec *pspec)
{
	GPluginGjsPlugin *plugin = GPLUGIN_GJS_PLUGIN(obj);

	switch(param_id) {
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_gjs_plugin_set_property(GObject *obj, guint param_id,
                                   const GValue *value, GParamSpec *pspec)
{
	GPluginGjsPlugin *plugin = GPLUGIN_GJS_PLUGIN(obj);

	switch(param_id) {
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_gjs_plugin_finalize(GObject *obj) {
	GPluginGjsPluginPrivate *priv = GPLUGIN_GJS_PLUGIN_GET_PRIVATE(obj);

	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static void
gplugin_gjs_plugin_class_init(GPluginGjsPluginClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	g_type_class_add_private(klass, sizeof(GPluginGjsPluginPrivate));

	obj_class->get_property = gplugin_gjs_plugin_get_property;
	obj_class->set_property = gplugin_gjs_plugin_set_property;
	obj_class->finalize = gplugin_gjs_plugin_finalize;
}

/******************************************************************************
 * API
 *****************************************************************************/
void
gplugin_gjs_plugin_register(GPluginNativePlugin *plugin) {
	if(g_once_init_enter(&type_real)) {
		GType type = 0;

		static const GTypeInfo info = {
			.class_size = sizeof(GPluginGjsPluginClass),
			.class_init = (GClassInitFunc)gplugin_gjs_plugin_class_init,
			.instance_size = sizeof(GPluginGjsPlugin),
		};

		type = gplugin_native_plugin_register_type(plugin,
		                                           GPLUGIN_TYPE_PLUGIN,
		                                           "GPluginGjsPlugin",
		                                           &info,
		                                           0);

		g_once_init_leave(&type_real, type);
	}
}

GType
gplugin_gjs_plugin_get_type(void) {
	if(G_UNLIKELY(type_real == 0)) {
		g_warning("gplugin_gjs_plugin_get_type was called before "
		          "the type was registered!\n");
	}

	return type_real;
}

