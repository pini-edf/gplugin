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

#include <gplugin/gplugin-core.h>
#include <gplugin/gplugin-native-plugin-loader.h>
#include <gplugin/gplugin-native-plugin.h>

#include <gmodule.h>

#define GPLUGIN_QUERY_SYMBOL "gplugin_plugin_query"

/******************************************************************************
 * Typedefs
 *****************************************************************************/
typedef const GPluginPluginInfo *(*GPluginNativePluginQueryFunc)(void);

/******************************************************************************
 * Implementations
 *****************************************************************************/
static GModule *
gplugin_native_plugin_loader_open(const gchar *filename, GError **error) {
	GModule *module = NULL;

	module = g_module_open(filename, 0);
	if(module)
		return module;

	if(error) {
		const gchar *msg = g_module_error();

		*error = g_error_new(GPLUGIN_DOMAIN, 0,
		                     "Failed to open plugin '%s': %s\n",
		                     filename, (msg) ? msg : "Unknown error");
	}

	return NULL;
}

static GPluginPluginInfo *
gplugin_native_plugin_loader_query(GModule *module, GError **error) {
	const GPluginPluginInfo *info = NULL;
	GPluginNativePluginQueryFunc query = NULL;
	gpointer func = NULL;

	if(!g_module_symbol(module, GPLUGIN_QUERY_SYMBOL, &func)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "Query symbol was not found");
		}

		return NULL;
	}

	query = (GPluginNativePluginQueryFunc)func;
	info = query();

	if(!info) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "Query symbol did not return a valid value");
		}

		return NULL;
	}

	if(info->abi_version != GPLUGIN_NATIVE_PLUGIN_ABI_VERSION) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "ABI version mismatch.  Wanted %x, got %x",
			                     GPLUGIN_NATIVE_PLUGIN_ABI_VERSION,
			                     info->abi_version);
		}

		return NULL;
	}

	return gplugin_plugin_info_copy(info);
}

/******************************************************************************
 * GPluginPluginLoaderInterface API
 *****************************************************************************/
static GPluginPluginInfo *
gplugin_native_plugin_loader_query_filename(GPluginPluginLoader *loader,
                                            const gchar *filename,
                                            GError **error)
{
	GPluginPluginInfo *info = NULL;
	GModule *module = NULL;

	module = gplugin_native_plugin_loader_open(filename, error);
	if(!module)
		return NULL;

	info = gplugin_native_plugin_loader_query(module, error);

	g_module_close(module);

	return info;
}

static GPluginPlugin *
gplugin_native_plugin_loader_load(GPluginPluginLoader *loader,
                                  const gchar *filename,
                                  GError **error)
{
	return NULL;
}

static gboolean
gplugin_native_plugin_loader_unload(GPluginPluginLoader *loader,
                                    GPluginPlugin *plugin,
                                    GError **error)
{
	return FALSE;
}

static void
gplugin_native_loader_loader_loader_init(GPluginPluginLoaderIface *iface) {
	iface->supported_extensions = g_slist_append(NULL, G_MODULE_SUFFIX);
	iface->query = gplugin_native_plugin_loader_query_filename;
	iface->load = gplugin_native_plugin_loader_load;
	iface->unload = gplugin_native_plugin_loader_unload;
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
gplugin_native_plugin_loader_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		static const GTypeInfo info = {
			.class_size = sizeof(GPluginNativePluginLoaderClass),
			.instance_size = sizeof(GPluginNativePluginLoader),
		};

		static const GInterfaceInfo loader_info = {
			.interface_init = (GInterfaceInitFunc)gplugin_native_loader_loader_loader_init,
		};

		type = g_type_register_static(G_TYPE_OBJECT,
		                              "GPluginNativePluginLoader",
		                              &info, 0);
		g_type_add_interface_static(type, GPLUGIN_TYPE_PLUGIN_LOADER, &loader_info);
	}

	return type;
}


