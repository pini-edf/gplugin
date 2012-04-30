/*
 * Copyright (C) 2011-2012 Gary Kramlich <grim@reaperworld.com>
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

#include <glib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gplugin.h>

/******************************************************************************
 * Globals
 *****************************************************************************/
static gint verbosity = 0;
static gchar **paths = NULL;

/******************************************************************************
 * Helpers
 *****************************************************************************/
static gboolean
verbosity_cb(const gchar *n, const gchar *v, gpointer d, GError **e) {
	verbosity++;

	return TRUE;
}

static gboolean
output_plugin(const gchar *id) {
	GSList *plugins = NULL, *l = NULL;
	gboolean first = TRUE;

	printf("%s", id);

	plugins = gplugin_plugin_manager_find_plugins(id);
	if(plugins == NULL) {
		printf(": not found\n");

		return FALSE;
	} else {
		printf("\n");
	}

	for(l = plugins; l; l = l->next) {
		GPluginPlugin *plugin = GPLUGIN_PLUGIN(l->data);
		const GPluginPluginInfo *info = gplugin_plugin_get_info(plugin);

		if(!first)
			printf("\n");

		printf("  name:        %s\n", info->name);
		printf("  version:     %s\n", info->version);
		printf("  summary:     %s\n", info->summary);
		printf("  author:      %s\n", info->author);
		printf("  website:     %s\n", info->website);
		if(verbosity > 0)
			printf("  filename:    %s\n", gplugin_plugin_get_filename(plugin));
		if(verbosity > 1) {
			GPluginPluginLoader *loader = gplugin_plugin_get_loader(plugin);

			printf("  abi version: %d\n", info->abi_version);
			printf("  flags:       %u\n", info->flags);
			printf("  loader:      %s\n", G_OBJECT_TYPE_NAME(loader));
		}
		printf("  description: %s\n", info->description);

		if(first)
			first = FALSE;
	}

	gplugin_plugin_manager_free_plugin_list(plugins);

	return TRUE;
}

static gboolean
output_plugins(GList *plugins) {
	GList *l = NULL;
	gboolean ret = TRUE;
	gboolean first = TRUE;

	for(l = plugins; l; l = l->next) {
		if(!first)
			printf("----------------------------------------\n");

		if(!output_plugin(l->data))
			ret = FALSE;

		if(first)
			first = FALSE;
	}

	return ret;
}

/******************************************************************************
 * Main Stuff
 *****************************************************************************/
static GOptionEntry entries[] = {
	{
		"verbose", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
		verbosity_cb, "Increase verbosity.",
		NULL,
	}, {
		"path", 'p', 0, G_OPTION_ARG_STRING_ARRAY,
		&paths, "Additional path to look for plugins",
		"PATH",
	}, {
		NULL
	},
};

gint
main(gint argc, gchar **argv) {
	GError *error = NULL;
	GOptionContext *ctx = NULL;
	gint i = 0, ret = 0;

	gplugin_init();

	ctx = g_option_context_new("PLUGIN-ID...");
	g_option_context_add_main_entries(ctx, entries, NULL);
	g_option_context_parse(ctx, &argc, &argv, &error);

	if(error) {
		fprintf(stderr, "%s\n", error->message);

		g_error_free(error);

		gplugin_uninit();

		return EXIT_FAILURE;
	}

	if(paths) {
		/* go through the paths and add them to the plugin manager */
		for(i = 0; paths[i]; i++)
			gplugin_plugin_manager_prepend_path(paths[i]);
	}

	gplugin_plugin_manager_refresh();

	/* check if the user gave us atleast one plugin, and output them */
	if(argc > 1) {
		GQueue *plugins = g_queue_new();

		for(i = 1; i < argc; i++) {
			if(!argv[i])
				continue;

			if(strlen(argv[i]) == 0)
				continue;

			g_queue_push_tail(plugins, argv[i]);
		}

		if(!output_plugins(plugins->head))
			ret = EXIT_FAILURE;
	} else {
		GList *plugins = gplugin_plugin_manager_list_plugins();

		if(!output_plugins(plugins))
			ret = EXIT_FAILURE;
	}

	gplugin_uninit();

	return ret;
}

