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

#include <gidbits/gidbits-plugin-loader.h>

GType
gidbits_plugin_loader_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		static const GTypeInfo info = {
			.class_size = sizeof(GidbitsPluginLoaderIface),
		};

		type = g_type_register_static(G_TYPE_INTERFACE,
		                              "GidbitsPluginLoader",
		                              &info, 0);
	}

	return type;
}

GList *
gidbits_plugin_loader_get_supported_extensions(GidbitsPluginLoader *loader) {
	GidbitsPluginLoaderIface *iface = NULL;

	g_return_val_if_fail(GIDBITS_IS_PLUGIN_LOADER(loader), NULL);

	iface = GIDBITS_PLUGIN_LOADER_GET_IFACE(loader);

	if(iface && iface->supported_extensions)
		return iface->supported_extensions(loader);

	return NULL;
}

GidbitsPluginInfo *
gidbits_plugin_loader_query_plugin(GidbitsPluginLoader *loader,
                                   const gchar *filename, GError **error)
{
	GidbitsPluginLoaderIface *iface = NULL;

	g_return_val_if_fail(GIDBITS_IS_PLUGIN_LOADER(loader), NULL);
	g_return_val_if_fail(filename, NULL);
	g_return_val_if_fail(error != NULL, NULL);

	iface = GIDBITS_PLUGIN_LOADER_GET_IFACE(loader);

	if(iface && iface->query)
		return iface->query(loader, filename, error);

	return NULL;
}

GidbitsPlugin *
gidbits_plugin_loader_load_plugin(GidbitsPluginLoader *loader,
                                  const gchar *filename, GError **error)
{
	GidbitsPluginLoaderIface *iface = NULL;

	g_return_val_if_fail(GIDBITS_IS_PLUGIN_LOADER(loader), NULL);
	g_return_val_if_fail(filename, NULL);
	g_return_val_if_fail(error != NULL, NULL);

	iface = GIDBITS_PLUGIN_LOADER_GET_IFACE(loader);

	if(iface && iface->load)
		return iface->load(loader, filename, error);

	return NULL;
}

gboolean
gidbits_plugin_loader_unload_plugin(GidbitsPluginLoader *loader,
                                    GidbitsPlugin *plugin, GError **error)
{
	GidbitsPluginLoaderIface *iface = NULL;

	g_return_val_if_fail(GIDBITS_IS_PLUGIN_LOADER(loader), FALSE);
	g_return_val_if_fail(GIDBITS_IS_PLUGIN(plugin), FALSE);

	iface = GIDBITS_PLUGIN_LOADER_GET_IFACE(loader);

	if(iface && iface->unload)
		return iface->unload(loader, plugin, error);

	return FALSE;
}

