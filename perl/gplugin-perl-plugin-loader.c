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

#include "gplugin-perl-plugin-loader.h"

#include <EXTERN.h>
#include <perl.h>

/* perl define's _() to something completely different that we don't use.  So
 * we undef it so that we can use it for gettext.
 */
#undef _
#include <glib/gi18n.h>

#define GPLUGIN_PERL_PLUGIN_LOADER_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GPLUGIN_TYPE_PERL_PLUGIN_LOADER, GPluginPerlPluginLoaderPrivate))

/******************************************************************************
 * Typedefs
 *****************************************************************************/
typedef struct {
	gint dummy;
} GPluginPerlPluginLoaderPrivate;

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;
static volatile GType type_real = 0;

/* I can't believe I have to use this variable name... */
static PerlInterpreter *my_perl = NULL;

/******************************************************************************
 * GPluginPluginLoaderInterface API
 *****************************************************************************/
static GPluginPlugin *
gplugin_perl_plugin_loader_query(GPluginPluginLoader *loader,
                                   const gchar *filename,
                                   GError **error)
{
	const gchar *args[] = { "", filename };
	gchar **argv = (gchar **)args;
	gint argc = 2;

	perl_parse(my_perl, NULL, argc, argv, NULL);

	call_argv("gplugin_plugin_query", G_DISCARD | G_NOARGS, argv);

	return NULL;
}

static gboolean
gplugin_perl_plugin_loader_load(GPluginPluginLoader *loader,
                                  GPluginPlugin *plugin,
                                  GError **error)
{
	return FALSE;
}

static gboolean
gplugin_perl_plugin_loader_unload(GPluginPluginLoader *loader,
                                    GPluginPlugin *plugin,
                                    GError **error)
{
	return FALSE;
}

/******************************************************************************
 * Perl Stuff
 *****************************************************************************/
static void
gplugin_perl_plugin_loader_init_perl(void) {
	gchar *args[] = { "", };
	gchar **argv = (gchar **)args;
	gint argc = 1;

	PERL_SYS_INIT(&argc, &argv);

	my_perl = perl_alloc();
	PERL_SET_CONTEXT(my_perl);
	PL_perl_destruct_level = 1;
	perl_construct(my_perl);
}

static void
gplugin_perl_plugin_loader_uninit_perl(void) {
	PERL_SYS_TERM();

	perl_destruct(my_perl);
	perl_free(my_perl);
	my_perl = NULL;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
gplugin_perl_plugin_loader_class_init(GPluginPerlPluginLoaderClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	GPluginPluginLoaderClass *loader_class =
		GPLUGIN_PLUGIN_LOADER_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	g_type_class_add_private(klass, sizeof(GPluginPerlPluginLoaderPrivate));

	loader_class->supported_extensions = g_slist_append(NULL, "pl");
	loader_class->query = gplugin_perl_plugin_loader_query;
	loader_class->load = gplugin_perl_plugin_loader_load;
	loader_class->unload = gplugin_perl_plugin_loader_unload;

	/* perl initialization */
	gplugin_perl_plugin_loader_init_perl();
}

static void
gplugin_perl_plugin_loader_class_finalize(GPluginPerlPluginLoaderClass *klass,
                                          gpointer class_data)
{
	/* perl uninitialization */
	gplugin_perl_plugin_loader_uninit_perl();
}

/******************************************************************************
 * API
 *****************************************************************************/
void
gplugin_perl_plugin_loader_register(GPluginNativePlugin *plugin) {
	if(g_once_init_enter(&type_real)) {
		GType type = 0;

		static const GTypeInfo info = {
			.class_size = sizeof(GPluginPerlPluginLoaderClass),
			.class_init = (GClassInitFunc)gplugin_perl_plugin_loader_class_init,
			.class_finalize = (GClassFinalizeFunc)gplugin_perl_plugin_loader_class_finalize,
			.instance_size = sizeof(GPluginPerlPluginLoader),
		};

		type = gplugin_native_plugin_register_type(plugin,
		                                           GPLUGIN_TYPE_PLUGIN_LOADER,
		                                           "GPluginPerlPluginLoader",
		                                           &info,
		                                           0);

		g_once_init_leave(&type_real, type);
	}
}

GType
gplugin_perl_plugin_loader_get_type(void) {
	if(G_UNLIKELY(type_real == 0)) {
		g_warning("gplugin_perl_plugin_loader_get_type was called before "
		          "the type was registered!\n");
	}

	return type_real;
}

