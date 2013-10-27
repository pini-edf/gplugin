--[[
 Copyright (C) 2011-2013 Gary Kramlich <grim@reaperworld.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
--]]

local lgi = require 'lgi'
local GPlugin = lgi.GPlugin

function gplugin_query()
	info = GPlugin.PluginInfo {
		id = "gplugin-lua/basic-plugin",
		abi_version = 0x01020304,
		name = "basic plugin",
		category = "test",
		version = "0.0.10",
		license_id = "license-id",
		summary = "basic lua plugin",
		description = "description of the basic lua plugin",
		authors = { "Gary Kramlich <grim@reaperworld.com>" },
		website = "http://bitbucket.org/rw_grim/gplugin/"
	}

	print(info)
	print(info._native)

	return info._native
end

function gplugin_load(plugin)
	return true
end

function gplugin_unload(plugin)
	return true
end

