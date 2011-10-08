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

static gboolean refresh_needed = FALSE;

/******************************************************************************
 * Helpers
 *****************************************************************************/
static void
gplugin_plugin_manager_remove_loader_value(gpointer data) {
	GSList *loaders = (GSList *)data;
	GSList *l = NULL;

	for(l = loaders; l; l = l->next)
		g_object_unref(G_OBJECT(l->data));

	g_slist_free(loaders);
}

/******************************************************************************
 * Private API
 *****************************************************************************/
void
gplugin_plugin_manager_init(void) {
	paths = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

	plugins = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

	/* The loaders hash table is keyed on the supported extensions of the
	 * loader.  Which means that a loader that supports multiple extensions
	 * will be in the table multiple times.
	 *
	 * We deal with collisions by using a GSList for the value which will hold
	 * references to instances of the actual loaders.
	 *
	 * Storing this in this method allows up to quickly figure out which loader
	 * to use by the filename and helps us to avoid iterating the loaders table
	 * again and again.
	 */
	loaders = g_hash_table_new_full(g_str_hash, g_str_equal, g_free,
	                                gplugin_plugin_manager_remove_loader_value);

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
	GPluginPluginLoaderIface *iface = NULL;
	GSList *l = NULL;

	g_return_if_fail(g_type_is_a(type, GPLUGIN_TYPE_PLUGIN_LOADER));

	/* Create the loader instance first.  If we can't create it, we bail */
	lo = g_object_new(type, NULL);
	if(!GPLUGIN_IS_PLUGIN_LOADER(lo))
		return;

	/* grab the loader class and make sure it's valid */
	iface = GPLUGIN_PLUGIN_LOADER_GET_IFACE(lo);
	if(!iface) {
		g_object_unref(G_OBJECT(lo));

		return;
	}

	for(l = iface->supported_extensions; l; l = l->next) {
		GSList *existing = NULL, *ll = NULL;
		const gchar *ext = (const gchar *)l->data;

		/* grab any existing loaders that are registered for this type so that
		 * we can prepend our loader.  But before we add our, we remove any old
		 * copies we might have our ours.
		 */
		existing = g_hash_table_lookup(loaders, ext);
		for(ll = existing; ll; ll = ll->next) {
			if(G_OBJECT_TYPE(ll->data) == type) {
				GPluginPluginLoader *old = GPLUGIN_PLUGIN_LOADER(ll->data);

				existing = g_slist_remove(existing, old);

				g_object_unref(G_OBJECT(old));

				break;
			}
		}

		existing = g_slist_prepend(existing, g_object_ref(G_OBJECT(lo)));

		/* Now insert the updated slist back into the hash table */
		g_hash_table_insert(loaders, g_strdup(ext), existing);
	}

	/* make a note that we need to refresh */
	refresh_needed = TRUE;

	/* we remove our initial reference from the loader now to avoid a leak */
	g_object_unref(G_OBJECT(lo));
}

void
gplugin_plugin_manager_unregister_loader(GType type) {
	GPluginPluginLoaderIface *iface = NULL;
	GSList *exts = NULL;

	g_return_if_fail(g_type_is_a(type, GPLUGIN_TYPE_PLUGIN_LOADER));

	iface = g_type_class_ref(type);
	if(!iface)
		return;

	for(exts = iface->supported_extensions; exts; exts = exts->next) {
		GSList *los = NULL;
		GSList *l = NULL;
		const gchar *ext = NULL;

		ext = (const gchar *)exts->data;
		los = g_hash_table_lookup(loaders, ext);

		for(l = los; l; l = l->next) {
			GPluginPluginLoader *lo = GPLUGIN_PLUGIN_LOADER(l->data);

			if(!GPLUGIN_IS_PLUGIN_LOADER(lo))
				continue;

			if(G_OBJECT_TYPE(lo) != type)
				continue;

			/* at this point, the loader we're at is of the type we're
			 * removing.  So we'll remove it from the los SList.  Then if the
			 * SList is empty, we remove it from the hash table, otherwise we
			 * just update it.
			 */
			los = g_slist_remove(los, lo);
			if(los)
				g_hash_table_insert(loaders, g_strdup(ext), los);
			else
				g_hash_table_remove(loaders, ext);

			/* kill our ref to the loader */
			g_object_unref(G_OBJECT(lo));

			/* now move to the next extension to check */
			break;
		}
	}

	g_type_class_unref(iface);
}

void
gplugin_plugin_manager_refresh(void) {
	refresh_needed = TRUE;

	while(refresh_needed) {
		GHashTableIter piter;
		gpointer key;

		refresh_needed = FALSE;

		g_hash_table_iter_init(&piter, paths);
		while(g_hash_table_iter_next(&piter, &key, NULL)) {
			GDir *dir = NULL;
			GError *error = NULL;

			dir = g_dir_open((const gchar *)key, 0, &error);
			if(error) {
				g_warning("Failed to open %s: %s\n",
				          (const gchar *)key,
					      (error->message) ? error->message :
				                             "unknown failure");

				g_error_free(error);
				error = NULL;

				continue;
			}

			g_dir_close(dir);
		}
	}
}

