/*
 * Copyright (C) 2011 Gary Kramlich <grim@reaperworld.com>
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
#if !defined(GIDBITS_H_INSIDE) && !defined(GIDBITS_COMPILATION)
#error "only <gidbits.h> may be included directly"
#endif

#ifndef GIDBITS_PLUGIN_LOADER_H
#define GIDBITS_PLUGIN_LOADER_H

#define GIDBITS_TYPE_PLUGIN_LOADER           (gidbits_plugin_loader_get_type())
#define GIDBITS_PLUGIN_LOADER(obj)           (G_TYPE_CHECK_INSTANCE_CAST((obj), GIDBITS_TYPE_PLUGIN_LOADER, GidbitsPluginLoader))
#define GIDBITS_IS_PLUGIN_LOADER(obj)        (G_TYPE_CHECK_INSTANCE_TYPE((obj), GIDBITS_TYPE_PLUGIN_LOADER))
#define GIDBITS_PLUGIN_LOADER_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE((obj), GIDBITS_TYPE_PLUGIN_LOADER, GidbitsPluginLoaderIface))

typedef struct _GidbitsPluginLoader          GidbitsPluginLoader;
typedef struct _GidbitsPluginLoaderIface     GidbitsPluginLoaderIface;

#include <glib.h>
#include <glib-object.h>

#include <gidbits/gidbits-plugin.h>

struct _GidbitsPluginLoaderIface {
	GTypeInterface gparent;

	GList *(*supported_extensions)(GidbitsPluginLoader *loader);

	GidbitsPluginInfo *(*query)(GidbitsPluginLoader *loader, const gchar *filename, GError **error);

	GidbitsPlugin *(*load)(GidbitsPluginLoader *loader, const gchar *filename, GError **error);
	gboolean (*unload)(GidbitsPluginLoader *loader, GidbitsPlugin *plugin, GError **error);

	void (*_gidbits_reserved_1)(void);
	void (*_gidbits_reserved_2)(void);
	void (*_gidbits_reserved_3)(void);
	void (*_gidbits_reserved_4)(void);
};

G_BEGIN_DECLS

GType gidbits_plugin_loader_get_type(void);

GList *gidbits_plugin_loader_get_supported_extensions(GidbitsPluginLoader *loader);

GidbitsPluginInfo *gidbits_plugin_loader_query_plugin(GidbitsPluginLoader *loader, const gchar *filename, GError **error);

GidbitsPlugin *gidbits_plugin_loader_load_plugin(GidbitsPluginLoader *loader, const gchar *filename, GError **error);
gboolean gidbits_plugin_loader_unload_plugin(GidbitsPluginLoader *loader, GidbitsPlugin *plugin, GError **error);

G_END_DECLS

#endif /* GIDBITS_PLUGIN_LOADER_H */

