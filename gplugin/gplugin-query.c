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
static gboolean internal = FALSE;
static gchar **paths = NULL;
static gboolean add_default_paths = TRUE;

/******************************************************************************
 * Helpers
 *****************************************************************************/
static gboolean
no_default_cb(const gchar *n, const gchar *v, gpointer d, GError **e) {
	add_default_paths = FALSE;

	return TRUE;
}

static gboolean
verbosity_cb(const gchar *n, const gchar *v, gpointer d, GError **e) {
	verbosity++;

	return TRUE;
}

static gboolean
full_verbosity_cb(const gchar *n, const gchar *v, gpointer d, GError **e) {
	verbosity = 1 << 11;

	return TRUE;
}

static gboolean
internal_cb(const gchar *n, const gchar *v, gpointer d, GError **e) {
	internal = TRUE;

	return TRUE;
}

static gchar *
flags_to_string(GPluginPluginInfoFlags flags) {
	GString *str = g_string_new("");
	gchar *ret = NULL;
	GFlagsValue *value = NULL;
	GFlagsClass *klass = NULL;
	gboolean first = TRUE;

	klass = g_type_class_ref(GPLUGIN_TYPE_PLUGIN_INFO_FLAGS);

	while((value = g_flags_get_first_value(klass, flags))) {
		if(!first)
			str = g_string_append(str, ",");
		else
			first = FALSE;

		str = g_string_append(str, value->value_nick);

		flags ^= value->value;
	}

	g_type_class_unref(klass);

	/* hold on to the final string then free the gstring */
	ret = str->str;
	g_string_free(str, FALSE);

	return ret;

}

static gboolean
output_plugin(const gchar *id) {
	GSList *plugins = NULL, *l = NULL;
	gboolean first = TRUE;

	#define FORMAT "%-13s"
	#define MAIN_FORMAT_NEL "  " FORMAT ": "
	#define MAIN_FORMAT MAIN_FORMAT_NEL "%s\n"

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
		GPluginPluginInfoFlags flags = gplugin_plugin_info_get_flags(info);
		gchar **authors = NULL, **dependencies = NULL;
		gint i = 0;

		if(!internal && (flags & GPLUGIN_PLUGIN_INFO_FLAGS_INTERNAL))
			continue;

		if(!first)
			printf("\n");

		printf(MAIN_FORMAT, "name", gplugin_plugin_info_get_name(info));
		if(verbosity > 0)
			printf(MAIN_FORMAT, "category",
			       gplugin_plugin_info_get_category(info));
		printf(MAIN_FORMAT, "version", gplugin_plugin_info_get_version(info));
		printf(MAIN_FORMAT, "summary", gplugin_plugin_info_get_summary(info));
		if(verbosity > 0) {
			printf(MAIN_FORMAT_NEL, "authors");
			authors = gplugin_plugin_info_get_authors(info);
			if(authors) {
				for(i = 0; authors[i]; i++) {
					if(i > 0)
						printf("                ");
					printf("%s\n", authors[i]);
				}
				g_strfreev(authors);
			} else {
				printf("\n");
			}
			printf(MAIN_FORMAT, "website", gplugin_plugin_info_get_website(info));
		}
		if(verbosity > 1)
			printf(MAIN_FORMAT, "filename",
			       gplugin_plugin_get_filename(plugin));
		if(verbosity > 2) {
			gchar *flags_str = flags_to_string(flags);

			GPluginPluginLoader *loader = gplugin_plugin_get_loader(plugin);

			printf(MAIN_FORMAT_NEL "%08x\n", "abi version",
			       gplugin_plugin_info_get_abi_version(info));
			printf(MAIN_FORMAT, "flags", flags_str);
			printf(MAIN_FORMAT, "loader", G_OBJECT_TYPE_NAME(loader));

			g_free(flags_str);
		}
		if(verbosity > 0) {
			printf(MAIN_FORMAT, "description",
			       gplugin_plugin_info_get_description(info));
		}
		if(verbosity > 2) {
			printf(MAIN_FORMAT_NEL, "dependencies");
			dependencies = gplugin_plugin_info_get_dependencies(info);
			if(dependencies) {
				for(i = 0; dependencies[i]; i++) {
					if(i > 0)
						printf("                ");
					printf("%s\n", dependencies[i]);
				}
				g_strfreev(dependencies);
			} else {
				printf("\n");
			}
		}

		g_object_unref(G_OBJECT(info));

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
		"no-default-paths", 'D', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
		no_default_cb, "Do not search the default plugin paths",
		NULL,
	}, {
		"internal", 'i', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
		internal_cb, "Show internal plugins",
		NULL,
	}, {
		"path", 'p', 0, G_OPTION_ARG_STRING_ARRAY,
		&paths, "Additional path to look for plugins",
		"PATH",
	}, {
		"verbose", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
		verbosity_cb, "Increase verbosity",
		NULL,
	}, {
		"full-verbose", 'V', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
		full_verbosity_cb, "Increase verbosity to eleven",
		NULL,
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
	g_option_context_free(ctx);

	if(error) {
		fprintf(stderr, "%s\n", error->message);

		g_error_free(error);

		gplugin_uninit();

		return EXIT_FAILURE;
	}

	/* add the default gplugin paths unless asked not to */
	if(add_default_paths)
		gplugin_plugin_manager_add_default_paths();

	/* now add any paths the user provided */
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

