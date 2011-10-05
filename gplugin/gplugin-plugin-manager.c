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

#include <glib.h>

#include <gplugin/gplugin-plugin-manager.h>
#include <gplugin/gplugin-core.h>
#include <gplugin/gplugin-native-plugin-loader.h>

/******************************************************************************
 * Globals
 *****************************************************************************/
static GHashTable *paths = NULL;

static GHashTable *plugins = NULL;
static GHashTable *loaders = NULL;

/******************************************************************************
 * Private API
 *****************************************************************************/
void
gplugin_plugin_manager_init(void) {
	paths = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

	plugins = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

	loaders = g_hash_table_new_full(g_direct_hash, g_direct_equal,
	                                NULL, g_object_unref);

	gplugin_plugin_manager_register_loader(GPLUGIN_TYPE_NATIVE_PLUGIN_LOADER);
}

void
gplugin_plugin_manager_uninit(void) {
	g_hash_table_destroy(paths);
	g_hash_table_destroy(plugins);
	g_hash_table_destroy(loaders);
}

/******************************************************************************
 * API
 *****************************************************************************/
void
gplugin_plugin_manager_add_path(const gchar *path_str) {
	if(!path_str)
		return;

	g_hash_table_insert(paths, g_strdup(path_str), NULL);
}

void
gplugin_plugin_manager_remove_path(const gchar *path_str) {
	g_hash_table_remove(paths, path_str);
}

GList *
gplugin_plugin_manager_get_paths(void) {
	return g_hash_table_get_keys(paths);
}

void
gplugin_plugin_manager_register_loader(GType type) {
	GPluginPluginLoader *lo = NULL;

	g_return_if_fail(type != G_TYPE_INVALID);

	lo = g_object_new(type, NULL);
	if(!lo)
		return;

	g_hash_table_insert(loaders, GINT_TO_POINTER(type), lo);
}

void
gplugin_plugin_manager_unregister_loader(GType type) {
	g_hash_table_remove(loaders, GINT_TO_POINTER(type));
}

void
gplugin_plugin_manager_refresh(void) {
	GHashTableIter piter;
	gpointer key;

	g_hash_table_iter_init(&piter, paths);
	while(g_hash_table_iter_next(&piter, &key, NULL)) {
		GDir *dir = NULL;
		GError *error = NULL;

		dir = g_dir_open((const gchar *)key, 0, &error);
		if(error) {
			g_warning("Failed to open %s: %s\n",
			          (const gchar *)key,
			          (error->message) ? error->message : "unknown failure");

			g_error_free(error);
			error = NULL;

			continue;
		}

		g_dir_close(dir);
	}
}

