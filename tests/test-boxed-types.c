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
static void
test_gplugin_plugin_info(void) {
	GPluginPluginInfo *copy = NULL;
	GPluginPluginInfo info = {
		.id = "test",
		.abi_version = 1,
		.flags = GPLUGIN_PLUGIN_FLAGS_LOAD_ON_QUERY,
		.name = "testplugin",
		.version = "version",
		.summary = "summary",
		.description = "description",
		.author = "author",
		.website = "website",
		.dependencies = g_slist_prepend(NULL, "foo"),
	};

	copy = gplugin_plugin_info_copy(&info);

	/* for the string pointers, we make sure the strings match and that they
	 * are pointing to different memory.
	 */
	g_assert_cmpstr(info.id, ==, copy->id);
	g_assert(info.id != copy->id);

	g_assert(info.abi_version == copy->abi_version);
	g_assert(info.flags == copy->flags);

	g_assert_cmpstr(info.name, ==, copy->name);
	g_assert(info.name != copy->name);

	g_assert_cmpstr(info.version, ==, copy->version);
	g_assert(info.version != copy->version);

	g_assert_cmpstr(info.summary, ==, copy->summary);
	g_assert(info.summary != copy->summary);

	g_assert_cmpstr(info.description, ==, copy->description);
	g_assert(info.description != copy->description);

	g_assert_cmpstr(info.author, ==, copy->author);
	g_assert(info.author != copy->author);

	g_assert_cmpstr(info.website, ==, copy->website);
	g_assert(info.website != copy->website);

	g_assert_cmpstr(info.dependencies->data, ==, copy->dependencies->data);
	g_assert(info.dependencies->data != copy->dependencies->data);

	gplugin_plugin_info_free(copy);
}

gint
main(gint argc, gchar **argv) {

	g_test_init(&argc, &argv, NULL);

	gplugin_init();

	g_test_add_func("/types/boxed/plugin_info",
	                test_gplugin_plugin_info);

	return g_test_run();
}

