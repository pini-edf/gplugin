/*
 * Copyright (C) 2011-2013 Gary Kramlich <grim@reaperworld.com>
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

#include <gplugin/gplugin-native-loader.h>
#include <gplugin/gplugin-native-plugin.h>
#include <gplugin/gplugin-native-private.h>

#include <glib/gi18n.h>

#include <gmodule.h>

#define GPLUGIN_QUERY_SYMBOL "gplugin_query"
#define GPLUGIN_LOAD_SYMBOL "gplugin_load"
#define GPLUGIN_UNLOAD_SYMBOL "gplugin_unload"

/******************************************************************************
 * Helpers
 *****************************************************************************/
static gpointer
gplugin_native_loader_lookup_symbol(GModule *module,
                                    const gchar *name, GError **error)
{
	gpointer symbol = NULL;

	g_return_val_if_fail(module != NULL, NULL);

	if(!g_module_symbol(module, name, &symbol)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     _("symbol %s was not found"), name);

		}

		return NULL;
	}

	return symbol;
}

static GModule *
gplugin_native_loader_open(const gchar *filename, GError **error) {
	GModule *module = NULL;

	module = g_module_open(filename, 0);
	if(module)
		return module;

	if(error) {
		const gchar *msg = g_module_error();

		*error = g_error_new(GPLUGIN_DOMAIN, 0,
		                     _("Failed to open plugin '%s': %s"),
		                     filename, (msg) ? msg : _("Unknown error"));
	}

	return NULL;
}

/******************************************************************************
 * GPluginLoaderInterface API
 *****************************************************************************/
static GSList *
gplugin_native_loader_class_supported_extensions(const GPluginLoaderClass *klass) {
	return g_slist_append(NULL, G_MODULE_SUFFIX);
}

static GPluginPlugin *
gplugin_native_loader_query(GPluginLoader *loader,
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
	module = gplugin_native_loader_open(filename, error);
	if(!module)
		return NULL;

	/* now look for the query symbol */
	query = gplugin_native_loader_lookup_symbol(module,
	                                            GPLUGIN_QUERY_SYMBOL,
	                                            error);
	if((query == NULL) || (error && *error)) {
		g_module_close(module);
		return NULL;
	}

	/* now look for the load symbol */
	load = gplugin_native_loader_lookup_symbol(module,
	                                           GPLUGIN_LOAD_SYMBOL,
	                                           error);
	if(error && *error) {
		g_module_close(module);
		return NULL;
	}

	/* now look for the unload symbol */
	unload = gplugin_native_loader_lookup_symbol(module,
	                                             GPLUGIN_UNLOAD_SYMBOL,
	                                             error);
	if(error && *error) {
		g_module_close(module);
		return NULL;
	}

	/* now we have all of our symbols, so let's see if this plugin will return a
	 * valid GPluginPluginInfo structure
	 */
	info = ((GPluginNativePluginQueryFunc)(query))(error);
	if(error && *error) {
		g_module_close(module);

		return NULL;
	}

	if(!GPLUGIN_IS_PLUGIN_INFO(info)) {
		g_module_close(module);

		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     _("the query function did not return a "
			                       "GPluginPluginInfo instance"));
		}

		return NULL;
	}

	/* claim ownership of the info object */
	g_object_ref_sink(G_OBJECT(info));

	/* now create the actual plugin instance */
	plugin = g_object_new(GPLUGIN_TYPE_NATIVE_PLUGIN,
	                      "module", module,
	                      "info", info,
	                      "load-func", load,
	                      "unload-func", unload,
	                      "loader", loader,
	                      "filename", filename,
	                      NULL);

	/* now that the plugin instance owns the info, remove our ref */
	g_object_unref(G_OBJECT(info));

	if(!GPLUGIN_IS_NATIVE_PLUGIN(plugin)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     _("failed to create plugin instance"));
		}
		return NULL;
	}

	return plugin;
}

static gboolean
gplugin_native_loader_load(GPluginLoader *loader,
                           GPluginPlugin *plugin,
                           GError **error)
{
	GPluginNativePluginLoadFunc func;

	g_return_val_if_fail(plugin != NULL, FALSE);
	g_return_val_if_fail(GPLUGIN_IS_NATIVE_PLUGIN(plugin), FALSE);

	/* get the function */
	g_object_get(G_OBJECT(plugin), "load-func", &func, NULL);

	/* validate the function */
	if(func == NULL) {
		const char *filename = gplugin_plugin_get_filename(plugin);

		g_warning(_("load function for %s is NULL"), filename);

		return FALSE;
	}

	/* now call the function */
	if(!func(GPLUGIN_NATIVE_PLUGIN(plugin), error)) {
		if(error && *error == NULL)
			*error = g_error_new(GPLUGIN_DOMAIN, 0, _("unknown failure"));

		return FALSE;
	}

	return TRUE;
}

static gboolean
gplugin_native_loader_unload(GPluginLoader *loader,
                             GPluginPlugin *plugin,
                             GError **error)
{
	GPluginNativePluginUnloadFunc func;

	g_return_val_if_fail(plugin != NULL, FALSE);
	g_return_val_if_fail(GPLUGIN_IS_NATIVE_PLUGIN(plugin), FALSE);

	/* get the function */
	g_object_get(G_OBJECT(plugin), "unload-func", &func, NULL);

	/* validate the function */
	if(func == NULL) {
		const char *filename = gplugin_plugin_get_filename(plugin);

		g_warning(_("unload function for %s is NULL"), filename);

		return FALSE;
	}

	/* now call the function */
	if(!func(GPLUGIN_NATIVE_PLUGIN(plugin), error)) {
		if(error && *error)
			*error = g_error_new(GPLUGIN_DOMAIN, 0, _("unknown failure"));

		return FALSE;
	}

	return TRUE;
}

static void
gplugin_native_loader_class_init(GPluginNativeLoaderClass *klass) {
	GPluginLoaderClass *loader_class =
		GPLUGIN_LOADER_CLASS(klass);

	loader_class->supported_extensions =
		gplugin_native_loader_class_supported_extensions;
	loader_class->query = gplugin_native_loader_query;
	loader_class->load = gplugin_native_loader_load;
	loader_class->unload = gplugin_native_loader_unload;
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
gplugin_native_loader_get_type(void) {
	static volatile gsize type_volatile = 0;

	if(g_once_init_enter(&type_volatile)) {
		 GType type = 0;

		static const GTypeInfo info = {
			.class_size = sizeof(GPluginNativeLoaderClass),
			.class_init = (GClassInitFunc)gplugin_native_loader_class_init,
			.instance_size = sizeof(GPluginNativeLoader),
		};

		type = g_type_register_static(GPLUGIN_TYPE_LOADER,
		                              "GPluginNativeLoader",
		                              &info, 0);

		g_once_init_leave(&type_volatile, type);
	}

	return type_volatile;
}


