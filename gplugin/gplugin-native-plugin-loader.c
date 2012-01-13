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
#define GPLUGIN_LOAD_SYMBOL "gplugin_plugin_load"
#define GPLUGIN_UNLOAD_SYMBOL "gplugin_plugin_unload"

/******************************************************************************
 * Typedefs
 *****************************************************************************/
typedef const GPluginPluginInfo *(*GPluginNativePluginQueryFunc)(void);
typedef gboolean (*GPluginNativePluginLoadFunc)(GPluginNativePlugin *plugin);
typedef gboolean (*GPluginNativePluginUnloadFunc)(GPluginNativePlugin *plugin);

/******************************************************************************
 * Helpers
 *****************************************************************************/
static gpointer
gplugin_native_plugin_loader_lookup_symbol(GModule *module,
                                           const gchar *name, GError **error)
{
	gpointer symbol = NULL;

	g_return_val_if_fail(module != NULL, NULL);

	if(!g_module_symbol(module, name, &symbol)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "symbol %s was not found", name);

		}

		return NULL;
	}

	return symbol;
}

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

/******************************************************************************
 * GPluginPluginLoaderInterface API
 *****************************************************************************/
static GPluginPlugin *
gplugin_native_plugin_loader_query(GPluginPluginLoader *loader,
                                   const gchar *filename,
                                   GError **error)
{
	GPluginPlugin *plugin = NULL;
	const GPluginPluginInfo *info = NULL;
	GPluginNativePluginLoadFunc load = NULL;
	GPluginNativePluginQueryFunc query = NULL;
	GPluginNativePluginUnloadFunc unload = NULL;
	GModule *module = NULL;

	/* open the file via gmodule */
	module = gplugin_native_plugin_loader_open(filename, error);
	if(!module)
		return NULL;

	/* now look for the query symbol */
	query = gplugin_native_plugin_loader_lookup_symbol(module,
	                                                   GPLUGIN_QUERY_SYMBOL,
	                                                   error);
	if(*error) {
		g_module_close(module);
		return NULL;
	}

	/* now look for the load symbol */
	load = gplugin_native_plugin_loader_lookup_symbol(module,
	                                                  GPLUGIN_LOAD_SYMBOL,
	                                                  error);
	if(*error) {
		g_module_close(module);
		return NULL;
	}

	/* now look for the unload symbol */
	unload = gplugin_native_plugin_loader_lookup_symbol(module,
	                                                    GPLUGIN_UNLOAD_SYMBOL,
	                                                    error);
	if(*error) {
		g_module_close(module);
		return NULL;
	}

	/* now we have all of our symbols, so let's see if this plugin will return a
	 * valid GPluginPluginInfo structure
	 */
	info = ((GPluginNativePluginQueryFunc)(query))();
	if(!info) {
		g_module_close(module);

		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "the query function did not return a "
			                     "GPluginPluginInfo structure");
		}

		return NULL;
	}

	/* now create the actual plugin instance */
	plugin = g_object_new(GPLUGIN_TYPE_NATIVE_PLUGIN,
	                      "module", module,
	                      "info", info,
	                      "load-func", load,
	                      "unload-func", unload,
	                      "loader", loader,
	                      "filename", filename,
	                      NULL);

	if(!GPLUGIN_IS_NATIVE_PLUGIN(plugin)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "failed to create plugin instance");
		}

		return NULL;
	}

	return plugin;
}

static gboolean
gplugin_native_plugin_loader_load(GPluginPluginLoader *loader,
                                  GPluginPlugin *plugin,
                                  GError **error)
{
	GPluginNativePluginLoadFunc load = NULL;
	const GPluginPluginInfo *info = NULL;
	gpointer func = NULL;

	g_return_val_if_fail(plugin != NULL, FALSE);
	g_return_val_if_fail(GPLUGIN_IS_NATIVE_PLUGIN(plugin), FALSE);

	/* grab the info from the plugin for error reporting */
	info = gplugin_plugin_get_info(plugin);

	/* grab the load function from the plugin */
	g_object_get(G_OBJECT(plugin), "load-func", &func, NULL);

	/* if the load function is null, we need to bail */
	if(func == NULL) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "load function for %s is NULL",
			                      info->name);
		}

		return FALSE;
	}

	/* now call the load function and exit */
	load = (GPluginNativePluginLoadFunc)func;
	if(!load(GPLUGIN_NATIVE_PLUGIN(plugin)))
		return FALSE;

	return TRUE;
}

static gboolean
gplugin_native_plugin_loader_unload(GPluginPluginLoader *loader,
                                    GPluginPlugin *plugin,
                                    GError **error)
{
	GPluginNativePluginUnloadFunc unload = NULL;
	const GPluginPluginInfo *info = NULL;
	gpointer func = NULL;

	g_return_val_if_fail(plugin != NULL, FALSE);
	g_return_val_if_fail(GPLUGIN_IS_NATIVE_PLUGIN(plugin), FALSE);

	/* grab the info from the plugin for error reporting */
	info = gplugin_plugin_get_info(plugin);

	/* grab the load function from the plugin */
	g_object_get(G_OBJECT(plugin), "unload-func", &func, NULL);

	/* if the load function is null, we need to bail */
	if(func == NULL) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "unload function for %s is NULL",
			                      info->name);
		}

		return FALSE;
	}

	/* now call the unload function and exit */
	unload = (GPluginNativePluginLoadFunc)func;
	if(!unload(GPLUGIN_NATIVE_PLUGIN(plugin)))
		return FALSE;

	return TRUE;
}

static void
gplugin_native_plugin_loader_class_init(GPluginNativePluginLoaderClass *klass) {
	GPluginPluginLoaderClass *loader_class =
		GPLUGIN_PLUGIN_LOADER_CLASS(klass);

	loader_class->supported_extensions = g_slist_append(NULL, G_MODULE_SUFFIX);
	loader_class->query = gplugin_native_plugin_loader_query;
	loader_class->load = gplugin_native_plugin_loader_load;
	loader_class->unload = gplugin_native_plugin_loader_unload;
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
			.class_init = (GClassInitFunc)gplugin_native_plugin_loader_class_init,
			.instance_size = sizeof(GPluginNativePluginLoader),
		};

		type = g_type_register_static(GPLUGIN_TYPE_PLUGIN_LOADER,
		                              "GPluginNativePluginLoader",
		                              &info, 0);
	}

	return type;
}


