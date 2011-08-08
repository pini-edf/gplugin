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

#ifndef GIDBITS_PLUGIN_H
#define GIDBITS_PLUGIN_H

#define GIDBITS_TYPE_PLUGIN		       (gidbits_plugin_get_type())
#define GIDBITS_PLUGIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GIDBITS_TYPE_PLUGIN, GidbitsPlugin))
#define GIDBITS_PLUGIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), GIDBITS_TYPE_PLUGIN, GidbitsPluginClass))
#define GIDBITS_IS_PLUGIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GIDBITS_TYPE_PLUGIN))
#define GIDBITS_IS_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), GIDBITS_TYPE_PLUGIN))
#define GIDBITS_PLUGIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GIDBITS_TYPE_PLUGIN, GidbitsPluginClass))

typedef struct _GidbitsPlugin          GidbitsPlugin;
typedef struct _GidbitsPluginClass     GidbitsPluginClass;

#define GIDBITS_TYPE_PLUGIN_INFO       (gidbits_plugin_info_get_type())

typedef struct _GidbitsPluginInfo      GidbitsPluginInfo;

typedef enum {
	GIDBITS_PLUGIN_STATE_UNKNOWN = -1,
	GIDBITS_PLUGIN_STATE_ERROR = 0,
	GIDBITS_PLUGIN_STATE_LOADED,
	GIDBITS_PLUGIN_STATE_LOAD_FAILED,
	GIDBITS_PLUGIN_STATE_NOT_LOADED,
	GIDBITS_PLUGIN_STATES,
} GidbitsPluginState;

#include <glib.h>
#include <glib-object.h>

struct _GidbitsPlugin {
	GObject gparent;
};

struct _GidbitsPluginClass {
	GObjectClass gparent;

	void (*_gidbits_reserved_1)(void);
	void (*_gidbits_reserved_2)(void);
	void (*_gidbits_reserved_3)(void);
	void (*_gidbits_reserved_4)(void);
};

struct _GidbitsPluginInfo {
	guint32 abi_version;
	gchar *name;
	gchar *version;
	gchar *summary;
	gchar *description;
	gchar *author;
	gchar *website;
};

G_BEGIN_DECLS

GType gidbits_plugin_get_type(void);

const gchar *gidbits_plugin_get_filename(const GidbitsPlugin *plugin);
GidbitsPluginInfo *gidbits_plugin_get_info(const GidbitsPlugin *plugin);

GType gidbits_plugin_info_get_type(void);

GidbitsPluginInfo *gidbits_plugin_info_copy(const GidbitsPluginInfo *info);
void gidbits_plugin_info_free(GidbitsPluginInfo *info);

G_END_DECLS

#endif /* GIDBITS_PLUGIN_H */

