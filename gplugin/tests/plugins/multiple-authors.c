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

G_MODULE_EXPORT GPluginPluginInfo *
gplugin_query(GError **error) {
	const gchar * const authors[] = {
		"author1",
		"author2",
		"author3",
		"author4",
		NULL
	};

	return g_object_new(GPLUGIN_TYPE_PLUGIN_INFO,
		"id", "gplugin/multiple-authors",
		"abi-version", GPLUGIN_NATIVE_PLUGIN_ABI_VERSION,
		"authors", authors,
		NULL
	);
}

G_MODULE_EXPORT gboolean
gplugin_load(GPluginNativePlugin *plugin, GError **error) {
	return TRUE;
}

G_MODULE_EXPORT gboolean
gplugin_unload(GPluginNativePlugin *plugin, GError **error) {
	return TRUE;
}

