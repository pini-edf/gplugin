/*
 * Copyright (C) 2011-2013 Gary Kramlich <grim@reaperworld.com>
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

#include <glib.h>

typedef struct {
	gboolean loading;
	gboolean loaded;
	gboolean unloading;
	gboolean unloaded;
} TestGPluginManagerSignalsData;

/******************************************************************************
 * Callbacks
 *****************************************************************************/
static gboolean
test_gplugin_manager_signals_normal_loading(GPLUGIN_UNUSED GObject *manager,
                                            GPLUGIN_UNUSED GPluginPlugin *plugin,
                                            GPLUGIN_UNUSED GError **error,
                                            gpointer d)
{
	TestGPluginManagerSignalsData *data = (TestGPluginManagerSignalsData *)d;

	data->loading = TRUE;

	return TRUE;
}

static void
test_gplugin_manager_signals_normal_loaded(GPLUGIN_UNUSED GObject *manager,
                                           GPLUGIN_UNUSED GPluginPlugin *plugin,
                                           gpointer d)
{
	TestGPluginManagerSignalsData *data = (TestGPluginManagerSignalsData *)d;

	data->loaded = TRUE;
}

static gboolean
test_gplugin_manager_signals_normal_unloading(GPLUGIN_UNUSED GObject *manager,
                                              GPLUGIN_UNUSED GPluginPlugin *plugin,
                                              GPLUGIN_UNUSED GError **error,
                                              gpointer d)
{
	TestGPluginManagerSignalsData *data = (TestGPluginManagerSignalsData *)d;

	data->unloading = TRUE;

	return TRUE;
}

static void
test_gplugin_manager_signals_normal_unloaded(GPLUGIN_UNUSED GObject *manager,
                                             GPLUGIN_UNUSED GPluginPlugin *plugin,
                                             gpointer d)
{
	TestGPluginManagerSignalsData *data = (TestGPluginManagerSignalsData *)d;

	data->unloaded = TRUE;
}

static gboolean
test_gplugin_manager_signals_stop_loading(GPLUGIN_UNUSED GObject *manager,
                                          GPLUGIN_UNUSED GPluginPlugin *plugin,
                                          GError **error,
                                          gpointer d)
{
	TestGPluginManagerSignalsData *data = (TestGPluginManagerSignalsData *)d;

	data->loading = TRUE;

	*error = g_error_new(GPLUGIN_DOMAIN, 0, "loading failed");

	return FALSE;
}

static gboolean
test_gplugin_manager_signals_stop_unloading(GPLUGIN_UNUSED GObject *manager,
                                            GPLUGIN_UNUSED GPluginPlugin *plugin,
                                            GError **error,
                                            gpointer d)
{
	TestGPluginManagerSignalsData *data = (TestGPluginManagerSignalsData *)d;

	data->unloading = TRUE;

	*error = g_error_new(GPLUGIN_DOMAIN, 0, "unloading failed");

	return FALSE;
}

/******************************************************************************
 * Tests
 *****************************************************************************/
static void
test_gplugin_manager_signals_normal(void) {
	GPluginPlugin *plugin = NULL;
	GObject *manager = gplugin_manager_get_instance();
	GError *error = NULL;
	TestGPluginManagerSignalsData data = { FALSE, FALSE, FALSE, FALSE };
	gulong signals[] = { 0, 0, 0, 0};

	signals[0] =
		g_signal_connect(manager, "loading-plugin",
		                 G_CALLBACK(test_gplugin_manager_signals_normal_loading),
		                 &data);
	signals[1] =
		g_signal_connect(manager, "loaded-plugin",
		                 G_CALLBACK(test_gplugin_manager_signals_normal_loaded),
		                 &data);
	signals[2] =
		g_signal_connect(manager, "unloading-plugin",
		                 G_CALLBACK(test_gplugin_manager_signals_normal_unloading),
		                 &data);
	signals[3] =
		g_signal_connect(manager, "unloaded-plugin",
		                 G_CALLBACK(test_gplugin_manager_signals_normal_unloaded),
		                 &data);

	gplugin_manager_append_path(TEST_DIR);
	gplugin_manager_refresh();

	plugin = gplugin_manager_find_plugin("gplugin/native-basic-plugin");
	gplugin_manager_load_plugin(plugin, &error);
	g_assert_no_error(error);
	g_assert(data.loading);
	g_assert(data.loaded);

	gplugin_manager_unload_plugin(plugin, &error);
	g_assert_no_error(error);
	g_assert(data.unloading);
	g_assert(data.unloaded);

	g_signal_handler_disconnect(manager, signals[0]);
	g_signal_handler_disconnect(manager, signals[1]);
	g_signal_handler_disconnect(manager, signals[2]);
	g_signal_handler_disconnect(manager, signals[3]);
}

