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

#ifndef GPLUGIN_PLUGIN_INFO_H
#define GPLUGIN_PLUGIN_INFO_H

#define GPLUGIN_TYPE_PLUGIN_INFO            (gplugin_plugin_info_get_type())
#define GPLUGIN_PLUGIN_INFO(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GPLUGIN_TYPE_PLUGIN_INFO, GPluginPluginInfo))
#define GPLUGIN_PLUGIN_INFO_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), GPLUGIN_TYPE_PLUGIN_INFO, GPluginPluginInfoClass))
#define GPLUGIN_IS_PLUGIN_INFO(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GPLUGIN_TYPE_PLUGIN_INFO))
#define GPLUGIN_IS_PLUGIN_INFO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), GPLUGIN_TYPE_PLUGIN_INFO))
#define GPLUGIN_PLUGIN_INFO_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GPLUGIN_TYPE_PLUGIN_INFO, GPluginPluginInfoClass))

typedef struct _GPluginPluginInfo           GPluginPluginInfo;
typedef struct _GPluginPluginInfoClass      GPluginPluginInfoClass;

typedef enum /*< prefix=GPLUGIN_PLUGIN_INFO_FLAGS,underscore_name=GPLUGIN_PLUGIN_INFO_FLAGS >*/ {
    GPLUGIN_PLUGIN_INFO_FLAGS_LOAD_ON_QUERY = 1 << 1,
    GPLUGIN_PLUGIN_INFO_FLAGS_INTERNAL = 1 << 2,
} GPluginPluginInfoFlags;

#include <glib.h>
#include <glib-object.h>

#include <gplugin/gplugin-plugin-loader.h>

struct _GPluginPluginInfo {
	GInitiallyUnowned gparent;
};

struct _GPluginPluginInfoClass {
	GObjectClass gparent;

	void (*_gplugin_reserved_1)(void);
	void (*_gplugin_reserved_2)(void);
	void (*_gplugin_reserved_3)(void);
	void (*_gplugin_reserved_4)(void);
};

G_BEGIN_DECLS

GType gplugin_plugin_info_get_type(void);

const gchar *gplugin_plugin_info_get_id(const GPluginPluginInfo *info);
guint32 gplugin_plugin_info_get_abi_version(const GPluginPluginInfo *info);
GPluginPluginInfoFlags gplugin_plugin_info_get_flags(const GPluginPluginInfo *info);
const gchar *gplugin_plugin_info_get_name(const GPluginPluginInfo *info);
const gchar *gplugin_plugin_info_get_version(const GPluginPluginInfo *info);
const gchar *gplugin_plugin_info_get_license(const GPluginPluginInfo *info);
const gchar *gplugin_plugin_info_get_icon(const GPluginPluginInfo *info);
const gchar *gplugin_plugin_info_get_summary(const GPluginPluginInfo *info);
const gchar *gplugin_plugin_info_get_description(const GPluginPluginInfo *info);
const gchar *gplugin_plugin_info_get_author(const GPluginPluginInfo *info);
const gchar *gplugin_plugin_info_get_website(const GPluginPluginInfo *info);
GSList *gplugin_plugin_info_get_dependencies(const GPluginPluginInfo *info);

G_END_DECLS

#endif /* GPLUGIN_PLUGIN_INFO_H */

