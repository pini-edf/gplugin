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

#include <gidbits/gidbits-core.h>
#include <gidbits/gidbits-native-plugin-loader.h>
#include <gidbits/gidbits-native-plugin.h>

#include <gmodule.h>

#define GIDBITS_QUERY_SYMBOL "gidbits_plugin_query"

/******************************************************************************
 * Typedefs
 *****************************************************************************/
typedef const GidbitsPluginInfo *(*GidbitsNativePluginQueryFunc)(void);

/******************************************************************************
 * Implementations
 *****************************************************************************/
static GModule *
gidbits_native_plugin_loader_open(const gchar *filename, GError **error) {
	GModule *module = NULL;

	module = g_module_open(filename, 0);
	if(module)
		return module;

	if(error) {
		const gchar *msg = g_module_error();

		*error = g_error_new(GIDBITS_DOMAIN, 0,
		                     "Failed to open plugin '%s': %s\n",
		                     filename, (msg) ? msg : "Unknown error");
	}

	return NULL;
}

static GidbitsPluginInfo *
gidbits_native_plugin_loader_query(GModule *module, GError **error) {
	const GidbitsPluginInfo *info = NULL;
	GidbitsNativePluginQueryFunc query = NULL;
	gpointer func = NULL;

	if(!g_module_symbol(module, GIDBITS_QUERY_SYMBOL, &func)) {
		if(error) {
			*error = g_error_new(GIDBITS_DOMAIN, 0,
			                     "Query symbol was not found");
		}

		return NULL;
	}

	query = (GidbitsNativePluginQueryFunc)func;
	info = query();

	if(!info) {
		if(error) {
			*error = g_error_new(GIDBITS_DOMAIN, 0,
			                     "Query symbol did not return a valid value");
		}

		return NULL;
	}

	if(info->abi_version != GIDBITS_NATIVE_PLUGIN_ABI_VERSION) {
		if(error) {
			*error = g_error_new(GIDBITS_DOMAIN, 0,
			                     "ABI version mismatch.  Wanted %x, got %x",
			                     GIDBITS_NATIVE_PLUGIN_ABI_VERSION,
			                     info->abi_version);
		}

		return NULL;
	}

	return gidbits_plugin_info_copy(info);
}

/******************************************************************************
 * GidbitsPluginLoaderInterface API
 *****************************************************************************/
static GList *
gidbits_native_plugin_loader_supported_extensions(GidbitsPluginLoader *loader) {
	return g_list_append(NULL, G_MODULE_SUFFIX);
}

static GidbitsPluginInfo *
gidbits_native_plugin_loader_query_filename(GidbitsPluginLoader *loader,
                                            const gchar *filename,
                                            GError **error)
{
	GidbitsPluginInfo *info = NULL;
	GModule *module = NULL;

	module = gidbits_native_plugin_loader_open(filename, error);
	if(!module)
		return NULL;

	info = gidbits_native_plugin_loader_query(module, error);

	g_module_close(module);

	return info;
}

static GidbitsPlugin *
gidbits_native_plugin_loader_load(GidbitsPluginLoader *loader,
                                  const gchar *filename,
                                  GError **error)
{
	return NULL;
}

static gboolean
gidbits_native_plugin_loader_unload(GidbitsPluginLoader *loader,
                                    GidbitsPlugin *plugin,
                                    GError **error)
{
	return FALSE;
}

static void
gidbits_native_loader_loader_loader_init(GidbitsPluginLoaderIface *iface) {
	iface->supported_extensions = gidbits_native_plugin_loader_supported_extensions;
	iface->query = gidbits_native_plugin_loader_query_filename;
	iface->load = gidbits_native_plugin_loader_load;
	iface->unload = gidbits_native_plugin_loader_unload;
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
gidbits_native_plugin_loader_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		static const GTypeInfo info = {
			.class_size = sizeof(GidbitsNativePluginLoaderClass),
			.instance_size = sizeof(GidbitsNativePluginLoader),
		};

		static const GInterfaceInfo loader_info = {
			.interface_init = (GInterfaceInitFunc)gidbits_native_loader_loader_loader_init,
		};

		type = g_type_register_static(G_TYPE_OBJECT,
		                              "GidbitsNativePluginLoader",
		                              &info, 0);
		g_type_add_interface_static(type, GIDBITS_TYPE_PLUGIN_LOADER, &loader_info);
	}

	return type;
}


