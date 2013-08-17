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

G_MODULE_EXPORT GPluginPluginInfo *
gplugin_plugin_query(GError **error) {
	const gchar * const authors[] = {
		"Gary Kramlich <grim@reaperworld.com>",
		NULL
	};

	return gplugin_plugin_info_new(
		"gplugin-python-loader",
		GPLUGIN_NATIVE_PLUGIN_ABI_VERSION,
		"flags", GPLUGIN_PLUGIN_INFO_FLAGS_LOAD_ON_QUERY |
		         GPLUGIN_PLUGIN_INFO_FLAGS_INTERNAL,
		"name", "Python Plugin Loader",
		"version", GPLUGIN_VERSION,
		"license-id", "GPL3",
		"summary", "A plugin that can load python plugins",
		"description", "This plugin allows the load of plugins written in "
		               "the python programming language.",
		"authors", authors,
		"website", "http://bitbucket.org/rw_grim/gplugin",
		NULL
	);
}

G_MODULE_EXPORT gboolean
gplugin_plugin_load(GPluginNativePlugin *plugin, GError **error) {
	return FALSE;
}

G_MODULE_EXPORT gboolean
gplugin_plugin_unload(GPluginNativePlugin *plugin, GError **error) {
	return FALSE;
}

