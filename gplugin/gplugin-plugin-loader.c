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

#include <gplugin/gplugin-plugin-loader.h>

GType
gplugin_plugin_loader_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		static const GTypeInfo info = {
			.class_size = sizeof(GPluginPluginLoaderClass),
			.instance_size = sizeof(GPluginPluginLoader),
		};

		type = g_type_register_static(G_TYPE_OBJECT,
		                              "GPluginPluginLoader",
		                              &info, G_TYPE_FLAG_ABSTRACT);
	}

	return type;
}

/**
 * gplugin_plugin_loader_query_plugin:
 * @loader: #GPluginPluginLoader instance performing the query
 * @filename: filename to query
 * error: return location for a GError, or NULL
 *
 * Return value: (transfer full): A #GPluginPlugin instance or NULL on failure
 */
GPluginPlugin *
gplugin_plugin_loader_query_plugin(GPluginPluginLoader *loader,
                                   const gchar *filename, GError **error)
{
	GPluginPluginLoaderClass *klass = NULL;

	g_return_val_if_fail(loader != NULL, NULL);
	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_LOADER(loader), NULL);
	g_return_val_if_fail(filename, NULL);
	g_return_val_if_fail(error != NULL, NULL);

	klass = GPLUGIN_PLUGIN_LOADER_GET_CLASS(loader);

	if(klass && klass->query)
		return klass->query(loader, filename, error);

	return NULL;
}

/**
 * gplugin_plugin_loader_load_plugin:
 * @loader: #GPluginPluginLoader instance performing the load
 * @plugin: #GPluginPlugin instance to load
 * error: return location for a GError, or NULL
 *
 * Return value: TRUE if @plugin was loaded successfully, FALSE otherwise
 */
gboolean
gplugin_plugin_loader_load_plugin(GPluginPluginLoader *loader,
                                  GPluginPlugin *plugin, GError **error)
{
	GPluginPluginLoaderClass *klass = NULL;

	g_return_val_if_fail(loader != NULL, FALSE);
	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_LOADER(loader), FALSE);
	g_return_val_if_fail(GPLUGIN_IS_PLUGIN(plugin), FALSE);
	g_return_val_if_fail(error != NULL, FALSE);

	klass = GPLUGIN_PLUGIN_LOADER_GET_CLASS(loader);

	if(klass && klass->load)
		return klass->load(loader, plugin, error);

	return FALSE;
}

/**
 * gplugin_plugin_loader_unload_plugin:
 * @loader: #GPluginPluginLoader instance performing the unload
 * @plugin: #GPluginPlugin instance to unload
 * error: return location for a GError, or NULL
 *
 * Return value: TRUE if @plugin was unloaded successfully, FALSE otherwise
 */
gboolean
gplugin_plugin_loader_unload_plugin(GPluginPluginLoader *loader,
                                    GPluginPlugin *plugin, GError **error)
{
	GPluginPluginLoaderClass *klass = NULL;

	g_return_val_if_fail(loader != NULL, FALSE);
	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_LOADER(loader), FALSE);
	g_return_val_if_fail(GPLUGIN_IS_PLUGIN(plugin), FALSE);

	klass = GPLUGIN_PLUGIN_LOADER_GET_CLASS(loader);

	if(klass && klass->unload)
		return klass->unload(loader, plugin, error);

	return FALSE;
}

