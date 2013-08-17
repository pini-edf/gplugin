/*
 * Copyright (C) 2011-2012 Gary Kramlich <grim@reaperworld.com>
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

#include <string.h>

#include <glib.h>

#include <gplugin/gplugin-plugin-manager.h>
#include <gplugin/gplugin-core.h>

#include <gplugin/gplugin-private.h>

#include <gplugin/gplugin-native.h>

/******************************************************************************
 * Structs
 *****************************************************************************/
typedef struct {
	gchar *filename;
	gchar *extension;
} GPluginPluginManagerTreeEntry;

/******************************************************************************
 * Globals
 *****************************************************************************/
static GQueue *paths = NULL;

static GHashTable *plugins = NULL;
static GHashTable *plugins_filename_view = NULL;

static GHashTable *loaders = NULL;

static gboolean refresh_needed = FALSE;

/******************************************************************************
 * Helpers
 *****************************************************************************/
static guint
gplugin_plugin_manager_str_hash(gconstpointer v) {
	if(v == NULL)
		return g_str_hash("");

	return g_str_hash(v);
}

static GPluginPluginManagerTreeEntry *
gplugin_plugin_manager_tree_entry_new(const gchar *filename) {
	GPluginPluginManagerTreeEntry *e = NULL;

	e = g_slice_new(GPluginPluginManagerTreeEntry);

	e->filename = g_filename_to_utf8(filename, -1, NULL, NULL, NULL);

	/* we have to use e->filename, because g_utf8_strrchr returns a pointer
	 * in the string given too it, and not a new copy.
	 */
	e->extension = g_utf8_strrchr(e->filename, -1, g_utf8_get_char("."));

	/* if we have an extension, we need to iterate past the . */
	if(e->extension)
		e->extension = g_utf8_next_char(e->extension);

	return e;
}

static void
gplugin_plugin_manager_tree_entry_free(GPluginPluginManagerTreeEntry *e) {
	if(!e)
		return;

	g_free(e->filename);

	g_slice_free(GPluginPluginManagerTreeEntry, e);

	e = NULL;
}

/**
 * gplugin_plugin_manager_file_tree_new:
 *
 * Builds a GNode tree of consisting of a root node, whose children contain
 * an allocated string of a plugin directory.  The directory node's children
 * are GPluginPluginManagerTreeEntry instances for the files in that directory.
 */
static GNode *
gplugin_plugin_manager_file_tree_new(void) {
	GList *iter = NULL;
	GNode *root = NULL;

	/* read all of the files from all of our paths and store then in a tree */
	root = g_node_new(NULL);

	for(iter = paths->head; iter; iter = iter->next) {
		GDir *d = NULL;
		GError *error = NULL;
		GNode *dir = NULL;
		const gchar *path = (const gchar *)iter->data;
		const gchar *filename = NULL;

		d = g_dir_open(path, 0, &error);
		if(error) {
			g_debug("Failed to open %s: %s\n",
			        path,
			        (error->message) ? error->message : "unknown failure");

			g_error_free(error);
			error = NULL;

			continue;
		}

		/* insert the directory into the tree since we know it exists */
		dir = g_node_new(g_strdup(path));
		g_node_prepend(root, dir);

		/* now run through all of the files and add them to the tree */
		while((filename = g_dir_read_name(d)) != NULL) {
			GNode *file = NULL;
			GPluginPluginManagerTreeEntry *entry = NULL;

			entry = gplugin_plugin_manager_tree_entry_new(filename);

			file = g_node_new(entry);

			g_node_prepend(dir, file);
		}

		/* close the directory */
		g_dir_close(d);
	}

	return root;
}

static gboolean
gplugin_plugin_manager_file_tree_free_leaves(GNode *n, gpointer d) {
	GPluginPluginManagerTreeEntry *e = n->data;

	gplugin_plugin_manager_tree_entry_free(e);

	return FALSE;
}

