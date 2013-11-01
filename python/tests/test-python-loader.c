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

#include <glib.h>
#include <gplugin.h>

#include <stdio.h>

static void
test_full(void) {
	GPluginPlugin *plugin = NULL;
	GPluginPluginInfo *info = NULL;
	GError *error = NULL;
	const gchar *id = "gplugin-python/basic-plugin";
	gchar **authors = NULL;
	const gchar * const r_authors[] = { "author1", NULL };
	gint i;

	plugin = gplugin_manager_find_plugin(id);
	g_assert(plugin != NULL);

	info = gplugin_plugin_get_info(plugin);
	g_assert(info != NULL);

	g_assert_cmpstr(id, ==, gplugin_plugin_info_get_id(info));
	g_assert_cmpuint(0x01020304, ==,
	                 gplugin_plugin_info_get_abi_version(info));
	g_assert_cmpstr("basic plugin", ==, gplugin_plugin_info_get_name(info));

	authors = (gchar **)gplugin_plugin_info_get_authors(info);
	for(i = 0; r_authors[i]; i++)
		g_assert_cmpstr(authors[i], ==, r_authors[i]);

	g_assert_cmpstr("test", ==, gplugin_plugin_info_get_category(info));
	g_assert_cmpstr("version", ==, gplugin_plugin_info_get_version(info));
	g_assert_cmpstr("license", ==, gplugin_plugin_info_get_license_id(info));
	g_assert_cmpstr("summary", ==, gplugin_plugin_info_get_summary(info));
	g_assert_cmpstr("description", ==,
	                gplugin_plugin_info_get_description(info));
	g_assert_cmpstr("website", ==, gplugin_plugin_info_get_website(info));

	g_object_unref(G_OBJECT(info));

	g_assert_cmpint(GPLUGIN_PLUGIN_STATE_QUERIED, ==,
	                gplugin_plugin_get_state(plugin));

	gplugin_manager_load_plugin(plugin, &error);
	g_assert_no_error(error);
	g_assert_cmpint(GPLUGIN_PLUGIN_STATE_LOADED, ==,
	                gplugin_plugin_get_state(plugin));

	gplugin_manager_unload_plugin(plugin, &error);
	g_assert_no_error(error);
	g_assert_cmpint(GPLUGIN_PLUGIN_STATE_QUERIED, ==,
	                gplugin_plugin_get_state(plugin));

	g_object_unref(G_OBJECT(plugin));
}

static void
test_load_failed(void) {
	GPluginPlugin *plugin = NULL;
	GError *error = NULL;
	gboolean ret = FALSE;

	plugin = gplugin_manager_find_plugin("gplugin-python/load-failed");
	g_assert(plugin != NULL);

	ret = gplugin_manager_load_plugin(plugin, &error);
	g_assert(ret == FALSE);
	g_assert_error(error, GPLUGIN_DOMAIN, 0);
	g_assert_cmpint(gplugin_plugin_get_state(plugin), ==,
	                GPLUGIN_PLUGIN_STATE_LOAD_FAILED);

	g_object_unref(G_OBJECT(plugin));
}

static void
test_load_exception(void) {
	GPluginPlugin *plugin = NULL;
	GError *error = NULL;
	gboolean ret = FALSE;

	plugin = gplugin_manager_find_plugin("gplugin-python/load-exception");
	g_assert(plugin != NULL);

	ret = gplugin_manager_load_plugin(plugin, &error);
	g_assert(ret == FALSE);
	g_assert_error(error, GPLUGIN_DOMAIN, 0);
	g_assert_cmpint(gplugin_plugin_get_state(plugin), ==,
	                GPLUGIN_PLUGIN_STATE_LOAD_FAILED);

	g_object_unref(G_OBJECT(plugin));
}

static void
test_unload_failed(void) {
	GPluginPlugin *plugin = NULL;
	GError *error = NULL;
	gboolean ret = FALSE;

	plugin = gplugin_manager_find_plugin("gplugin-python/unload-failed");
	g_assert(plugin != NULL);

	ret = gplugin_manager_load_plugin(plugin, &error);
	if(error)
		printf("error: %s\n", error->message);
	g_assert_no_error(error);
	g_assert(ret == TRUE);

	ret = gplugin_manager_unload_plugin(plugin, &error);
	g_assert(ret == FALSE);
	g_assert_error(error, GPLUGIN_DOMAIN, 0);
	g_assert_cmpint(gplugin_plugin_get_state(plugin), ==,
	                GPLUGIN_PLUGIN_STATE_LOADED);

	g_object_unref(G_OBJECT(plugin));
}

static void
test_dependencies(void) {
	GPluginPlugin *plugin = NULL;
	GPluginPluginInfo *info = NULL;
	const gchar *id = "gplugin-python/dependent-plugin";
	gchar **deps = NULL;
	const gchar * const r_deps[] = { "dep1", "dep2", NULL };
	gint i;

	plugin = gplugin_manager_find_plugin(id);
	g_assert(plugin != NULL);

	info = gplugin_plugin_get_info(plugin);
	g_assert(info != NULL);

	g_assert_cmpstr(id, ==, gplugin_plugin_info_get_id(info));
	g_assert_cmpuint(0x01020304, ==,
	                 gplugin_plugin_info_get_abi_version(info));
	g_assert_cmpstr("dependent plugin", ==, gplugin_plugin_info_get_name(info));

	deps = (gchar **)gplugin_plugin_info_get_dependencies(info);
	for(i = 0; r_deps[i]; i++)
		g_assert_cmpstr(deps[i], ==, r_deps[i]);
}

/******************************************************************************
 * Main
 *****************************************************************************/
gint
main(gint argc, gchar **argv) {
	g_test_init(&argc, &argv, NULL);

	gplugin_init();

	g_setenv("GI_TYPELIB_PATH", GI_TYPELIB_PATH, TRUE);

	gplugin_manager_append_path(PYTHON_LOADER_DIR);
	gplugin_manager_append_path(PYTHON_PLUGIN_DIR);

	gplugin_manager_refresh();

	g_test_add_func("/loaders/python/full", test_full);
	g_test_add_func("/loaders/python/load-failed", test_load_failed);
	g_test_add_func("/loaders/python/load-exception", test_load_exception);
	g_test_add_func("/loaders/python/unload-failed", test_unload_failed);
	g_test_add_func("/loaders/python/dependencies", test_dependencies);

	return g_test_run();
}

