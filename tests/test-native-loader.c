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

#include <gplugin.h>

#include <glib.h>

/******************************************************************************
 * Tests
 *****************************************************************************/
gint
main(gint argc, gchar **argv) {

	g_test_init(&argc, &argv, NULL);

	gplugin_init();

	gplugin_plugin_manager_add_path(TEST_DIR);

	gplugin_plugin_manager_refresh();

	return g_test_run();
}

