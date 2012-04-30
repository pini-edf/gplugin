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

static GPluginPluginInfo info = {
	.id = "dependent-native-plugin",
	.abi_version = GPLUGIN_NATIVE_PLUGIN_ABI_VERSION,
	.flags = 0,
	.name = "name",
	.version = "version",
	.summary = "summary",
	.description = "description",
	.author = "author",
	.website = "website",
};

G_MODULE_EXPORT const GPluginPluginInfo *
gplugin_plugin_query(void) {
	if(info.dependencies == NULL)
		info.dependencies = g_slist_prepend(NULL, "basic-native-plugin");

	return &info;
}

G_MODULE_EXPORT gboolean
gplugin_plugin_load(GPluginNativePlugin *plugin) {
	return TRUE;
}

G_MODULE_EXPORT gboolean
gplugin_plugin_unload(GPluginNativePlugin *plugin) {
	return TRUE;
}