static gboolean
gplugin_plugin_manager_file_tree_free_nonleaves(GNode *n, gpointer d) {
	g_free(n->data);

	return FALSE;
}

static void
gplugin_plugin_manager_file_tree_free(GNode *root) {
	if (root->data) {
		g_node_traverse(root, G_POST_ORDER, G_TRAVERSE_LEAVES, -1,
		                gplugin_plugin_manager_file_tree_free_leaves, NULL);

		g_node_traverse(root, G_POST_ORDER, G_TRAVERSE_NON_LEAVES, -1,
		                gplugin_plugin_manager_file_tree_free_nonleaves, NULL);
	}

	g_node_destroy(root);
}

static void
gplugin_plugin_manager_remove_list_value(gpointer data) {
	GSList *l = NULL;

	for(l = (GSList *)data; l; l = l->next)
		g_object_unref(G_OBJECT(l->data));

	g_slist_free((GSList *)data);
}

/******************************************************************************
 * Private API
 *****************************************************************************/
void
gplugin_plugin_manager_init(void) {
	paths = g_queue_new();

	plugins = g_hash_table_new_full(g_str_hash, g_str_equal, g_free,
	                                gplugin_plugin_manager_remove_list_value);

	plugins_filename_view = g_hash_table_new_full(g_str_hash, g_str_equal,
	                                              NULL, g_object_unref);

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
	loaders = g_hash_table_new_full(gplugin_plugin_manager_str_hash,
	                                g_str_equal, g_free,
	                                gplugin_plugin_manager_remove_list_value);

	gplugin_plugin_manager_register_loader(GPLUGIN_TYPE_NATIVE_PLUGIN_LOADER);
}

void
gplugin_plugin_manager_uninit(void) {
#if GLIB_CHECK_VERSION(2,32,0)
	g_queue_free_full(paths, g_free);
#else
	GList *iter = NULL;

	for(iter = paths->head; iter; iter = iter->next)
		g_free(iter->data);
	g_queue_free(paths);
#endif /* GLIB_CHECK_VERSION(2,32,0) */

	g_hash_table_destroy(plugins);
	g_hash_table_destroy(plugins_filename_view);
	g_hash_table_destroy(loaders);
}

/******************************************************************************
 * API
 *****************************************************************************/

/**
 * gplugin_plugin_manager_append_path:
 * @path: A path to add to the end of the plugin search paths
 *
 * Adds @path to the end of the list of paths to search for plugins.
 */
void
gplugin_plugin_manager_append_path(const gchar *path) {
	GList *l = NULL;

	if(!path)
		return;

	for(l = paths->head; l; l = l->next)
		if(strcmp(l->data, path) == 0)
			return;

	g_queue_push_tail(paths, g_strdup(path));
}

/**
 * gplugin_plugin_manager_prepend_path:
 * @path: A path to add to the beginning of the plugin search paths
 *
 * Adds @path to the beginning of the list of paths to search for plugins.
 */
void
gplugin_plugin_manager_prepend_path(const gchar *path) {
	GList *l = NULL;

	if(!path)
		return;

	for(l = paths->head; l; l = l->next)
		if(strcmp(l->data, path) == 0)
			return;

	g_queue_push_head(paths, g_strdup(path));
}

/**
 * gplugin_plugin_manager_remove_path:
 * @path: A path to remove from the plugin search paths
 *
 * Removes @path from the list of paths to search for plugins.
 */
void
gplugin_plugin_manager_remove_path(const gchar *path) {
	GList *l = NULL, *link = NULL;

	g_return_if_fail(path != NULL);

	for(l = paths->head; l; l = l->next)
		if(strcmp(l->data, path) == 0) {
			g_free(l->data);
			link = l;
		}

	if(link)
		g_queue_delete_link(paths, link);
}

/**
 * gplugin_plugin_manager_add_default_paths:
 *
 * Adds the path that GPlugin was installed to to the plugin search path, as
 * well as ${XDG_CONFIG_HOME}/gplugin.
 */
