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

#include <stdlib.h>

#include <gplugin.h>
#include <gplugin-native.h>

#include <glib.h>

/******************************************************************************
 * GTypes
 *****************************************************************************/
typedef struct {
	GPluginPluginInfo parent;
} TestGPluginPluginInfo;

typedef struct {
	GPluginPluginInfoClass parent;
} TestGPluginPluginInfoClass;

G_DEFINE_TYPE(TestGPluginPluginInfo, test_gplugin_plugin_info, GPLUGIN_TYPE_PLUGIN_INFO);

static void
test_gplugin_plugin_info_init(TestGPluginPluginInfo *info) {
}

static void
test_gplugin_plugin_info_class_init(TestGPluginPluginInfoClass *klass) {
}

/******************************************************************************
 * Tests
 *****************************************************************************/
static void
test_basic_plugin_load(void) {
	GSList *plugins = NULL, *l = NULL;
	GError *error = NULL;
	const gchar * const *strv = NULL;

	/* add the test directory to the plugin manager's search paths */
	gplugin_plugin_manager_append_path(TEST_DIR);

	/* refresh the plugin manager */
	gplugin_plugin_manager_refresh();

	/* now look for the plugin */
	plugins = gplugin_plugin_manager_find_plugins("gplugin/basic-native-plugin");
	g_assert(plugins != NULL);

	/* now iterate through the plugins (we really only should have one...) */
	for(l = plugins; l; l = l->next) {
		GPluginPlugin *plugin = GPLUGIN_PLUGIN(l->data);
		const GPluginPluginInfo *info = NULL;
		GPluginPluginState state = GPLUGIN_PLUGIN_STATE_UNKNOWN;

		state = gplugin_plugin_get_state(plugin);
		g_assert_cmpint(state, ==, GPLUGIN_PLUGIN_STATE_QUERIED);

		info = gplugin_plugin_get_info(plugin);
		g_assert(info != NULL);

		g_assert_cmpuint(G_OBJECT_TYPE(info), ==, GPLUGIN_TYPE_PLUGIN_INFO);

		g_assert_cmpstr(gplugin_plugin_info_get_id(info), ==,
		                "gplugin/basic-native-plugin");
		g_assert_cmpint(gplugin_plugin_info_get_abi_version(info), ==,
		                GPLUGIN_NATIVE_PLUGIN_ABI_VERSION);
		g_assert(gplugin_plugin_info_get_internal(info) == FALSE);
		g_assert(gplugin_plugin_info_get_load_on_query(info) == FALSE);
		g_assert_cmpstr(gplugin_plugin_info_get_name(info), ==, "name");
		g_assert_cmpstr(gplugin_plugin_info_get_version(info), ==, "version");
		g_assert_cmpstr(gplugin_plugin_info_get_summary(info), ==, "summary");
		g_assert_cmpstr(gplugin_plugin_info_get_description(info), ==,
		                "description");
		strv = gplugin_plugin_info_get_authors(info);
		g_assert_cmpstr(strv[0], ==, "author");
		g_assert_cmpstr(gplugin_plugin_info_get_website(info), ==, "website");

		g_object_unref(G_OBJECT(info));

		g_assert(gplugin_plugin_manager_load_plugin(plugin, &error));

		state = gplugin_plugin_get_state(plugin);
		g_assert_cmpint(state, ==, GPLUGIN_PLUGIN_STATE_LOADED);

		g_assert(gplugin_plugin_manager_unload_plugin(plugin, &error));

		state = gplugin_plugin_get_state(plugin);
		g_assert_cmpint(state, !=, GPLUGIN_PLUGIN_STATE_LOADED);
	}

	/* make sure the free function works too */
	gplugin_plugin_manager_free_plugin_list(plugins);
}

static void
test_dependent_plugin_load(void) {
	GPluginPlugin *dependent = NULL, *parent = NULL;
	GPluginPluginState state;
	GError *error = NULL;

	/* add the test directory to the plugin manager's search paths */
	gplugin_plugin_manager_append_path(TEST_DIR);

	/* refresh the plugin manager */
	gplugin_plugin_manager_refresh();

	/* find the parent plugin and make sure it isn't loaded */
	parent = gplugin_plugin_manager_find_plugin("gplugin/basic-native-plugin");
	g_assert(parent != NULL);

	state = gplugin_plugin_get_state(parent);
	g_assert_cmpint(state, !=, GPLUGIN_PLUGIN_STATE_LOADED);

	/* find the dependent plugin and make sure it isn't loaded */
	dependent = gplugin_plugin_manager_find_plugin("gplugin/dependent-native-plugin");
	g_assert(dependent != NULL);

	state = gplugin_plugin_get_state(dependent);
	g_assert_cmpint(state, !=, GPLUGIN_PLUGIN_STATE_LOADED);

	/* now load the dependent plugin */
	g_assert(gplugin_plugin_manager_load_plugin(dependent, &error));
	g_assert_no_error(error);

	/* make sure the parent plugin got loaded too */
	state = gplugin_plugin_get_state(parent);
	g_assert_cmpint(state, ==, GPLUGIN_PLUGIN_STATE_LOADED);
}

