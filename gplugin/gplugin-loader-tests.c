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

void
gplugin_loader_tests_add_tests(const gchar *short_name) {
	gint i = 0;

	for(i = 0; test_functions[i].path; i++) {
		gchar *path = g_strdup_printf(test_functions[i].path, short_name);

		g_test_add_data_func(path, short_name, test_functions[i].func);

		g_free(path);
	}
}


