local lgi = require 'lgi'
local GPlugin = lgi.GPlugin

function gplugin_query()
	info = GPlugin.PluginInfo {
		id = "gplugin-lua/basic-plugin",
		abi_version = 0x01020304,
		name = "basic plugin",
		category = "test",
		version = "version",
		license_id = "license-id",
		summary = "basic lua plugin",
		description = "description of the basic lua plugin"
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

