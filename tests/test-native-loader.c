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

	/* add the test directory to the plugin manager's search paths */
	gplugin_plugin_manager_append_path(TEST_DIR);

	/* refresh the plugin manager */
	gplugin_plugin_manager_refresh();

	/* now look for the plugin */
	plugins = gplugin_plugin_manager_find_plugins("basic-native-plugin");
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

		g_assert_cmpuint(G_OBJECT_TYPE(info), ==,
		                 test_gplugin_plugin_info_get_type());

		g_assert_cmpstr(gplugin_plugin_info_get_id(info), ==,
		                "basic-native-plugin");
		g_assert_cmpint(gplugin_plugin_info_get_abi_version(info), ==,
		                GPLUGIN_NATIVE_PLUGIN_ABI_VERSION);
		g_assert_cmpuint(gplugin_plugin_info_get_flags(info), ==, 0);
		g_assert_cmpstr(gplugin_plugin_info_get_name(info), ==, "name");
		g_assert_cmpstr(gplugin_plugin_info_get_version(info), ==, "version");
		g_assert_cmpstr(gplugin_plugin_info_get_summary(info), ==, "summary");
		g_assert_cmpstr(gplugin_plugin_info_get_description(info), ==,
		                "description");
		g_assert_cmpstr(gplugin_plugin_info_get_author(info), ==, "author");
		g_assert_cmpstr(gplugin_plugin_info_get_website(info), ==, "website");

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
	parent = gplugin_plugin_manager_find_plugin("basic-native-plugin");
	g_assert(parent != NULL);

	state = gplugin_plugin_get_state(parent);
	g_assert_cmpint(state, !=, GPLUGIN_PLUGIN_STATE_LOADED);

	/* find the dependent plugin and make sure it isn't loaded */
	dependent = gplugin_plugin_manager_find_plugin("dependent-native-plugin");
	g_assert(dependent != NULL);

	state = gplugin_plugin_get_state(dependent);
	g_assert_cmpint(state, !=, GPLUGIN_PLUGIN_STATE_LOADED);

	/* now load the dependent plugin */
	g_assert(gplugin_plugin_manager_load_plugin(dependent, &error));

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
		gplugin_plugin_manager_find_plugin("broken-dependent-native-plugin");
	g_assert(plugin != NULL);

	state = gplugin_plugin_get_state(plugin);
	g_assert_cmpint(state, !=, GPLUGIN_PLUGIN_STATE_LOADED);

	/* now attempt to load the dependent plugin, it's supposed to fail */
	g_assert(!gplugin_plugin_manager_load_plugin(plugin, &error));
}

gint
main(gint argc, gchar **argv) {

	g_test_init(&argc, &argv, NULL);

	gplugin_init_with_args(test_gplugin_plugin_info_get_type());

	g_test_add_func("/loaders/native/load", test_basic_plugin_load);
	g_test_add_func("/loaders/native/load_dependent",
	                test_dependent_plugin_load);
	g_test_add_func("/loaders/native/load_broken_dependent",
	                test_broken_depend_plugin_load);

	return g_test_run();
}

