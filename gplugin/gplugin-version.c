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

#include <gplugin/gplugin-core.h>
#include <gplugin/gplugin-version.h>

#include <stdlib.h>

/******************************************************************************
 * Globals
 *****************************************************************************/
GRegex *regex = NULL;

static const gchar *version_pattern = "^(?P<major>\\d+)\\." \
                                      "(?P<minor>\\d+)" \
                                      "(\\.(?P<micro>\\d+)(?P<extra>.*))?$";


/******************************************************************************
 * Helpers
 *****************************************************************************/
static void
gplugin_version_lazy_init(void) {
	static volatile gsize init_volatile = 0;

	if(g_once_init_enter(&init_volatile)) {
		GError *error = NULL;

		regex = g_regex_new(version_pattern, G_REGEX_NO_AUTO_CAPTURE,
		                    0, &error);

		if(error) {
			g_warning("Failed to initialize the version regex: %s",
			          (error->message) ? error->message :
			                             "unknown");
			g_error_free(error);
		}

		g_once_init_leave(&init_volatile, 1);
	}
}

static gboolean
gplugin_version_parser(const gchar *v, gint *major, gint *minor, gint *micro,
                       gchar **extra, GError **error)
{
	GMatchInfo *info = NULL;
	gboolean matches = FALSE;
	gchar *temp = NULL;

	matches = g_regex_match(regex, v, 0, &info);
	if(!matches) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "%s does not match the version regex", v);
		}

		return FALSE;
	}

	/* grab the major version */
	if(major) {
		temp = g_match_info_fetch_named(info, "major");
		*major = atoi(temp);
		g_free(temp);
	}

	/* grab the minor version */
	if(minor) {
		temp = g_match_info_fetch_named(info, "minor");
		*minor = atoi(temp);
		g_free(temp);
	}

	/* grab the micro version */
	if(micro) {
		temp = g_match_info_fetch_named(info, "micro");
		*micro = atoi(temp);
		g_free(temp);
	}

	/* grab the extra version */
	if(extra)
		*extra = g_match_info_fetch_named(info, "extra");

	return TRUE;
}

/******************************************************************************
 * GPluginVersion API
 *****************************************************************************/
gint
gplugin_version_compare(const gchar *v1, const gchar *v2, GError **error) {
	gint v1_maj = 0, v1_min = 0, v1_mic = 0;
	gint v2_maj = 0, v2_min = 0, v2_mic = 0;
	gint t = 0;

	g_return_val_if_fail(v1 != NULL, 1);
	g_return_val_if_fail(v2 != NULL, 2);

	if(regex == NULL)
		gplugin_version_lazy_init();

	/* make sure v1 matches the regex */
	if(!gplugin_version_parser(v1, &v1_maj, &v1_min, &v1_mic, NULL, error))
		return 1;

	/* make sure v2 matches the regex */
	if(!gplugin_version_parser(v2, &v2_maj, &v2_min, &v2_mic, NULL, error))
		return -1;

	/* now figure out if they match */
	t = v2_maj - v1_maj;
	if(t != 0)
		return t;

	t = v2_min - v1_min;
	if(t != 0)
		return t;

	return v2_mic - v1_mic;
}

