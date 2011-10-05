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

/******************************************************************************
 * Globals
 *****************************************************************************/
static GHashTable *paths = NULL;
static GHashTable *plugins = NULL;

/******************************************************************************
 * Private API
 *****************************************************************************/
void
gplugin_plugin_manager_init(void) {
	paths = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

	plugins = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
}

void
gplugin_plugin_manager_uninit(void) {
	g_hash_table_destroy(paths);
	g_hash_table_destroy(plugins);
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

