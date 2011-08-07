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
static GList *paths = NULL;
static GHashTable *plugins = NULL;

/******************************************************************************
 * API
 *****************************************************************************/
void
gidbits_plugins_add_path(const gchar *path_str) {
	if(!path_str)
		return;

	if(g_list_find(paths, path_str))
		return;

	paths = g_list_append(paths, g_strdup(path_str));
}

void
gidbits_plugins_remove_path(const gchar *path_str) {
	GList *l = NULL;
	gchar *path = NULL;

	if(!path_str)
		return;

	if(!(l = g_list_find(paths, path_str)))
		return;

	path = (gchar *)l->data;
	paths = g_list_remove(paths, path);
	g_free(path);
}

GList *
gidbits_plugins_get_paths(void) {
	return paths;
}

