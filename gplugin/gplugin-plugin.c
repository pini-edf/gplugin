/*
 * Copyright (C) 2011-2012 Gary Kramlich <grim@reaperworld.com>
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

#include <gplugin/gplugin-plugin.h>

#include <gplugin/gplugin-enums.h>
#include <gplugin/gplugin-marshallers.h>
#include <gplugin/gplugin-private.h>

#define GPLUGIN_PLUGIN_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GPLUGIN_TYPE_PLUGIN, GPluginPluginPrivate))

/******************************************************************************
 * Structs
 *****************************************************************************/
typedef struct {
	gchar *filename;

	GPluginPluginLoader *loader;
	GPluginPluginInfo *info;

	GPluginPluginState state;
} GPluginPluginPrivate;

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	PROP_FILENAME,
	PROP_LOADER,
	PROP_INFO,
	PROP_STATE,
	PROP_LAST,
};

enum {
	SIG_STATE_CHANGED,
	SIG_LAST,
};

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;
static guint signals[SIG_LAST] = {0, };

/******************************************************************************
 * Private API
 *****************************************************************************/
static void
gplugin_plugin_set_filename(GPluginPlugin *plugin, const gchar *filename) {
	GPluginPluginPrivate *priv = GPLUGIN_PLUGIN_GET_PRIVATE(plugin);

	g_free(priv->filename);

	priv->filename = (filename) ? g_strdup(filename) : NULL;

	g_object_notify(G_OBJECT(plugin), "filename");
}

static void
gplugin_plugin_set_loader(GPluginPlugin *plugin, GPluginPluginLoader *loader) {
	GPluginPluginPrivate *priv = GPLUGIN_PLUGIN_GET_PRIVATE(plugin);

	if(priv->loader)
		g_object_unref(G_OBJECT(priv->loader));

	if(GPLUGIN_IS_PLUGIN_LOADER(loader))
		priv->loader = g_object_ref(G_OBJECT(loader));
	else
		priv->loader = NULL;
}

static void
gplugin_plugin_set_info(GPluginPlugin *plugin, GPluginPluginInfo *info) {
	GPluginPluginPrivate *priv = GPLUGIN_PLUGIN_GET_PRIVATE(plugin);

	if(priv->info)
		gplugin_plugin_info_free(priv->info);

	if(info)
		priv->info = gplugin_plugin_info_copy(info);
	else
		priv->info = NULL;
}

