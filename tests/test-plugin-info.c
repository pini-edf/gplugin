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
test_gplugin_plugin_info_construction(void) {
	GPluginPluginInfo *info = NULL;
	gchar *id = NULL, *name = NULL, *version = NULL;
	gchar *license = NULL, *license_text = NULL, *license_url = NULL;
	gchar *icon = NULL, *summary = NULL, *description = NULL, *category = NULL;
	gchar *author = NULL, *website = NULL, *dependencies = NULL;
	guint abi_version = 0;
	GPluginPluginInfoFlags flags = 0;

	#define test_string(var, value) G_STMT_START { \
		g_assert_cmpstr((var), ==, (value)); \
		g_assert_cmpstr((var), ==, gplugin_plugin_info_get_##var(info)); \
		g_free((var)); \
	} G_STMT_END

	#define test_uint(var, value) G_STMT_START { \
		g_assert_cmpuint((var), ==, (value)); \
		g_assert_cmpuint((var), ==, gplugin_plugin_info_get_##var(info)); \
	} G_STMT_END


	info = g_object_new(GPLUGIN_TYPE_PLUGIN_INFO,
		"id", "gplugin-test/plugin-info-test",
		"abi_version", GPLUGIN_NATIVE_PLUGIN_ABI_VERSION,
		"flags", GPLUGIN_PLUGIN_INFO_FLAGS_LOAD_ON_QUERY |
		         GPLUGIN_PLUGIN_INFO_FLAGS_INTERNAL,
		"name", "name",
		"version", "version",
		"license", "license",
		"license-text", "license-text",
		"license-url", "license-url",
		"icon", "icon",
		"summary", "summary",
		"description", "description",
		"category", "category",
		"author", "author",
		"website", "website",
		"dependencies", "dependencies",
		NULL
	);

	g_assert(GPLUGIN_IS_PLUGIN_INFO(info));

	g_object_get(G_OBJECT(info),
		"id", &id,
		"abi_version", &abi_version,
		"flags", &flags,
		"name", &name,
		"version", &version,
		"license", &license,
		"license-text", &license_text,
		"license-url", &license_url,
		"icon", &icon,
		"summary", &summary,
		"description", &description,
		"category", &category,
		"author", &author,
		"website", &website,
		"dependencies", &dependencies,
		NULL
	);

	test_string(id, "gplugin-test/plugin-info-test");
	test_uint(abi_version, GPLUGIN_NATIVE_PLUGIN_ABI_VERSION);
	test_uint(flags, GPLUGIN_PLUGIN_INFO_FLAGS_LOAD_ON_QUERY |
	                 GPLUGIN_PLUGIN_INFO_FLAGS_INTERNAL);
	test_string(name, "name");
	test_string(version, "version");
	test_string(license, "license");
	test_string(license_text, "license-text");
	test_string(license_url, "license-url");
	test_string(icon, "icon");
	test_string(summary, "summary");
	test_string(description, "description");
	test_string(category, "category");
	test_string(author, "author");
	test_string(website, "website");
	test_string(dependencies, "dependencies");

	#undef test_string
	#undef test_uint
}

/******************************************************************************
 * Main
 *****************************************************************************/
gint
main(gint argc, gchar **argv) {
	g_test_init(&argc, &argv, NULL);

	gplugin_init();

	g_test_add_func("/plugin-info/construction",
	                test_gplugin_plugin_info_construction);

	return g_test_run();
}