static void
test_broken_depend_plugin_load(void) {
	GPluginPlugin *plugin = NULL;
	GPluginPluginState state;
	GError *error = NULL;

	/* add the test directory to the plugin manager's search paths */
	gplugin_plugin_manager_append_path(TEST_DIR);

	/* refresh the plugin manager */
	gplugin_plugin_manager_refresh();

	/* find the dependent plugin and make sure it isn't loaded */
	plugin =
		gplugin_plugin_manager_find_plugin("gplugin/broken-dependent-native-plugin");
	g_assert(plugin != NULL);

	state = gplugin_plugin_get_state(plugin);
	g_assert_cmpint(state, !=, GPLUGIN_PLUGIN_STATE_LOADED);

	/* now attempt to load the dependent plugin, it's supposed to fail */
	g_assert(!gplugin_plugin_manager_load_plugin(plugin, &error));
}

/******************************************************************************
 * Test bad plugins
 *****************************************************************************/
static void
test_query_error(void) {
	GPluginPlugin *plugin = NULL;

	if(g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDERR)) {
		/* add the test directory to the plugin manager's search paths */
		gplugin_plugin_manager_append_path(TEST_BAD_DIR);

		/* refresh the plugin manager */
		gplugin_plugin_manager_refresh();

		/* find the query-error plugin */
		plugin = gplugin_plugin_manager_find_plugin("gplugin/query-error");
		g_assert(plugin == NULL);
	}

	g_test_trap_assert_failed();
}

static void
test_load_error(void) {
	GPluginPlugin *plugin = NULL;
	GError *error;

	if(g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDERR)) {
		/* add the test directory to the plugin manager's search paths */
		gplugin_plugin_manager_append_path(TEST_BAD_DIR);

		/* refresh the plugin manager */
		gplugin_plugin_manager_refresh();

		/* find the query-error plugin */
		plugin = gplugin_plugin_manager_find_plugin("gplugin/load-error");
		g_assert(plugin == NULL);

		g_assert(gplugin_plugin_manager_load_plugin(plugin, &error));
		g_assert_no_error(error);
	}

	g_test_trap_assert_failed();
}

static void
test_unload_error(void) {
	GPluginPlugin *plugin = NULL;
	GError *error;

	if(g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDERR)) {
		/* add the test directory to the plugin manager's search paths */
		gplugin_plugin_manager_append_path(TEST_BAD_DIR);

		/* refresh the plugin manager */
		gplugin_plugin_manager_refresh();

		/* find the query-error plugin */
		plugin = gplugin_plugin_manager_find_plugin("gplugin/unload-error");
		g_assert(plugin == NULL);

		g_assert(!gplugin_plugin_manager_load_plugin(plugin, &error));
		g_assert_error(error, GPLUGIN_DOMAIN, 0);

		g_assert(gplugin_plugin_manager_unload_plugin(plugin, &error));
		g_assert_no_error(error);
	}

	g_test_trap_assert_failed();
}

/* id collisions */
static void
test_id_collision(void) {
	GSList *plugins = NULL, *l = NULL;

	gplugin_plugin_manager_append_path(TEST_ID_DIR);
	gplugin_plugin_manager_refresh();

	plugins = gplugin_plugin_manager_find_plugins("gplugin/id-collision");
	g_assert(plugins);
}

/* load on query */
static void
test_load_on_query(void) {
	GPluginPlugin *plugin = NULL;

	gplugin_plugin_manager_append_path(TEST_DIR);
	gplugin_plugin_manager_refresh();

	plugin = gplugin_plugin_manager_find_plugin("gplugin/load-on-query");
	g_assert(plugin);

	g_assert_cmpint(gplugin_plugin_get_state(plugin), ==,
	                GPLUGIN_PLUGIN_STATE_LOADED);
}

static void
test_load_on_query_fail(void) {
	GPluginPlugin *plugin = NULL;

	if(g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDERR)) {
		gplugin_plugin_manager_append_path(TEST_LOAD_DIR);
		gplugin_plugin_manager_refresh();

		plugin = gplugin_plugin_manager_find_plugin("gplugin/load-on-query-fail");
		g_assert(plugin);

		g_assert_cmpint(gplugin_plugin_get_state(plugin), ==,
		                GPLUGIN_PLUGIN_STATE_LOAD_FAILED);
	}

	g_test_trap_assert_failed();
}

/******************************************************************************
 * Main
 *****************************************************************************/
gint
main(gint argc, gchar **argv) {

	g_test_init(&argc, &argv, NULL);

	g_log_set_fatal_mask(g_quark_to_string(GPLUGIN_DOMAIN), 0);

	gplugin_init();

	g_test_add_func("/loaders/native/load", test_basic_plugin_load);
	g_test_add_func("/loaders/native/load_dependent",
	                test_dependent_plugin_load);
	g_test_add_func("/loaders/native/load_broken_dependent",
	                test_broken_depend_plugin_load);

	/* bad plugin tests */
	g_test_add_func("/loaders/native/query-error",
	                test_query_error);
	g_test_add_func("/loaders/native/load-error",
	                test_load_error);
	g_test_add_func("/loaders/native/unload-error",
	                test_unload_error);

	/* test plugins with id collisions */
	g_test_add_func("/loaders/native/id-collision",
	                test_id_collision);

	/* test the load on query flag */
	g_test_add_func("/loaders/native/load-on-query",
	                test_load_on_query);
	g_test_add_func("/loaders/native/load-on-query-fail",
	                test_load_on_query_fail);

	return g_test_run();
}

