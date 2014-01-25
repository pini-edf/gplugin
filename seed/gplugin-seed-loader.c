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

#include "gplugin-seed-loader.h"
#include "gplugin-seed-plugin.h"

#include <glib/gi18n.h>

#include <seed.h>

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;
static GType type_real = 0;
static SeedEngine *engine = NULL;

/******************************************************************************
 * Helpers
 *****************************************************************************/

/******************************************************************************
 * GPluginLoaderInterface API
 *****************************************************************************/
static GSList *
gplugin_seed_loader_class_supported_extensions(const GPluginLoaderClass *klass) {
	return g_slist_append(NULL, "js");
}

static GPluginPlugin *
gplugin_seed_loader_query(GPluginLoader *loader, const gchar *filename,
                          GError **error)
{
	GPluginPluginInfo *info = NULL;
	SeedContext ctx = NULL;
	SeedException exp = NULL;
	SeedObject global = NULL, query = NULL, load = NULL, unload = NULL;
	SeedObject sinfo = NULL;
	SeedScript *script = NULL;
	gchar *contents = NULL;

	/* load the file */
	if(!g_file_get_contents(filename, &contents, NULL, error)) {
		return NULL;
	}

	/* create our context */
	ctx = seed_context_create(engine->group, NULL);
	seed_prepare_global_context(ctx);

	/* now create and evaluate the script object */
	script = seed_make_script(ctx, contents, filename, 0);
	g_free(contents);

	seed_evaluate(ctx, script, NULL);
	exp = seed_script_exception(script);
	seed_script_destroy(script);

	/* did we get any exceptions during evaluation? */
	if(exp) {
		if(error) {
			gchar *message = seed_exception_to_string(ctx, exp);

			*error = g_error_new(GPLUGIN_DOMAIN, 0, "Failed to load %s : %s",
			                     filename, message);

			g_free(message);
		}

		seed_context_unref(ctx);

		return NULL;
	}

	/* now get the global namespace from the context and use it to get the
	 * GPlugin entry points.
	 */
	global = seed_context_get_global_object(ctx);
	query = seed_object_get_property(ctx, global, "gplugin_query");
	load = seed_object_get_property(ctx, global, "gplugin_load");
	unload = seed_object_get_property(ctx, global, "gplugin_unload");

	/* now validate the entry points */
	if(!seed_value_is_function(ctx, query)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "Failed to find the gplugin_query function");
		}

		seed_context_unref(ctx);

		return NULL;
	}

	if(!seed_value_is_function(ctx, load)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "Failed to find the gplugin_load function");
		}

		seed_context_unref(ctx);

		return NULL;
	}

	if(!seed_value_is_function(ctx, unload)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "Failed to find the gplugin_unload function");
		}

		seed_context_unref(ctx);

		return NULL;
	}

	/* now that we have all of our entry points, call the query method to get
	 * the PluginInfo.
	 */
	sinfo = seed_object_call(ctx, query, NULL, 0, NULL, &exp);
	if(exp != NULL) {
		if(error) {
			gchar *message = seed_exception_get_message(ctx, exp);

			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "Failed to query %s : %s", filename, message);

			g_free(message);
		}

		seed_context_unref(ctx);

		return NULL;
	}

	/* now convert the JavaScript PluginInfo into the C version */
	info = seed_value_to_object(ctx, sinfo, exp);
	if(exp != NULL) {
		if(error) {
			gchar *message = seed_exception_get_message(ctx, exp);

			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "Failed to query %s : %s", filename, message);

			g_free(message);
		}

		seed_context_unref(ctx);

		return NULL;
	}

	/* validate that the info we got is an instance of GPluginPluginInfo */
	if(!GPLUGIN_IS_PLUGIN_INFO(info)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "Plugin %s did not return a PluginInfo",
			                     filename);
		}

		seed_context_unref(ctx);

		return NULL;
	}

	return g_object_new(GPLUGIN_TYPE_SEED_PLUGIN,
	                    "filename", filename,
	                    "loader", loader,
	                    "info", info,
	                    "context", ctx,
	                    "load", load,
	                    "unload", unload,
	                    NULL);
}

static gboolean
gplugin_seed_loader_load(GPluginLoader *loader, GPluginPlugin *plugin,
                         GError **error)
{
	return FALSE;
}

static gboolean
gplugin_seed_loader_unload(GPluginLoader *loader, GPluginPlugin *plugin,
                           GError **error)
{
	return FALSE;
}

/******************************************************************************
 * GObject Stuff
 *****************************************************************************/
static void
gplugin_seed_loader_class_init(GPluginSeedLoaderClass *klass) {
	GPluginLoaderClass *loader_class = GPLUGIN_LOADER_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	loader_class->supported_extensions =
		gplugin_seed_loader_class_supported_extensions;
	loader_class->query = gplugin_seed_loader_query;
	loader_class->load = gplugin_seed_loader_load;
	loader_class->unload = gplugin_seed_loader_unload;
}

/******************************************************************************
 * API
 *****************************************************************************/
void
gplugin_seed_loader_register(GPluginNativePlugin *plugin) {
	if(g_once_init_enter(&type_real)) {
		GType type = 0;

		static const GTypeInfo info = {
			.class_size = sizeof(GPluginSeedLoaderClass),
			.class_init = (GClassInitFunc)gplugin_seed_loader_class_init,
			.instance_size = sizeof(GPluginSeedLoader),
		};

		type = gplugin_native_plugin_register_type(plugin,
		                                           GPLUGIN_TYPE_LOADER,
		                                           "GPluginSeedLoader",
		                                           &info,
		                                           0);

		engine = seed_init(NULL, NULL);

		g_once_init_leave(&type_real, type);
	}
}

GType
gplugin_seed_loader_get_type(void) {
	if(G_UNLIKELY(type_real == 0)) {
		g_warning("gplugin_seed_loader_get_type was called before "
		          "the type was registered!\n");
	}

	return type_real;
}

