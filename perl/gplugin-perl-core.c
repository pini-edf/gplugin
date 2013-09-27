/*
 * Copyright (C) 2011-2013 Gary Kramlich <grim@reaperworld.com>
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

#include "gplugin-perl-plugin-loader.h"

G_MODULE_EXPORT GPluginPluginInfo *
gplugin_plugin_query(GError **error) {
	const gchar * const authors[] = {
		"Gary Kramlich <grim@reaperworld.com>",
		NULL,
	};

	return gplugin_plugin_info_new(
		"gplugin/perl-loader",
		GPLUGIN_NATIVE_PLUGIN_ABI_VERSION,
		"internal", TRUE,
		"load-on-query", TRUE,
		"name", "Perl plugin loader",
		"version", GPLUGIN_VERSION,
		"license-id", "GPL3",
		"summary", "A plugin that can load perl plugins",
		"description", "This plugin allows the loading of plugins written in "
		               "the perl programming language.",
		"authors", authors,
		"website", GPLUGIN_WEBSITE,
		"category", "loaders",
		NULL
	);
}

G_MODULE_EXPORT gboolean
gplugin_plugin_load(GPluginNativePlugin *plugin, GError **error) {
	gplugin_perl_plugin_loader_register(plugin);

	gplugin_plugin_manager_register_loader(GPLUGIN_TYPE_PERL_PLUGIN_LOADER);

	return TRUE;
}

G_MODULE_EXPORT gboolean
gplugin_plugin_unload(GPluginNativePlugin *plugin, GError **error) {
	return TRUE;
}