void
gplugin_plugin_manager_add_default_paths(void) {
	gchar *path;

	path = g_build_filename(PREFIX, "lib", "gplugin", NULL);
	gplugin_plugin_manager_prepend_path(path);
	g_free(path);

	path = g_build_filename(g_get_user_config_dir(), "gplugin", NULL);
	gplugin_plugin_manager_prepend_path(path);
	g_free(path);
}


/**
 * gplugin_plugin_manager_add_app_paths:
 * @prefix: The installation prefix for the application.
 * @appname: The name of the application whose paths to add.
 *
 * Adds the application installation path for @appname.  This will add
 * $prefix/@appname/plugins to the list as well as
 * ${XDG_CONFIG_HOME}/@appname/plugins.
 */
void
gplugin_plugin_manager_add_app_paths(const gchar *prefix,
                                     const gchar *appname)
{
	gchar *path;

	g_return_if_fail(appname != NULL);

	path = g_build_filename(prefix, appname, NULL);
	gplugin_plugin_manager_prepend_path(path);
	g_free(path);

	path = g_build_filename(g_get_user_config_dir(), appname, "plugins", NULL);
	gplugin_plugin_manager_prepend_path(path);
	g_free(path);
}

/**
 * gplugin_plugin_manager_get_paths:
 *
 * Gets the list of paths which will be search for plugins.
 *
 * Return value: (element-type utf8) (transfer none): list of paths which will
 *               be searched for plugins.  free the list with g_list_free when
 *               done.
 */
GList *
gplugin_plugin_manager_get_paths(void) {
	return paths->head;
}

/**
 * gplugin_plugin_manager_register_loader:
 * @type: #GType of a #GPluginPluginLoader
 *
 * Registers @type as an available loader.
 */
void
gplugin_plugin_manager_register_loader(GType type) {
	GPluginPluginLoader *loader = NULL;
	GPluginPluginLoaderClass *lo_class = NULL;
	GSList *l = NULL;

	g_return_if_fail(g_type_is_a(type, GPLUGIN_TYPE_PLUGIN_LOADER));

	/* Create the loader instance first.  If we can't create it, we bail */
	loader = g_object_new(type, NULL);
	if(!GPLUGIN_IS_PLUGIN_LOADER(loader)) {
		g_warning("failed to create loader instance for %s",
		          g_type_name(type));

		return;
	}

	/* grab the class of the loader */
	lo_class = GPLUGIN_PLUGIN_LOADER_GET_CLASS(loader);
	if(!lo_class) {
		g_object_unref(G_OBJECT(loader));

		return;
	}

	for(l = lo_class->supported_extensions; l; l = l->next) {
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

		existing = g_slist_prepend(existing, g_object_ref(G_OBJECT(loader)));

		/* Now insert the updated slist back into the hash table */
		g_hash_table_insert(loaders, g_strdup(ext), existing);
	}

	/* make a note that we need to refresh */
	refresh_needed = TRUE;

	/* we remove our initial reference from the loader now to avoid a leak */
	g_object_unref(G_OBJECT(loader));
}

/**
 * gplugin_plugin_manager_unregister_loader:
 * @type: #GType of a #GPluginPluginLoader
 *
 * Unregisters @type as an available loader.
 */
