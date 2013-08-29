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

#include "gplugin-python-plugin-loader.h"

/* _POSIX_C_SOURCE get's redefined in Python.h so we undef it to avoid the
 * compiler warning...
 */
#undef _POSIX_C_SOURCE
#include <pygobject.h>
#include <Python.h>

#define GPLUGIN_PYTHON_PLUGIN_LOADER_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GPLUGIN_TYPE_PYTHON_PLUGIN_LOADER, GPluginPythonPluginLoaderPrivate))

/******************************************************************************
 * Typedefs
 *****************************************************************************/
typedef struct {
	guint gc_id;
	PyThreadState *py_thread_state;
} GPluginPythonPluginLoaderPrivate;

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;
static GType type = G_TYPE_INVALID;

/******************************************************************************
 * GPluginPluginLoaderInterface API
 *****************************************************************************/
static GPluginPlugin *
gplugin_python_plugin_loader_query(GPluginPluginLoader *loader,
                                   const gchar *filename,
                                   GError **error)
{
	g_warning("querying: %s", filename);
	return NULL;
}

static gboolean
gplugin_python_plugin_loader_load(GPluginPluginLoader *loader,
                                  GPluginPlugin *plugin,
                                  GError **error)
{
	return FALSE;
}

static gboolean
gplugin_python_plugin_loader_unload(GPluginPluginLoader *loader,
                                    GPluginPlugin *plugin,
                                    GError **error)
{
	return FALSE;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
gplugin_python_plugin_loader_finalize(GObject *obj) {
	GPluginPythonPluginLoaderPrivate *priv =
		GPLUGIN_PYTHON_PLUGIN_LOADER_GET_PRIVATE(obj);

	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static void
gplugin_python_plugin_loader_class_init(GPluginPythonPluginLoaderClass *klass) {
	GSList *exts = NULL;
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	GPluginPluginLoaderClass *loader_class =
		GPLUGIN_PLUGIN_LOADER_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	g_type_class_add_private(klass, sizeof(GPluginPythonPluginLoaderPrivate));

	obj_class->finalize = gplugin_python_plugin_loader_finalize;

	exts = g_slist_append(NULL, "py");
	exts = g_slist_append(exts, "pyc");
	loader_class->supported_extensions = exts;
	loader_class->query = gplugin_python_plugin_loader_query;
	loader_class->load = gplugin_python_plugin_loader_load;
	loader_class->unload = gplugin_python_plugin_loader_unload;
}

/******************************************************************************
 * API
 *****************************************************************************/
void
gplugin_python_plugin_loader_register(GPluginNativePlugin *plugin) {
	if(G_UNLIKELY(type == 0)) {
		static const GTypeInfo info = {
			.class_size = sizeof(GPluginPythonPluginLoaderClass),
			.class_init = (GClassInitFunc)gplugin_python_plugin_loader_class_init,
			.instance_size = sizeof(GPluginPythonPluginLoader),
		};

		type = gplugin_native_plugin_register_type(plugin,
		                                           GPLUGIN_TYPE_PLUGIN_LOADER,
		                                           "GPluginPythonPluginLoader",
		                                           &info,
		                                           0);
	}
}

GType
gplugin_python_plugin_loader_get_type(void) {
	if(G_UNLIKELY(type == 0)) {
		g_warning("gplugin_python_plugin_loader_get_type was called before "
		          "the type was registered!\n");
	}

	return type;
}

