#include <gplugin.h>

static const GPluginPluginInfo info = {
	.id = "test-native-plugin",
	.abi_version = GPLUGIN_NATIVE_PLUGIN_ABI_VERSION,
	.flags = 0,
	.name = "test plugin name",
	.version = "test plugin version",
	.summary = "test plugin summary",
	.description = "test plugin description",
	.author = "test plugin author",
	.website = "test plugin website",
};

G_MODULE_EXPORT const GPluginPluginInfo *
gplugin_plugin_query(void) {
	return &info;
}

