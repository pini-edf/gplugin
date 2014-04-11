-- Copyright (C) 2011-2014 Gary Kramlich <grim@reaperworld.com>
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.

lgi = require 'lgi'
GPlugin = lgi.GPlugin

gplugin_query = ->
	return GPlugin.PluginInfo {
		id: "gplugin/moon-basic-plugin",
		abi_version: 0x01020304,
		name: "basic plugin",
		category: "test",
		version: "version",
		license_id: "license",
		summary: "summary",
		description: "description",
		authors: { "author1" },
		website: "website"
	}

gplugin_load = ->
	return true


gplugin_unload = ->
	return true

