/*
 * Copyright (C) 2011-2014 Gary Kramlich <grim@reaperworld.com>
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
 * Tests
 *****************************************************************************/
/* load on query */
static void
test_load_on_query(void) {
	GPluginPlugin *plugin = NULL;

	gplugin_manager_remove_paths();
	gplugin_manager_append_path(TEST_LOAD_ON_QUERY_PASS_DIR);
	gplugin_manager_refresh();

	plugin = gplugin_manager_find_plugin("gplugin/load-on-query");
	g_assert(plugin);
	g_assert(GPLUGIN_IS_PLUGIN(plugin));

	g_assert_cmpint(gplugin_plugin_get_state(plugin), ==,
	                GPLUGIN_PLUGIN_STATE_LOADED);
}

static void
test_load_on_query_fail(void) {
	GPluginPlugin *plugin = NULL;
	GSList *plugins = NULL, *l = NULL;

	if(g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDERR)) {
		gplugin_manager_remove_paths();
		gplugin_manager_append_path(TEST_LOAD_ON_QUERY_FAIL_DIR);
		gplugin_manager_refresh();
		gplugin_manager_refresh();

		plugin = gplugin_manager_find_plugin("gplugin/load-on-query-fail");
		g_assert(GPLUGIN_IS_PLUGIN(plugin));

		g_assert_cmpint(gplugin_plugin_get_state(plugin), ==,
		                GPLUGIN_PLUGIN_STATE_LOAD_FAILED);
	}

	g_test_trap_assert_stderr("*failed to load*during query*");

	plugins = gplugin_manager_find_plugins("gplugin/load-on-query-fail");
	for(l = plugins; l; l = l->next) {
		g_message("plugin: %p", l->data);
	}
	if(plugins)
		gplugin_manager_free_plugin_list(plugins);
}

/******************************************************************************
 * Main
 *****************************************************************************/
gint
main(gint argc, gchar **argv) {

	g_test_init(&argc, &argv, NULL);

	gplugin_init();

	/* test the load on query flag */
	g_test_add_func("/loaders/native/load-on-query/pass",
	                test_load_on_query);
	g_test_add_func("/loaders/native/load-on-query/fail",
	                test_load_on_query_fail);

	return g_test_run();
}

