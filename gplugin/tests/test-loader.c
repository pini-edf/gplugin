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

#include <stdio.h>
#include <stdlib.h>

#include <glib.h>
#include <glib/gi18n.h>

#include <gplugin.h>

/******************************************************************************
 * Structs
 *****************************************************************************/
typedef struct {
	gchar *path;
	GTestDataFunc func;
} GPluginTestLoaderFunction;

/******************************************************************************
 * Tests
 *****************************************************************************/
static void
gplugin_test_loader_full(gconstpointer d) {
}

static void
gplugin_test_loader_load_failed(gconstpointer d) {
}

static void
gplugin_test_loader_load_exception(gconstpointer d) {
}

static void
gplugin_test_loader_unload_failed(gconstpointer d) {
}

static void
gplugin_test_loader_dependencies(gconstpointer d) {
}

/******************************************************************************
 * Main
 *****************************************************************************/
static GPluginTestLoaderFunction test_functions[] = {
	{ "/loaders/%s/full", gplugin_test_loader_full },
	{ "/loaders/%s/load-failed", gplugin_test_loader_load_failed },
	{ "/loaders/%s/load-exception", gplugin_test_loader_load_exception },
	{ "/loaders/%s/unload-failed", gplugin_test_loader_unload_failed },
	{ "/loaders/%s/dependencies", gplugin_test_loader_dependencies },
	{ NULL },
};

static gboolean
gplugin_test_loader_envvar_cb(const gchar *n, const gchar *v, gpointer d,
                              GError **error)
{
	gchar **parts = g_strsplit(v, "=", 2);
	g_setenv(parts[0], parts[1], TRUE);
	g_strfreev(parts);

	return TRUE;
}

static GOptionEntry entries[] = {
	{
		"envvar", 'e', 0, G_OPTION_ARG_CALLBACK,
		gplugin_test_loader_envvar_cb, N_("Set environment variable"),
		NULL,
	}, {
		NULL
	},
};

static void
gplugin_test_loader_add_tests(const gchar *short_name) {
	gint i = 0;

	for(i = 0; test_functions[i].path; i++) {
		gchar *path = g_strdup_printf(test_functions[i].path, short_name);

		g_test_add_data_func(path, short_name, test_functions[i].func);

		g_free(path);
	}
}

gint
main(gint argc, gchar **argv) {
	GError *error = NULL;
	GOptionContext *ctx = NULL;
	gint ret = 0;

	g_test_init(&argc, &argv, NULL);
	gplugin_init();

	ctx = g_option_context_new("LOADER_SHORT_NAME");
	g_option_context_set_translation_domain(ctx, GETTEXT_PACKAGE);
	g_option_context_set_ignore_unknown_options(ctx, TRUE);
	g_option_context_add_main_entries(ctx, entries, NULL);
	g_option_context_add_group(ctx, gplugin_get_option_group());
	g_option_context_parse(ctx, &argc, &argv, &error);
	g_option_context_free(ctx);


	if(error) {
		fprintf(stderr, "%s\n", error->message);

		g_error_free(error);

		gplugin_uninit();

		return EXIT_FAILURE;
	}

	if(argc != 2) {
		fprintf(stderr, "You must specify a shortname for the loader\n");

		gplugin_uninit();

		return EXIT_FAILURE;
	}

	gplugin_manager_refresh();

	gplugin_test_loader_add_tests(argv[1]);

	ret = g_test_run();

	gplugin_uninit();

	return ret;
}

