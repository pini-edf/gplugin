/*
 * Copyright (C) 2011 Gary Kramlich <grim@reaperworld.com>
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

#include <gidbits/gidbits-plugin.h>

#define GIDBITS_PLUGIN_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GIDBITS_TYPE_PLUGIN, GidbitsPluginPrivate))

/******************************************************************************
 * Structs
 *****************************************************************************/
typedef struct {
	gchar *filename;

	GidbitsPluginInfo *info;
} GidbitsPluginPrivate;

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	PROP_FILENAME,
	PROP_INFO,
	PROP_LAST,
};

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;

/******************************************************************************
 * Private API
 *****************************************************************************/
static void
gidbits_plugin_set_filename(GidbitsPlugin *plugin, const gchar *filename) {
	GidbitsPluginPrivate *priv = GIDBITS_PLUGIN_GET_PRIVATE(plugin);

	g_free(priv->filename);

	priv->filename = (filename) ? g_strdup(filename) : NULL;

	g_object_notify(G_OBJECT(plugin), "filename");
}

static void
gidbits_plugin_set_info(GidbitsPlugin *plugin, GidbitsPluginInfo *info) {
	GidbitsPluginPrivate *priv = GIDBITS_PLUGIN_GET_PRIVATE(plugin);

	if(priv->info)
		gidbits_plugin_info_free(priv->info);

	if(info)
		priv->info = gidbits_plugin_info_copy(info);
	else
		priv->info = NULL;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
gidbits_plugin_get_property(GObject *obj, guint param_id, GValue *value,
                            GParamSpec *pspec)
{
	GidbitsPlugin *plugin = GIDBITS_PLUGIN(obj);

	switch(param_id) {
		case PROP_FILENAME:
			g_value_set_string(value, gidbits_plugin_get_filename(plugin));
			break;
		case PROP_INFO:
			g_value_set_boxed(value, gidbits_plugin_get_info(plugin));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gidbits_plugin_set_property(GObject *obj, guint param_id, const GValue *value,
                            GParamSpec *pspec)
{
	GidbitsPlugin *plugin = GIDBITS_PLUGIN(obj);

	switch(param_id) {
		case PROP_FILENAME:
			gidbits_plugin_set_filename(plugin, g_value_get_string(value));
			break;
		case PROP_INFO:
			gidbits_plugin_set_info(plugin, g_value_get_boxed(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gidbits_plugin_class_init(GidbitsPluginClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	g_type_class_add_private(klass, sizeof(GidbitsPluginPrivate));

	obj_class->get_property = gidbits_plugin_get_property;
	obj_class->set_property = gidbits_plugin_set_property;

	g_object_class_install_property(obj_class, PROP_FILENAME,
		g_param_spec_string("filename", "filename",
		                    "The filename of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

/******************************************************************************
 * GidbitsPlugin API
 *****************************************************************************/
GType
gidbits_plugin_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		static const GTypeInfo info = {
			.class_size = sizeof(GidbitsPluginClass),
			.class_init = (GClassInitFunc)gidbits_plugin_class_init,
			.instance_size = sizeof(GidbitsPlugin),
		};

		type = g_type_register_static(G_TYPE_OBJECT,
		                              "GidbitsPlugin",
		                              &info, G_TYPE_FLAG_ABSTRACT);
	}

	return type;
}

const gchar *
gidbits_plugin_get_filename(const GidbitsPlugin *plugin) {
	GidbitsPluginPrivate *priv = NULL;

	g_return_val_if_fail(GIDBITS_IS_PLUGIN(plugin), NULL);

	priv = GIDBITS_PLUGIN_GET_PRIVATE(plugin);

	return priv->filename;
}

GidbitsPluginInfo *
gidbits_plugin_get_info(const GidbitsPlugin *plugin) {
	GidbitsPluginPrivate *priv = NULL;

	g_return_val_if_fail(GIDBITS_IS_PLUGIN(plugin), NULL);

	priv = GIDBITS_PLUGIN_GET_PRIVATE(plugin);

	return priv->info;
}

/******************************************************************************
 * GidbitsPluginInfo API
 *****************************************************************************/
GType
gidbits_plugin_info_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		type = g_boxed_type_register_static("GidbitsPluginInfo",
		                                    (GBoxedCopyFunc)gidbits_plugin_info_copy,
		                                    (GBoxedFreeFunc)gidbits_plugin_info_free);
	}

	return type;
}

void
gidbits_plugin_info_free(GidbitsPluginInfo *info) {
	g_return_if_fail(info);

	g_free(info->name);
	g_free(info->version);
	g_free(info->summary);
	g_free(info->description);
	g_free(info->author);
	g_free(info->website);

	g_slice_free(GidbitsPluginInfo, info);
}

GidbitsPluginInfo *
gidbits_plugin_info_copy(const GidbitsPluginInfo *info) {
	GidbitsPluginInfo *copy = NULL;

	g_return_val_if_fail(info, NULL);

	copy = g_slice_new(GidbitsPluginInfo);

	copy->abi_version = info->abi_version;
	copy->name = (info->name) ? g_strdup(info->name) : NULL;
	copy->version = (info->version) ? g_strdup(info->version) : NULL;
	copy->summary = (info->summary) ? g_strdup(info->summary) : NULL;
	copy->description = (info->description) ? g_strdup(info->description)
	                                        : NULL;
	copy->author = (info->author) ? g_strdup(info->author) : NULL;
	copy->website = (info->website) ? g_strdup(info->website) : NULL;

	return copy;
}

