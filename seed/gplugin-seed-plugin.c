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

#include "gplugin-seed-plugin.h"

#include <seed.h>

#define GPLUGIN_SEED_PLUGIN_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GPLUGIN_TYPE_SEED_PLUGIN, GPluginSeedPluginPrivate))

/******************************************************************************
 * Typedefs
 *****************************************************************************/
typedef struct {
	SeedContext ctx;
	SeedObject load;
	SeedObject unload;
} GPluginSeedPluginPrivate;

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	PROP_CONTEXT,
	PROP_LOAD,
	PROP_UNLOAD,
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
static void
gplugin_seed_plugin_set_context(GPluginSeedPlugin *plugin, SeedContext ctx) {
	GPluginSeedPluginPrivate *priv = GPLUGIN_SEED_PLUGIN_GET_PRIVATE(plugin);

	priv->ctx = ctx;
}

static void
gplugin_seed_plugin_set_load(GPluginSeedPlugin *plugin, SeedObject load) {
	GPluginSeedPluginPrivate *priv = GPLUGIN_SEED_PLUGIN_GET_PRIVATE(plugin);

	priv->load = load;
}

static void
gplugin_seed_plugin_set_unload(GPluginSeedPlugin *plugin, SeedObject unload) {
	GPluginSeedPluginPrivate *priv = GPLUGIN_SEED_PLUGIN_GET_PRIVATE(plugin);

	priv->unload = unload;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
gplugin_seed_plugin_get_property(GObject *obj, guint param_id, GValue *value,
                                   GParamSpec *pspec)
{
	GPluginSeedPlugin *plugin = GPLUGIN_SEED_PLUGIN(obj);

	switch(param_id) {
		case PROP_CONTEXT:
			g_value_set_pointer(value,
			                    gplugin_seed_plugin_get_context(plugin));
			break;
		case PROP_LOAD:
			g_value_set_pointer(value,
			                    gplugin_seed_plugin_get_load_func(plugin));
			break;
		case PROP_UNLOAD:
			g_value_set_pointer(value,
			                    gplugin_seed_plugin_get_unload_func(plugin));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_seed_plugin_set_property(GObject *obj, guint param_id,
                                   const GValue *value, GParamSpec *pspec)
{
	GPluginSeedPlugin *plugin = GPLUGIN_SEED_PLUGIN(obj);

	switch(param_id) {
		case PROP_CONTEXT:
			gplugin_seed_plugin_set_context(plugin,
			                                g_value_get_pointer(value));
			break;
		case PROP_LOAD:
			gplugin_seed_plugin_set_load(plugin,
			                             g_value_get_pointer(value));
			break;
		case PROP_UNLOAD:
			gplugin_seed_plugin_set_unload(plugin,
			                               g_value_get_pointer(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_seed_plugin_finalize(GObject *obj) {
	GPluginSeedPluginPrivate *priv = GPLUGIN_SEED_PLUGIN_GET_PRIVATE(obj);

	if(priv->ctx)
		seed_context_unref(priv->ctx);

	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static void
gplugin_seed_plugin_class_init(GPluginSeedPluginClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	g_type_class_add_private(klass, sizeof(GPluginSeedPluginPrivate));

	obj_class->get_property = gplugin_seed_plugin_get_property;
	obj_class->set_property = gplugin_seed_plugin_set_property;
	obj_class->finalize = gplugin_seed_plugin_finalize;

	g_object_class_install_property(obj_class, PROP_CONTEXT,
		g_param_spec_pointer("context", "context",
		                     "The seed context for the plugin",
		                     G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_LOAD,
		g_param_spec_pointer("load", "load",
		                     "The load function for the plugin",
		                     G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_UNLOAD,
		g_param_spec_pointer("unload", "unload",
		                     "The unload function for the plugin",
		                     G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

/******************************************************************************
 * API
 *****************************************************************************/
void
gplugin_seed_plugin_register(GPluginNativePlugin *plugin) {
	if(g_once_init_enter(&type_real)) {
		GType type = 0;

		static const GTypeInfo info = {
			.class_size = sizeof(GPluginSeedPluginClass),
			.class_init = (GClassInitFunc)gplugin_seed_plugin_class_init,
			.instance_size = sizeof(GPluginSeedPlugin),
		};

		type = gplugin_native_plugin_register_type(plugin,
		                                           GPLUGIN_TYPE_PLUGIN,
		                                           "GPluginSeedPlugin",
		                                           &info,
		                                           0);

		g_once_init_leave(&type_real, type);
	}
}

GType
gplugin_seed_plugin_get_type(void) {
	if(G_UNLIKELY(type_real == 0)) {
		g_warning("gplugin_seed_plugin_get_type was called before "
		          "the type was registered!\n");
	}

	return type_real;
}

SeedContext
gplugin_seed_plugin_get_context(const GPluginSeedPlugin *plugin) {
	GPluginSeedPluginPrivate *priv = GPLUGIN_SEED_PLUGIN_GET_PRIVATE(plugin);

	return priv->ctx;
}

SeedObject
gplugin_seed_plugin_get_load_func(const GPluginSeedPlugin *plugin) {
	GPluginSeedPluginPrivate *priv = GPLUGIN_SEED_PLUGIN_GET_PRIVATE(plugin);

	return priv->load;
}

SeedObject
gplugin_seed_plugin_get_unload_func(const GPluginSeedPlugin *plugin) {
	GPluginSeedPluginPrivate *priv = GPLUGIN_SEED_PLUGIN_GET_PRIVATE(plugin);

	return priv->unload;
}