void
gplugin_plugin_manager_unregister_loader(GType type) {
	GPluginPluginLoaderClass *klass = NULL;
	GSList *exts = NULL;

	g_return_if_fail(g_type_is_a(type, GPLUGIN_TYPE_PLUGIN_LOADER));

	klass = g_type_class_ref(type);
	if(!klass)
		return;

	for(exts = klass->supported_extensions; exts; exts = exts->next) {
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

	g_type_class_unref(klass);
}

/**
 * gplugin_plugin_manager_refresh:
 *
 * Forces a refresh of all plugins found in the search paths.
 */
void
gplugin_plugin_manager_refresh(void) {
	GNode *root = NULL;

	/* build a tree of all possible plugins */
	root = gplugin_plugin_manager_file_tree_new();

	refresh_needed = TRUE;

	while(refresh_needed) {
		GNode *dir = NULL;

		refresh_needed = FALSE;

		for(dir = root->children; dir; dir = dir->next) {
			GNode *file = NULL;
			const gchar *path = (const gchar *)dir->data;

			for(file = dir->children; file; file = file->next) {
				GPluginPlugin *plugin = NULL;
				GPluginPluginManagerTreeEntry *e = NULL;
				GError *error = NULL;
				GSList *l = NULL;
				gchar *filename = NULL;

				e = (GPluginPluginManagerTreeEntry *)file->data;

				/* Build the path and see if we need to probe it! */
				filename = g_build_filename(path, e->filename, NULL);
				plugin = g_hash_table_lookup(plugins_filename_view, filename);

				if(plugin && GPLUGIN_IS_PLUGIN(plugin)) {
					GPluginPluginState state =
						gplugin_plugin_get_state(plugin);

					/* The plugin is in our "view", check it's state.  If it's
					 * probed or loaded, move on to the next one.
					 */
					if(state == GPLUGIN_PLUGIN_STATE_QUERIED ||
					   state == GPLUGIN_PLUGIN_STATE_LOADED)
					{
						g_free(filename);
						continue;
					}
				}

				/* grab the list of loaders for this extension */
				for(l = g_hash_table_lookup(loaders, e->extension); l;
				    l = l->next)
				{
					GPluginPluginLoader *loader = NULL;

					if(!l->data)
						continue;

					loader = GPLUGIN_PLUGIN_LOADER(l->data);
					if(!GPLUGIN_IS_PLUGIN_LOADER(loader))
						continue;

					/* Try to probe the plugin with the current loader */
					plugin = gplugin_plugin_loader_query_plugin(loader,
					                                            filename,
				                                                &error);

					/* Check the GError, if it's set, output it's message and
					 * try the next loader.
					 */
					if(error) {
						g_warning("failed to query '%s' with loader '%s': %s",
					              filename, G_OBJECT_TYPE_NAME(loader),
						          error->message ? error->message : "Unknown");

						g_error_free(error);
						error = NULL;

						continue;
					}

					/* if the plugin instance is good, then break out of this
					 * loop.
					 */
					if(plugin != NULL && GPLUGIN_IS_PLUGIN(plugin))
						break;

					g_object_unref(G_OBJECT(plugin));
				}

				/* check if our plugin instance is good.  If it's not good we
				 * don't need to do anything but free the filename which we'll
				 * do later.
				 */
				if(plugin != NULL && GPLUGIN_IS_PLUGIN(plugin)) {
					/* we have a good plugin, huzzah!  We need to add it to our
					 * "view" as well as the main plugin hash table.
					 */

					/* we want the internal filename from the plugin to avoid
					 * duplicate memory, so we need to grab it for the "view".
					 */
					gchar *real_filename =
						gplugin_plugin_get_internal_filename(plugin);

					/* we also need the GPluginPluginInfo to get the plugin's
					 * ID for the key in our main hash table.
					 */
					const GPluginPluginInfo *info =
						gplugin_plugin_get_info(plugin);

					const gchar *id = gplugin_plugin_info_get_id(info);
					GSList *l = NULL;

					/* throw a warning if the info->id is NULL */
					if(id == NULL)
						g_warning("Plugin %s has a NULL id.",  real_filename);

					/* now insert into our view */
					g_hash_table_insert(plugins_filename_view, real_filename,
					                    g_object_ref(G_OBJECT(plugin)));

					/* Grab the list of plugins with our id and prepend the new
					 * plugin to it before updating it.
					 */
					l = g_hash_table_lookup(plugins, id);
					l = g_slist_prepend(l, g_object_ref(plugin));
					g_hash_table_insert(plugins, g_strdup(id), l);

					g_object_unref(G_OBJECT(info));

					/* finally set the plugin state queried */
					gplugin_plugin_set_state(plugin, GPLUGIN_PLUGIN_STATE_QUERIED);
				}

				g_free(filename);
			}
		}
	}

	/* free the file tree */
	gplugin_plugin_manager_file_tree_free(root);
}

/**
 * gplugin_plugin_manager_find_plugins:
 * @id: id string of the plugin to find
 *
 * Finds all plugins matching @id.
 *
 * Return value: (element-type GPlugin.Plugin) (transfer full): A #GSList of
 *               referenced #GPluginPlugin's matching @id.  Call
 *               #gplugin_plugin_manager_free_plugin_list on the returned value
 *               when you're done with it.
 */
GSList *
gplugin_plugin_manager_find_plugins(const gchar *id) {
	GSList *plugins_list = NULL, *l;

	g_return_val_if_fail(id != NULL, NULL);

	for(l = g_hash_table_lookup(plugins, id); l; l = l->next) {
		GPluginPlugin *plugin = NULL;

		if(l->data == NULL)
			continue;

		plugin = GPLUGIN_PLUGIN(l->data);

		plugins_list = g_slist_prepend(plugins_list,
		                               g_object_ref(G_OBJECT(plugin)));
	}

	return plugins_list;
}

/**
 * gplugin_plugin_manager_free_plugin_list:
 * @plugins_list: (element-type GPlugin.Plugin): Returned value from
 *                #gplugin_plugin_manager_find_plugins
 *
 * Frees the return value of #gplugin_plugin_manager_find_plugins.
 */
void
gplugin_plugin_manager_free_plugin_list(GSList *plugins_list) {
	GSList *l = NULL;

	g_return_if_fail(plugins_list != NULL);

	for(l = plugins_list; l; l = l->next) {
		GPluginPlugin *plugin = NULL;

		if(l->data == NULL)
			continue;

		plugin = GPLUGIN_PLUGIN(l->data);

		g_object_unref(G_OBJECT(plugin));
	}

	g_slist_free(plugins_list);
}

/**
 * gplugin_plugin_manager_find_plugin:
 * @id: The id of the plugin to find.
 *
 * Finds the first plugin matching @id.  This function uses
 * #gplugin_plugin_manager_find_plugins and returns the first plugin in the
 * list.
 *
 * Return value: (transfer full): A referenced #GPluginPlugin instance or NULL
 *               if no plugin matching @id was found.
 */
GPluginPlugin *
gplugin_plugin_manager_find_plugin(const gchar *id) {
	GSList *plugins_list = NULL;
	GPluginPlugin *plugin = NULL;

	g_return_val_if_fail(id != NULL, NULL);

	plugins_list = gplugin_plugin_manager_find_plugins(id);
	if(plugins_list == NULL)
		return NULL;

	plugin = g_object_ref(G_OBJECT(plugins_list->data));

	gplugin_plugin_manager_free_plugin_list(plugins_list);

	return plugin;
}


/**
 * gplugin_plugin_manager_load_plugin:
 * @plugin: #GPluginPlugin instance
 * @error: (out): return location for a #GError or null
 *
 * Loads @plugin and all of it's dependencies.  If a dependency can not be
 * loaded, @plugin will not be loaded either.  However, any other plugins that
 * @plugin depends on that were loaded from this call, will not be unloaded.
 *
 * Return value: TRUE if @plugin was loaded successfully or already loaded,
 *               FALSE otherwise.
 */
gboolean
gplugin_plugin_manager_load_plugin(GPluginPlugin *plugin, GError **error) {
	const GPluginPluginInfo *info = NULL;
	GPluginPluginLoader *loader = NULL;
	const gchar * const *dependencies = NULL;
	gint i = 0;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN(plugin), FALSE);

	/* if the plugin is already loaded there's nothing for us to do */
	if(gplugin_plugin_get_state(plugin) == GPLUGIN_PLUGIN_STATE_LOADED)
		return TRUE;

	/* now try to get the plugin info from the plugin */
	info = gplugin_plugin_get_info(plugin);
	if(info == NULL) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "Plugin %s did not return value plugin info",
			                     gplugin_plugin_get_filename(plugin));
		}

		return FALSE;
	}

	/* now walk through any dependencies the plugin has and load them.  If they
	 * fail to load we need to fail as well.
	 */
	dependencies = gplugin_plugin_info_get_dependencies(info);
	if(dependencies != NULL) {
		for(i = 0; dependencies[i]; i++) {
			GSList *matches = NULL, *m = NULL;
			gboolean ret = FALSE;

			matches = gplugin_plugin_manager_find_plugins(dependencies[i]);

			/* make sure we got at least 1 match */
			if(matches == NULL) {
				if(error) {
					*error = g_error_new(GPLUGIN_DOMAIN, 0,
					                     "Failed to find plugin %s which %s "
					                     "depends on",
					                     dependencies[i],
					                     gplugin_plugin_get_filename(plugin));
				}

				g_object_unref(G_OBJECT(info));

				return FALSE;
			}

			for(m = matches; m; m = m->next) {
				GPluginPlugin *plugin = g_object_ref(G_OBJECT(m->data));

				ret = gplugin_plugin_manager_load_plugin(plugin, error);

				g_object_unref(G_OBJECT(plugin));

				if(ret == TRUE)
					break;
			}

			if(ret == FALSE) {
				if(error) {
					if (*error == NULL) {
						*error = g_error_new(GPLUGIN_DOMAIN, 0,
							                 "Plugin did not give a reason.");
					}

					g_prefix_error(error,
					               "Found at least one dependency with the id %s, "
					               "but failed to load it: ", dependencies[i]);
				}

				g_object_unref(G_OBJECT(info));

				return FALSE;
			}
		}
	}

	g_object_unref(G_OBJECT(info));

	/* now load the actual plugin */
	loader = gplugin_plugin_get_loader(plugin);

	if(!GPLUGIN_IS_PLUGIN_LOADER(loader)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "The loader for %s is not a loader.  This "
			                     "should not happend!",
			                     gplugin_plugin_get_filename(plugin));
		}

		return FALSE;
	}

	return gplugin_plugin_loader_load_plugin(loader, plugin, error);
}

