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
test_gplugin_native_plugin_loader(void) {
	GSList *plugins = NULL, *l = NULL;

	/* add the test directory to the plugin manager's search paths */
	gplugin_plugin_manager_add_path(TEST_DIR);

	/* refresh the plugin manager */
	gplugin_plugin_manager_refresh();

	/* now look for the plugin */
	plugins = gplugin_plugin_manager_find_plugins("test-native-plugin");
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

		g_assert_cmpstr(info->id, ==, "test-native-plugin");
		g_assert_cmpint(info->abi_version, ==,
		                GPLUGIN_NATIVE_PLUGIN_ABI_VERSION);
		g_assert_cmpuint(info->flags, ==, 0);
		g_assert_cmpstr(info->name, ==, "test plugin name");
		g_assert_cmpstr(info->version, ==, "test plugin version");
		g_assert_cmpstr(info->summary, ==, "test plugin summary");
		g_assert_cmpstr(info->description, ==, "test plugin description");
		g_assert_cmpstr(info->author, ==, "test plugin author");
		g_assert_cmpstr(info->website, ==, "test plugin website");
	}

	/* make sure the free function works too */
	gplugin_plugin_manager_free_plugin_list(plugins);
}

gint
main(gint argc, gchar **argv) {

	g_test_init(&argc, &argv, NULL);

	gplugin_init();

	g_test_add_func("/loaders/native/load", test_gplugin_native_plugin_loader);

	return g_test_run();
}

