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

		g_assert_cmpstr(info->id, ==, "basic-native-plugin");
		g_assert_cmpint(info->abi_version, ==,
		                GPLUGIN_NATIVE_PLUGIN_ABI_VERSION);
		g_assert_cmpuint(info->flags, ==, 0);
		g_assert_cmpstr(info->name, ==, "name");
		g_assert_cmpstr(info->version, ==, "version");
		g_assert_cmpstr(info->summary, ==, "summary");
		g_assert_cmpstr(info->description, ==, "description");
		g_assert_cmpstr(info->author, ==, "author");
		g_assert_cmpstr(info->website, ==, "website");

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

gint
main(gint argc, gchar **argv) {

	g_test_init(&argc, &argv, NULL);

	gplugin_init();

	g_test_add_func("/loaders/native/load", test_basic_plugin_load);
	g_test_add_func("/loaders/native/load_dependent", test_dependent_plugin_load);

	return g_test_run();
}

