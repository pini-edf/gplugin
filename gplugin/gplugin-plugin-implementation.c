/*
 * Copyright (C) 2013 Ankit Vani <a@nevitus.org>
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

#include <gplugin/gplugin-plugin-implementation.h>

#define GPLUGIN_PLUGIN_IMPLEMENTATION_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GPLUGIN_TYPE_PLUGIN_IMPLEMENTATION, GPluginPluginImplementationPrivate))

/******************************************************************************
 * Structs
 *****************************************************************************/
typedef struct {
	GPluginPlugin *plugin;
} GPluginPluginImplementationPrivate;

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	PROP_PLUGIN,
	PROP_LAST,
};

/******************************************************************************
 * Private API
 *****************************************************************************/
static void
gplugin_plugin_implementation_set_plugin(GPluginPluginImplementation *implementation,
                                         GPluginPlugin *plugin)
{
	GPluginPluginImplementationPrivate *priv =
		GPLUGIN_PLUGIN_IMPLEMENTATION_GET_PRIVATE(implementation);

	priv->plugin = plugin;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
gplugin_plugin_implementation_get_property(GObject *obj, guint param_id,
                                           GValue *value, GParamSpec *pspec)
{
	GPluginPluginImplementation *implementation =
		GPLUGIN_PLUGIN_IMPLEMENTATION(obj);

	switch(param_id) {
		case PROP_PLUGIN:
			g_value_set_object(value,
				gplugin_plugin_implementation_get_plugin(implementation));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_plugin_implementation_set_property(GObject *obj, guint param_id,
                                           const GValue *value,
                                           GParamSpec *pspec)
{
	GPluginPluginImplementation *implementation =
		GPLUGIN_PLUGIN_IMPLEMENTATION(obj);

	switch(param_id) {
		case PROP_PLUGIN:
			gplugin_plugin_implementation_set_plugin(implementation,
				g_value_get_object(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_plugin_implementation_class_init(GPluginPluginImplementationClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	g_type_class_add_private(klass, sizeof(GPluginPluginImplementationPrivate));

	obj_class->get_property = gplugin_plugin_implementation_get_property;
	obj_class->set_property = gplugin_plugin_implementation_set_property;

	g_object_class_install_property(obj_class, PROP_PLUGIN,
		g_param_spec_object("gplugin-plugin", "gplugin-plugin",
		                    "The GPluginPlugin of this implementation",
		                    GPLUGIN_TYPE_PLUGIN,
		                    G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

/******************************************************************************
 * GPluginPluginImplementation API
 *****************************************************************************/
GType
gplugin_plugin_implementation_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		static const GTypeInfo info = {
			.class_size = sizeof(GPluginPluginImplementationClass),
			.class_init = (GClassInitFunc)gplugin_plugin_implementation_class_init,
			.instance_size = sizeof(GPluginPluginImplementation),
		};

		type = g_type_register_static(G_TYPE_OBJECT,
		                              "GPluginPluginImplementation",
		                              &info, G_TYPE_FLAG_ABSTRACT);
	}

	return type;
}

/**
 * gplugin_plugin_implementation_get_plugin:
 * @implementation: #GPluginPluginImplementation instance
 *
 * Return value: (transfer none): The #GPluginPlugin instance of @implementation
 */
GPluginPlugin *
gplugin_plugin_implementation_get_plugin(GPluginPluginImplementation *implementation) {
	GPluginPluginImplementationPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_IMPLEMENTATION(implementation), NULL);

	priv = GPLUGIN_PLUGIN_IMPLEMENTATION_GET_PRIVATE(implementation);

	return priv->plugin;
}

