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

