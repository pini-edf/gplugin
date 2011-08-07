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

#include <gidbits/gidbits-plugin-loader.h>

GType
gidbits_plugin_loader_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		static const GTypeInfo info = {
			.class_size = sizeof(GidbitsPluginLoaderIface),
		};

		type = g_type_register_static(G_TYPE_INTERFACE,
		                              "GidbitsPluginLoader",
		                              &info, 0);
	}

	return type;
}

