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
#include <glib-object.h>

#include <gplugin/gplugin-core.h>
#include <gplugin/gplugin-private.h>
#include <gplugin/gplugin-plugin.h>

/******************************************************************************
 * Globals
 *****************************************************************************/
GType plugin_info_type = G_TYPE_INVALID;

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
#if !GLIB_CHECK_VERSION(2, 36, 0)
	g_type_init();
#endif /* !GLIB_CHECK_VERSION(2, 36, 0) */

	plugin_info_type = GPLUGIN_TYPE_PLUGIN_INFO;

	gplugin_plugin_manager_init();
}

/**
 * gplugin_uninit:
 *
 * Uninitializes the GPlugin library
 */
void
gplugin_uninit(void) {
	gplugin_plugin_manager_uninit();
}

/**
 * gplugin_set_plugin_info_type:
 * @info_type: The #GType of the #GPluginPluginInfo instance that plugins
 *             should create in their query method.
 *
 * Sets the #GPluginPluginInfo instance type that plugins should create.
 */
void
gplugin_set_plugin_info_type(GType info_type) {
	g_return_if_fail(g_type_is_a(info_type, GPLUGIN_TYPE_PLUGIN_INFO));
	g_return_if_fail(!G_TYPE_IS_ABSTRACT(info_type));

	plugin_info_type = info_type;
}

/**
 * gpluin_get_plugin_info_type:
 *
 * Returns the #GType of the #GPluginPluginInfo instance that plugins should
 * create in their query method.
 *
 * Return Value: (transfer none): The #GType that plugins should use to create
 *                                their info instance.
 */
GType
gplugin_get_plugin_info_type(void) {
	return plugin_info_type;
}

