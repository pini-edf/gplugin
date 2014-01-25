var GPlugin = imports.gi.GPlugin;

function gplugin_query() {
	return new GPlugin.PluginInfo({
		id: "gplugin/seed-basic-plugin",
		abi_version: 0x01020304,
		name: "basic plugin",
		authors: ['author1'],
		category: 'test',
		version: 'version',
		license_id: 'license',
		summary: 'summary',
		website: 'website',
		description: 'description'
	});
};

function gplugin_load(plugin) {
	return true;
};

function gplugin_unload(plugin) {
	return false;
};


