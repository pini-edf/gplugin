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

#include <gplugin/gplugin-core.h>
#include <gplugin/gplugin-private.h>
#include <gplugin/gplugin-plugin.h>

/******************************************************************************
 * API
 *****************************************************************************/
/**
 * gplugin_init:
 *
 * Initializes the GPlugin library.
 *
 * This function *MUST* be called before interacting with any other GPlugin API
 */
void
gplugin_init(void) {
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");

#if !GLIB_CHECK_VERSION(2, 36, 0)
	g_type_init();
#endif /* !GLIB_CHECK_VERSION(2, 36, 0) */

	gplugin_plugin_manager_private_init();
}

/**
 * gplugin_uninit:
 *
 * Uninitializes the GPlugin library
 */
void
gplugin_uninit(void) {
	gplugin_plugin_manager_private_uninit();
}

