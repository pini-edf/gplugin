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

#include "gplugin-gjs-loader.h"
#include "gplugin-gjs-plugin.h"

#include <glib/gi18n.h>

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;
static GType type_real = 0;

/******************************************************************************
 * Helpers
 *****************************************************************************/

/******************************************************************************
 * GPluginLoaderInterface API
 *****************************************************************************/
static GSList *
gplugin_gjs_loader_class_supported_extensions(const GPluginLoaderClass *klass) {
	return g_slist_append(NULL, "js");
}

static GPluginPlugin *
gplugin_gjs_loader_query(GPluginLoader *loader, const gchar *filename,
                          GError **error)
{
	return NULL;
}

static gboolean
gplugin_gjs_loader_load_unload(GPluginLoader *loader,
                                GPluginPlugin *plugin,
                                const gchar *function,
                                GError **error)
{
	return FALSE;
}

static gboolean
gplugin_gjs_loader_load(GPluginLoader *loader, GPluginPlugin *plugin,
                        GError **error)
{
	return gplugin_gjs_loader_load_unload(loader, plugin, "load", error);
}

static gboolean
gplugin_gjs_loader_unload(GPluginLoader *loader, GPluginPlugin *plugin,
                          GError **error)
{
	return gplugin_gjs_loader_load_unload(loader, plugin, "unload", error);
}

/******************************************************************************
 * GObject Stuff
 *****************************************************************************/
static void
gplugin_gjs_loader_class_init(GPluginGjsLoaderClass *klass) {
	GPluginLoaderClass *loader_class = GPLUGIN_LOADER_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	loader_class->supported_extensions =
		gplugin_gjs_loader_class_supported_extensions;
	loader_class->query = gplugin_gjs_loader_query;
	loader_class->load = gplugin_gjs_loader_load;
	loader_class->unload = gplugin_gjs_loader_unload;
}

/******************************************************************************
 * API
 *****************************************************************************/
void
gplugin_gjs_loader_register(GPluginNativePlugin *plugin) {
	if(g_once_init_enter(&type_real)) {
		GType type = 0;

		static const GTypeInfo info = {
			.class_size = sizeof(GPluginGjsLoaderClass),
			.class_init = (GClassInitFunc)gplugin_gjs_loader_class_init,
			.instance_size = sizeof(GPluginGjsLoader),
		};

		type = gplugin_native_plugin_register_type(plugin,
		                                           GPLUGIN_TYPE_LOADER,
		                                           "GPluginGjsLoader",
		                                           &info,
		                                           0);

		g_once_init_leave(&type_real, type);
	}
}

GType
gplugin_gjs_loader_get_type(void) {
	if(G_UNLIKELY(type_real == 0)) {
		g_warning("gplugin_gjs_loader_get_type was called before "
		          "the type was registered!\n");
	}

	return type_real;
}

