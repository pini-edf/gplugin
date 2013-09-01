#include <glib.h>
#include <gplugin.h>

#include <stdio.h>

static void
test_full(void) {
	GPluginPlugin *plugin = NULL;
	GPluginPluginInfo *info = NULL;
	GError *error = NULL;
	const gchar *id = "gplugin-python/basic-plugin";

	plugin = gplugin_plugin_manager_find_plugin(id);
	g_assert(plugin != NULL);

	info = gplugin_plugin_get_info(plugin);
	g_assert(info != NULL);

	g_assert_cmpstr(id, ==, gplugin_plugin_info_get_id(info));
	g_assert_cmpuint(0x01020304, ==,
	                 gplugin_plugin_info_get_abi_version(info));
	g_assert_cmpstr("basic plugin", ==, gplugin_plugin_info_get_name(info));
	g_assert_cmpstr("test", ==, gplugin_plugin_info_get_category(info));
	g_assert_cmpstr("version", ==, gplugin_plugin_info_get_version(info));
	g_assert_cmpstr("license", ==, gplugin_plugin_info_get_license_id(info));
	g_assert_cmpstr("summary", ==, gplugin_plugin_info_get_summary(info));
	g_assert_cmpstr("description", ==,
	                gplugin_plugin_info_get_description(info));
	g_assert_cmpstr("website", ==, gplugin_plugin_info_get_website(info));

	g_object_unref(G_OBJECT(info));

	g_assert_cmpint(GPLUGIN_PLUGIN_STATE_QUERIED, ==,
	                gplugin_plugin_get_state(plugin));

	gplugin_plugin_manager_load_plugin(plugin, &error);
	g_assert_no_error(error);
	g_assert_cmpint(GPLUGIN_PLUGIN_STATE_LOADED, ==,
	                gplugin_plugin_get_state(plugin));

	gplugin_plugin_manager_unload_plugin(plugin, &error);
	g_assert_no_error(error);
	g_assert_cmpint(GPLUGIN_PLUGIN_STATE_QUERIED, ==,
	                gplugin_plugin_get_state(plugin));

	g_object_unref(G_OBJECT(plugin));
}

static void
test_load_failed(void) {
	GPluginPlugin *plugin = NULL;
	GError *error = NULL;
	gboolean ret = FALSE;

	plugin = gplugin_plugin_manager_find_plugin("gplugin-python/load-failed");
	g_assert(plugin != NULL);

	ret = gplugin_plugin_manager_load_plugin(plugin, &error);
	g_assert(ret == FALSE);
	g_assert_error(error, GPLUGIN_DOMAIN, 0);

	g_object_unref(G_OBJECT(plugin));
}

static void
test_unload_failed(void) {
	GPluginPlugin *plugin = NULL;
	GError *error = NULL;
	gboolean ret = FALSE;

	plugin = gplugin_plugin_manager_find_plugin("gplugin-python/unload-failed");
	g_assert(plugin != NULL);

	ret = gplugin_plugin_manager_load_plugin(plugin, &error);
	g_assert(ret == TRUE);
	g_assert_no_error(error);

	ret = gplugin_plugin_manager_unload_plugin(plugin, &error);
	g_assert(ret == FALSE);
	g_assert_error(error, GPLUGIN_DOMAIN, 0);

	g_object_unref(G_OBJECT(plugin));
}

/******************************************************************************
 * Main
 *****************************************************************************/
gint
main(gint argc, gchar **argv) {
	g_test_init(&argc, &argv, NULL);

	gplugin_init();

	g_setenv("GI_TYPELIB_PATH", GI_TYPELIB_PATH, TRUE);

	gplugin_plugin_manager_append_path(PYTHON_LOADER_DIR);
	gplugin_plugin_manager_append_path(PYTHON_PLUGIN_DIR);

	gplugin_plugin_manager_refresh();

	g_test_add_func("/loaders/python/full", test_full);
	g_test_add_func("/loaders/python/load-failed", test_load_failed);
	g_test_add_func("/loaders/python/unload-failed", test_unload_failed);

	return g_test_run();
}

