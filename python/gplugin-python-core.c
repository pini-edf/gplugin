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

static const GPluginPluginInfo info = {
	.id = "gplugin-python-loader",
	.abi_version = GPLUGIN_NATIVE_PLUGIN_ABI_VERSION,
	.name = "Python Plugin Loader",
	.summary = "A plugin that can load python plugins",
	.description = "This plugin allows the loading of plugins written in "
	               "the python programming language.",
	.author = "Gary Kramlich <grim@reaperworld.com>",
	.website = "https://www.guifications.org/projects/gplugin/",
};

G_MODULE_EXPORT const GPluginPluginInfo *
gplugin_plugin_query(void) {
	return &info;
}

