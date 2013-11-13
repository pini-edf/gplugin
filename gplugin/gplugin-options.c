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

#include <glib.h>
#include <glib/gi18n.h>
#include <glib-object.h>

#include <gplugin/gplugin-options.h>

#include <gplugin/gplugin-manager.h>

/******************************************************************************
 * Options
 *****************************************************************************/
static gboolean add_default_paths = TRUE;
static gchar **paths = NULL;

static gboolean
gplugin_options_no_default_paths_cb(const gchar *n, const gchar *v,
                                    gpointer d, GError **e)
{
	add_default_paths = FALSE;

	return TRUE;
}

static GOptionEntry entries[] = {
	{
		"no-default-paths", 'D', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK,
		gplugin_options_no_default_paths_cb,
		N_("Do not search the default plugin paths"),
		NULL,
	}, {
		"path", 'p', 0, G_OPTION_ARG_STRING_ARRAY,
		&paths, N_("Additional path to look for plugins"),
		NULL,
	}, {
		NULL,
	},
};

static gboolean
gplugin_options_post_parse_cb(GOptionContext *ctx, GOptionGroup *group,
                              gpointer data, GError **error)
{
	gint i = 0;

	if(add_default_paths)
		gplugin_manager_add_default_paths();

	if(paths)
		for(i = 0; paths[i]; paths++)
			gplugin_manager_prepend_path(paths[i]);

	return TRUE;
}

/******************************************************************************
 * API
 *****************************************************************************/
/**
 * gplugin_get_option_group:
 *
 * Returns a #GOptionGroup for the commandline arguments recognized by
 * GPlugin.  You should add this option group to your #GOptionContext with
 * g_option_context_add_group(), if you are using g_option_context_parse() to
 * parse your commandline arguments.
 *
 * Return Value: (transfer full): a #GOptionGroup for the commandline arguments
 *                                recognized by GPlugin.
 */
GOptionGroup *
gplugin_get_option_group(void) {
	GOptionGroup *group = NULL;

	group = g_option_group_new("gplugin", _("GPlugin Options"),
	                           _("Show GPlugin Options"), NULL, NULL);

	g_option_group_set_parse_hooks(group, NULL,
	                               gplugin_options_post_parse_cb);
	g_option_group_add_entries(group, entries);
	g_option_group_set_translation_domain(group, GETTEXT_PACKAGE);

	return group;
}

