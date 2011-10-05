#include <gplugin.h>

static const GPluginPluginInfo info = {
	.abi_version = GPLUGIN_NATIVE_PLUGIN_ABI_VERSION,
	.name = "test plugin",
	.summary = "a plugin to test the native loader",
	.description = "This plugin doesn't do anything, it is just used to test "
	               "that the native plugin loader is working correctly",
	.author = "Gary Kramlich <grim@reaperworld.com>",
	.website = "https://www.guifications.org/projects/gplugin/",
};

G_MODULE_EXPORT const GPluginPluginInfo *
gplugin_plugin_query(void) {
	return &info;
}