gchar *
gplugin_plugin_get_internal_filename(GPluginPlugin *plugin) {
	GPluginPluginPrivate *priv = GPLUGIN_PLUGIN_GET_PRIVATE(plugin);

	return priv->filename;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
gplugin_plugin_get_property(GObject *obj, guint param_id, GValue *value,
                            GParamSpec *pspec)
{
	GPluginPlugin *plugin = GPLUGIN_PLUGIN(obj);

	switch(param_id) {
		case PROP_FILENAME:
			g_value_set_string(value, gplugin_plugin_get_filename(plugin));
			break;
		case PROP_LOADER:
			g_value_set_object(value, gplugin_plugin_get_loader(plugin));
			break;
		case PROP_INFO:
			g_value_set_boxed(value, gplugin_plugin_get_info(plugin));
			break;
		case PROP_STATE:
			g_value_set_enum(value, gplugin_plugin_get_state(plugin));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_plugin_set_property(GObject *obj, guint param_id, const GValue *value,
                            GParamSpec *pspec)
{
	GPluginPlugin *plugin = GPLUGIN_PLUGIN(obj);

	switch(param_id) {
		case PROP_FILENAME:
			gplugin_plugin_set_filename(plugin, g_value_get_string(value));
			break;
		case PROP_LOADER:
			gplugin_plugin_set_loader(plugin, g_value_get_object(value));
			break;
		case PROP_INFO:
			gplugin_plugin_set_info(plugin, g_value_get_boxed(value));
			break;
		case PROP_STATE:
			gplugin_plugin_set_state(plugin, g_value_get_enum(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_plugin_finalize(GObject *obj) {
	GPluginPluginPrivate *priv = GPLUGIN_PLUGIN_GET_PRIVATE(obj);

	g_free(priv->filename);
	g_object_unref(priv->loader);
	gplugin_plugin_info_free(priv->info);

	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static void
gplugin_plugin_class_init(GPluginPluginClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	g_type_class_add_private(klass, sizeof(GPluginPluginPrivate));

	obj_class->get_property = gplugin_plugin_get_property;
	obj_class->set_property = gplugin_plugin_set_property;
	obj_class->finalize = gplugin_plugin_finalize;

	g_object_class_install_property(obj_class, PROP_FILENAME,
		g_param_spec_string("filename", "filename",
		                    "The filename of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_LOADER,
		g_param_spec_object("loader", "loader",
		                    "The loader for this plugin",
		                    GPLUGIN_TYPE_PLUGIN_LOADER,
		                    G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_INFO,
		g_param_spec_boxed("info", "info",
		                   "The information for the plugin",
		                   GPLUGIN_TYPE_PLUGIN_INFO,
		                   G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_STATE,
		g_param_spec_enum("state", "state",
		                  "The state of the plugin",
	                      GPLUGIN_TYPE_PLUGIN_STATE,
		                  GPLUGIN_PLUGIN_STATE_UNKNOWN,
		                  G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	signals[SIG_STATE_CHANGED] =
		g_signal_new("state-changed",
		             G_OBJECT_CLASS_TYPE(klass),
		             G_SIGNAL_RUN_LAST,
		             G_STRUCT_OFFSET(GPluginPluginClass, state_changed),
		             NULL, NULL,
		             gplugin_marshal_VOID__ENUM_ENUM,
		             G_TYPE_NONE,
		             2,
		             GPLUGIN_TYPE_PLUGIN_STATE, GPLUGIN_TYPE_PLUGIN_STATE);
}

/******************************************************************************
 * GPluginPlugin API
 *****************************************************************************/
GType
gplugin_plugin_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		static const GTypeInfo info = {
			.class_size = sizeof(GPluginPluginClass),
			.class_init = (GClassInitFunc)gplugin_plugin_class_init,
			.instance_size = sizeof(GPluginPlugin),
		};

		type = g_type_register_static(G_TYPE_OBJECT,
		                              "GPluginPlugin",
		                              &info, G_TYPE_FLAG_ABSTRACT);
	}

	return type;
}

/**
 * gplugin_plugin_get_filename:
 * @plugin: #GPluginPlugin instance
 *
 * Return value: The filename of @plugin
 */
const gchar *
gplugin_plugin_get_filename(const GPluginPlugin *plugin) {
	GPluginPluginPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN(plugin), NULL);

	priv = GPLUGIN_PLUGIN_GET_PRIVATE(plugin);

	return priv->filename;
}

/**
 * gplugin_plugin_get_loader:
 * @plugin: #GPluginPlugin instance
 *
 * Return Value: (transfer none): The #GPluginPluginLoader that loaded @plugin
 */
GPluginPluginLoader *
gplugin_plugin_get_loader(const GPluginPlugin *plugin) {
	GPluginPluginPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN(plugin), NULL);

	priv = GPLUGIN_PLUGIN_GET_PRIVATE(plugin);

	return priv->loader;
}

/**
 * gplugin_plugin_get_info:
 * @plugin: #GPluginPlugin instance
 *
 * Return value: (transfer none): The #GPluginPluginInfo instance for @plugin
 */
const GPluginPluginInfo *
gplugin_plugin_get_info(const GPluginPlugin *plugin) {
	GPluginPluginPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN(plugin), NULL);

	priv = GPLUGIN_PLUGIN_GET_PRIVATE(plugin);

	return priv->info;
}

/**
 * gplugin_plugin_get_state:
 * @plugin: #GPluginPlugin instance
 *
 * Gets the current state of @plugin
 *
 * Return value: (transfer full): The current state of @plugin
 */
GPluginPluginState
gplugin_plugin_get_state(const GPluginPlugin *plugin) {
	GPluginPluginPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN(plugin), GPLUGIN_PLUGIN_STATE_UNKNOWN);

	priv = GPLUGIN_PLUGIN_GET_PRIVATE(plugin);

	return priv->state;
}

/**
 * gplugin_plugin_set_state:
 * @plugin: #GPluginPlugin instance
 * @state: new #GPluginPluginState for @plugin
 *
 * Changes the state of @plugin to @state.  This function should only be called
 * by loaders.
 */
void
gplugin_plugin_set_state(GPluginPlugin *plugin, GPluginPluginState state) {
	GPluginPluginPrivate *priv = NULL;
	GPluginPluginState oldstate = GPLUGIN_PLUGIN_STATE_UNKNOWN;

	g_return_if_fail(GPLUGIN_IS_PLUGIN(plugin));

	priv = GPLUGIN_PLUGIN_GET_PRIVATE(plugin);

	oldstate = priv->state;
	priv->state = state;

	g_signal_emit(plugin, signals[SIG_STATE_CHANGED], 0,
	              oldstate, priv->state);
}

/******************************************************************************
 * GPluginPluginInfo API
 *****************************************************************************/
GType
gplugin_plugin_info_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		type = g_boxed_type_register_static("GPluginPluginInfo",
		                                    (GBoxedCopyFunc)gplugin_plugin_info_copy,
		                                    (GBoxedFreeFunc)gplugin_plugin_info_free);
	}

	return type;
}

/**
 * gplugin_plugin_info_free:
 * @info: #GPluginPluginInfo instance to free
 *
 * Free's an allocated #GPluginPluginInfo instance
 */
void
gplugin_plugin_info_free(GPluginPluginInfo *info) {
	GSList *l = NULL;

	g_return_if_fail(info);

	g_free(info->id);
	g_free(info->name);
	g_free(info->version);
	g_free(info->summary);
	g_free(info->description);
	g_free(info->author);
	g_free(info->website);

	for(l = info->dependencies; l; l = l->next)
		g_free(l->data);

	g_slist_free(info->dependencies);

	g_slice_free(GPluginPluginInfo, info);
}

/**
 * gplugin_plugin_info_copy:
 * @info: #GPluginPluginInfo instance
 *
 * Creates an allocated copy of @info
 *
 * Return value: (transfer full): A newly allocated #GPluginPluginInfo instance
 */
GPluginPluginInfo *
gplugin_plugin_info_copy(const GPluginPluginInfo *info) {
	GPluginPluginInfo *copy = NULL;
	GSList *l = NULL;

	g_return_val_if_fail(info, NULL);

	copy = g_slice_new(GPluginPluginInfo);

	copy->id = (info->id) ? g_strdup(info->id) : NULL;
	copy->abi_version = info->abi_version;
	copy->flags = info->flags;
	copy->name = (info->name) ? g_strdup(info->name) : NULL;
	copy->version = (info->version) ? g_strdup(info->version) : NULL;
	copy->summary = (info->summary) ? g_strdup(info->summary) : NULL;
	copy->description = (info->description) ? g_strdup(info->description)
	                                        : NULL;
	copy->author = (info->author) ? g_strdup(info->author) : NULL;
	copy->website = (info->website) ? g_strdup(info->website) : NULL;

	copy->dependencies = NULL;

	for(l = info->dependencies; l; l = l->next) {
		copy->dependencies = g_slist_append(copy->dependencies,
		                                    g_strdup(l->data));
	}

	return copy;
}

