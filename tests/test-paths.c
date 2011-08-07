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

#include <gidbits.h>

#include <glib.h>

#define test_path_count(e) G_STMT_START { \
	GList *paths = gidbits_plugin_manager_get_paths(); \
	g_assert_cmpint(g_list_length(paths), ==, (e)); \
} G_STMT_END

/******************************************************************************
 * Tests
 *****************************************************************************/
static void
test_gidbits_plugin_manager_paths_single(void) {
	gidbits_plugin_manager_add_path("foo");
	test_path_count(1);

	gidbits_plugin_manager_remove_path("foo");
	test_path_count(0);
}

static void
test_gidbits_plugin_manager_paths_duplicate(void) {
	gidbits_plugin_manager_add_path("foo");
	gidbits_plugin_manager_add_path("foo");

	test_path_count(1);

	gidbits_plugin_manager_remove_path("foo");
	test_path_count(0);
}

static void
test_gidbits_plugin_manager_paths_multiple_fifo(void) {
	/* add */
	gidbits_plugin_manager_add_path("foo");
	test_path_count(1);

	gidbits_plugin_manager_add_path("bar");
	test_path_count(2);

	/* remove */
	gidbits_plugin_manager_remove_path("foo");
	test_path_count(1);

	gidbits_plugin_manager_remove_path("bar");
	test_path_count(0);
}

static void
test_gidbits_plugin_manager_paths_multiple_filo(void) {
	/* add */
	gidbits_plugin_manager_add_path("foo");
	test_path_count(1);

	gidbits_plugin_manager_add_path("bar");
	test_path_count(2);

	/* remove */
	gidbits_plugin_manager_remove_path("bar");
	test_path_count(1);

	gidbits_plugin_manager_remove_path("foo");
	test_path_count(0);
}

gint
main(gint argc, gchar **argv) {

	g_test_init(&argc, &argv, NULL);

	gidbits_init();

	g_test_add_func("/plugins/paths/add_remove_single",
	                test_gidbits_plugin_manager_paths_single);

	g_test_add_func("/plugins/paths/add_remove_duplicate",
	                test_gidbits_plugin_manager_paths_duplicate);

	g_test_add_func("/plugins/paths/add_remove_multiple_fifo",
	                test_gidbits_plugin_manager_paths_multiple_fifo);

	g_test_add_func("/plugins/paths/add_remove_multiple_filo",
	                test_gidbits_plugin_manager_paths_multiple_filo);

	return g_test_run();
}

