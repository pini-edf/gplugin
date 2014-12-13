/*
 * Copyright (C) 2011-2014 Gary Kramlich <grim@reaperworld.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "gplugin-gjs-plugin.h"

#define GPLUGIN_GJS_PLUGIN_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GPLUGIN_TYPE_GJS_PLUGIN, GPluginGjsPluginPrivate))

/******************************************************************************
 * Typedefs
 *****************************************************************************/
typedef struct {
	GjsContext *context;
	JSObject *global;
	JSContext *js_context;
} GPluginGjsPluginPrivate;

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	PROP_CONTEXT,
	PROP_GLOBAL,
	PROP_JS_CONTEXT,
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
gplugin_gjs_plugin_set_context(GPluginGjsPlugin *plugin, GjsContext *context) {
	GPluginGjsPluginPrivate *priv = GPLUGIN_GJS_PLUGIN_GET_PRIVATE(plugin);

	if(priv->context)
		g_object_unref(G_OBJECT(priv->context));

	priv->context = GJS_IS_CONTEXT(context) ? g_object_ref(G_OBJECT(context)) :
	                                          NULL;
}

static void
gplugin_gjs_plugin_set_global_scope(GPluginGjsPlugin *plugin, JSObject *global) {
	GPluginGjsPluginPrivate *priv = GPLUGIN_GJS_PLUGIN_GET_PRIVATE(plugin);

	priv->global = global;
}

static void
gplugin_gjs_plugin_set_js_context(GPluginGjsPlugin *plugin, JSContext *jsctx) {
	GPluginGjsPluginPrivate *priv = GPLUGIN_GJS_PLUGIN_GET_PRIVATE(plugin);

	priv->js_context = jsctx;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
gplugin_gjs_plugin_get_property(GObject *obj, guint param_id, GValue *value,
                                GParamSpec *pspec)
{
	GPluginGjsPlugin *plugin = GPLUGIN_GJS_PLUGIN(obj);

	switch(param_id) {
		case PROP_CONTEXT:
			g_value_set_object(value, gplugin_gjs_plugin_get_context(plugin));
			break;
		case PROP_GLOBAL:
			g_value_set_pointer(value,
			                    gplugin_gjs_plugin_get_global_scope(plugin));
			break;
		case PROP_JS_CONTEXT:
			g_value_set_pointer(value,
			                    gplugin_gjs_plugin_get_js_context(plugin));
			break;
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
		case PROP_CONTEXT:
			gplugin_gjs_plugin_set_context(plugin, g_value_get_object(value));
			break;
		case PROP_GLOBAL:
			gplugin_gjs_plugin_set_global_scope(plugin,
			                                    g_value_get_pointer(value));
			break;
		case PROP_JS_CONTEXT:
			gplugin_gjs_plugin_set_js_context(plugin,
			                                  g_value_get_pointer(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_gjs_plugin_finalize(GObject *obj) {
	GPluginGjsPluginPrivate *priv = GPLUGIN_GJS_PLUGIN_GET_PRIVATE(obj);

	if(priv->context)
		g_object_unref(G_OBJECT(priv->context));

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

	g_object_class_install_property(obj_class, PROP_CONTEXT,
		g_param_spec_object("context", "context",
		                    "The Gjs Context for this plugin",
		                    GJS_TYPE_CONTEXT,
		                    G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
		                    G_PARAM_STATIC_STRINGS));

	g_object_class_install_property(obj_class, PROP_GLOBAL,
		g_param_spec_pointer("global", "global",
		                     "The global scope for this plugin",
		                     G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
		                     G_PARAM_STATIC_STRINGS));

	g_object_class_install_property(obj_class, PROP_JS_CONTEXT,
		g_param_spec_pointer("js-context", "js-context",
		                     "The JSContext function for this plugin",
		                     G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
		                     G_PARAM_STATIC_STRINGS));
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

GjsContext *
gplugin_gjs_plugin_get_context(const GPluginGjsPlugin *plugin) {
	GPluginGjsPluginPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_GJS_PLUGIN(plugin), NULL);

	priv = GPLUGIN_GJS_PLUGIN_GET_PRIVATE(plugin);

	return priv->context;
}

JSObject *
gplugin_gjs_plugin_get_global_scope(const GPluginGjsPlugin *plugin) {
	GPluginGjsPluginPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_GJS_PLUGIN(plugin), NULL);

	priv = GPLUGIN_GJS_PLUGIN_GET_PRIVATE(plugin);

	return priv->global;
}

JSContext *
gplugin_gjs_plugin_get_js_context(const GPluginGjsPlugin *plugin) {
	GPluginGjsPluginPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_GJS_PLUGIN(plugin), NULL);

	priv = GPLUGIN_GJS_PLUGIN_GET_PRIVATE(plugin);

	return priv->js_context;
}