/**
 * gplugin_plugin_manager_unload_plugin:
 * @plugin: #GPluginPlugin instance
 * @error: (out): return location for a #GError or null
 *
 * Unloads @plugin.  If @plugin has dependencies, they are not unloaded.
 *
 * Return value: TRUE if @plugin was unloaded successfully or not loaded,
 *               FALSE otherwise.
 */
gboolean
gplugin_plugin_manager_unload_plugin(GPluginPlugin *plugin, GError **error) {
	GPluginPluginLoader *loader = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN(plugin), FALSE);

	if(gplugin_plugin_get_state(plugin) != GPLUGIN_PLUGIN_STATE_LOADED)
		return TRUE;

	loader = gplugin_plugin_get_loader(plugin);
	if(!GPLUGIN_IS_PLUGIN_LOADER(loader)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "Plugin loader is not a loader");
		}

		return FALSE;
	}

	return gplugin_plugin_loader_unload_plugin(loader, plugin, error);
}

/**
 * gplugin_plugin_manager_list_plugins:
 *
 * Returns a #GList of all plugin id's.  Each id should be queried directly
 * for more information.
 *
 * Return value: (element-type utf8) (transfer full): A #GList of each unique
 *               plugin id.
 */
GList *
gplugin_plugin_manager_list_plugins(void) {
	GQueue *queue = g_queue_new();
	GList *ret = NULL;
	GHashTableIter iter;
	gpointer key = NULL;

	g_hash_table_iter_init(&iter, plugins);
	while(g_hash_table_iter_next(&iter, &key, NULL)) {
		g_queue_push_tail(queue, (gchar *)key);
	}

	ret = g_list_copy(queue->head);

	g_queue_free(queue);

	return ret;
}

