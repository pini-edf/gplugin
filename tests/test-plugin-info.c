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

static void
test_gplugin_plugin_info_construction(void) {
	GPluginPluginInfo *info = NULL;
	gchar *id = NULL, *name = NULL, *version = NULL;
	gchar *license = NULL, *license_text = NULL, *license_url = NULL;
	gchar *icon = NULL, *summary = NULL, *description = NULL, *category = NULL;
	gchar *author = NULL, *website = NULL, *dependencies = NULL;
	guint abi_version = 0;
	GPluginPluginInfoFlags flags = 0;

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

	g_assert_cmpstr(id, ==, "gplugin-test/plugin-info-test");
	g_assert_cmpstr(id, ==, gplugin_plugin_info_get_id(info));

	g_assert_cmpuint(abi_version, ==, GPLUGIN_NATIVE_PLUGIN_ABI_VERSION);
	g_assert_cmpuint(abi_version, ==,
	                 gplugin_plugin_info_get_abi_version(info));

	g_assert_cmpuint(flags, ==, GPLUGIN_PLUGIN_INFO_FLAGS_LOAD_ON_QUERY |
	                            GPLUGIN_PLUGIN_INFO_FLAGS_INTERNAL);
	g_assert_cmpuint(flags, ==, gplugin_plugin_info_get_flags(info));

	g_assert_cmpstr(name, ==, "name");
	g_assert_cmpstr(name, ==, gplugin_plugin_info_get_name(info));

	g_assert_cmpstr(version, ==, "version");
	g_assert_cmpstr(version, ==, gplugin_plugin_info_get_version(info));

	g_assert_cmpstr(license, ==, "license");
	g_assert_cmpstr(license, ==, gplugin_plugin_info_get_license(info));

	g_assert_cmpstr(license_text, ==, "license-text");
	g_assert_cmpstr(license_text, ==,
	                gplugin_plugin_info_get_license_text(info));

	g_assert_cmpstr(license_url, ==, "license-url");
	g_assert_cmpstr(license_url, ==,
	                gplugin_plugin_info_get_license_url(info));

	g_assert_cmpstr(icon, ==, "icon");
	g_assert_cmpstr(icon, ==, gplugin_plugin_info_get_icon(info));

	g_assert_cmpstr(summary, ==, "summary");
	g_assert_cmpstr(summary, ==, gplugin_plugin_info_get_summary(info));

	g_assert_cmpstr(description, ==, "description");
	g_assert_cmpstr(description, ==,
	                gplugin_plugin_info_get_description(info));

	g_assert_cmpstr(category, ==, "category");
	g_assert_cmpstr(category, ==, gplugin_plugin_info_get_category(info));

	g_assert_cmpstr(author, ==, "author");
	g_assert_cmpstr(author, ==, gplugin_plugin_info_get_author(info));

	g_assert_cmpstr(website, ==, "website");
	g_assert_cmpstr(website, ==, gplugin_plugin_info_get_website(info));

	g_assert_cmpstr(dependencies, ==, "dependencies");
	g_assert_cmpstr(dependencies, ==,
	                gplugin_plugin_info_get_dependencies(info));

	g_free(id);
	g_free(name);
	g_free(version);
	g_free(license);
	g_free(license_text);
	g_free(license_url);
	g_free(icon);
	g_free(summary);
	g_free(description);
	g_free(category);
	g_free(author);
	g_free(website);
	g_free(dependencies);
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