static void
test_gplugin_manager_signals_loading_stopped(void) {
	GPluginPlugin *plugin = NULL;
	GObject *manager = gplugin_manager_get_instance();
	GError *error = NULL;
	TestGPluginManagerSignalsData data = { FALSE, FALSE, FALSE, FALSE };
	gulong signals[] = { 0, 0, 0, 0};

	signals[0] =
		g_signal_connect(manager, "loading-plugin",
		                 G_CALLBACK(test_gplugin_manager_signals_stop_loading),
		                 &data);
	signals[1] =
		g_signal_connect(manager, "loaded-plugin",
		                 G_CALLBACK(test_gplugin_manager_signals_normal_loaded),
		                 &data);
	signals[2] =
		g_signal_connect(manager, "unloading-plugin",
		                 G_CALLBACK(test_gplugin_manager_signals_normal_unloading),
		                 &data);
	signals[3] =
		g_signal_connect(manager, "unloaded-plugin",
		                 G_CALLBACK(test_gplugin_manager_signals_normal_unloaded),
		                 &data);

	gplugin_manager_append_path(TEST_DIR);
	gplugin_manager_refresh();

	plugin = gplugin_manager_find_plugin("gplugin/native-basic-plugin");
	gplugin_manager_load_plugin(plugin, &error);
	g_assert_error(error, GPLUGIN_DOMAIN, 0);
	g_assert(data.loading);
	g_assert(data.loaded == FALSE);
	g_assert(data.unloading == FALSE);
	g_assert(data.unloaded == FALSE);

	g_signal_handler_disconnect(manager, signals[0]);
	g_signal_handler_disconnect(manager, signals[1]);
	g_signal_handler_disconnect(manager, signals[2]);
	g_signal_handler_disconnect(manager, signals[3]);
}

static void
test_gplugin_manager_signals_unloading_stopped(void) {
	GPluginPlugin *plugin = NULL;
	GObject *manager = gplugin_manager_get_instance();
	GError *error = NULL;
	TestGPluginManagerSignalsData data = { FALSE, FALSE, FALSE, FALSE };
	gulong signals[] = { 0, 0, 0, 0};

	signals[0] =
		g_signal_connect(manager, "loading-plugin",
		                 G_CALLBACK(test_gplugin_manager_signals_normal_loading),
		                 &data);
	signals[1] =
		g_signal_connect(manager, "loaded-plugin",
		                 G_CALLBACK(test_gplugin_manager_signals_normal_loaded),
		                 &data);
	signals[2] =
		g_signal_connect(manager, "unloading-plugin",
		                 G_CALLBACK(test_gplugin_manager_signals_stop_unloading),
		                 &data);
	signals[3] =
		g_signal_connect(manager, "unloaded-plugin",
		                 G_CALLBACK(test_gplugin_manager_signals_normal_unloaded),
		                 &data);

	gplugin_manager_append_path(TEST_DIR);
	gplugin_manager_refresh();

	plugin = gplugin_manager_find_plugin("gplugin/native-basic-plugin");
	gplugin_manager_load_plugin(plugin, &error);
	g_assert_no_error(error);
	g_assert(data.loading);
	g_assert(data.loaded);

	gplugin_manager_unload_plugin(plugin, &error);
	g_assert_error(error, GPLUGIN_DOMAIN, 0);
	g_assert(data.unloading);
	g_assert(data.unloaded == FALSE);

	g_signal_handler_disconnect(manager, signals[0]);
	g_signal_handler_disconnect(manager, signals[1]);
	g_signal_handler_disconnect(manager, signals[2]);
	g_signal_handler_disconnect(manager, signals[3]);
}

/******************************************************************************
 * Main
 *****************************************************************************/
gint
main(gint argc, gchar **argv) {

	g_test_init(&argc, &argv, NULL);

	gplugin_init();

	g_test_add_func("/manager/signals/normal",
	                test_gplugin_manager_signals_normal);
	g_test_add_func("/manager/signals/loading-stopped",
	                test_gplugin_manager_signals_loading_stopped);
	g_test_add_func("/manager/signals/unloading-stopped",
	                test_gplugin_manager_signals_unloading_stopped);

	return g_test_run();
}

