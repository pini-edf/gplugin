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

#ifndef __GI_SCANNER__ /* hide this bit from g-ir-scanner */
#if !defined(GPLUGIN_GLOBAL_HEADER_INSIDE) && !defined(GPLUGIN_COMPILATION)
#error "only <gplugin.h> may be included directly"
#endif
#endif /* __GI_SCANNER__ */

#ifndef GPLUGIN_PLUGIN_H
#define GPLUGIN_PLUGIN_H

#define GPLUGIN_TYPE_PLUGIN		       (gplugin_plugin_get_type())
#define GPLUGIN_PLUGIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GPLUGIN_TYPE_PLUGIN, GPluginPlugin))
#define GPLUGIN_PLUGIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), GPLUGIN_TYPE_PLUGIN, GPluginPluginClass))
#define GPLUGIN_IS_PLUGIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GPLUGIN_TYPE_PLUGIN))
#define GPLUGIN_IS_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), GPLUGIN_TYPE_PLUGIN))
#define GPLUGIN_PLUGIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GPLUGIN_TYPE_PLUGIN, GPluginPluginClass))

typedef struct _GPluginPlugin          GPluginPlugin;
typedef struct _GPluginPluginClass     GPluginPluginClass;

#define GPLUGIN_TYPE_PLUGIN_INFO       (gplugin_plugin_info_get_type())

typedef struct _GPluginPluginInfo      GPluginPluginInfo;

typedef enum /*< prefix=GPLUGIN_PLUGIN_STATE,underscore_name=GPLUGIN_PLUGIN_STATE >*/ {
	GPLUGIN_PLUGIN_STATE_UNKNOWN = -1,
	GPLUGIN_PLUGIN_STATE_ERROR = 0,
	GPLUGIN_PLUGIN_STATE_QUERIED,
	GPLUGIN_PLUGIN_STATE_REQUERY,
	GPLUGIN_PLUGIN_STATE_LOADED,
	GPLUGIN_PLUGIN_STATE_LOAD_FAILED,
	GPLUGIN_PLUGIN_STATES, /*< skip >*/
} GPluginPluginState;

typedef enum /*< prefix=GPLUGIN_PLUGIN_FLAGS,underscore_name=GPLUGIN_PLUGIN_FLAGS >*/ {
	GPLUGIN_PLUGIN_FLAGS_LOAD_ON_QUERY = 1 << 1,
	GPLUGIN_PLUGIN_FLAGS_INTERNAL = 1 << 2,
} GPluginPluginFlags;

#include <glib.h>
#include <glib-object.h>

#include <gplugin/gplugin-plugin-loader.h>

struct _GPluginPlugin {
	GObject gparent;
};

struct _GPluginPluginClass {
	GObjectClass gparent;

	void (*state_changed)(GPluginPlugin *, GPluginPluginState, GPluginPluginState);

	void (*_gplugin_reserved_1)(void);
	void (*_gplugin_reserved_2)(void);
	void (*_gplugin_reserved_3)(void);
	void (*_gplugin_reserved_4)(void);
};

struct _GPluginPluginInfo {
	gchar *id;

	guint32 abi_version;
	GPluginPluginFlags flags;

	gchar *name;
	gchar *version;
	gchar *license;

	gchar *icon;

	gchar *summary;
	gchar *description;
	gchar *author;
	gchar *website;

	GSList *dependencies;
};

G_BEGIN_DECLS

GType gplugin_plugin_get_type(void);

const gchar *gplugin_plugin_get_filename(const GPluginPlugin *plugin);
GPluginPluginLoader *gplugin_plugin_get_loader(const GPluginPlugin *plugin);
const GPluginPluginInfo *gplugin_plugin_get_info(const GPluginPlugin *plugin);

GType gplugin_plugin_info_get_type(void);

GPluginPluginInfo *gplugin_plugin_info_copy(const GPluginPluginInfo *info);
void gplugin_plugin_info_free(GPluginPluginInfo *info);

GPluginPluginState gplugin_plugin_get_state(const GPluginPlugin *plugin);
void gplugin_plugin_set_state(GPluginPlugin *plugin, GPluginPluginState state);

G_END_DECLS

#endif /* GPLUGIN_PLUGIN_H */

