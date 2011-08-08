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

#ifndef GIDBITS_NATIVE_PLUGIN_H
#define GIDBITS_NATIVE_PLUGIN_H

#define GIDBITS_TYPE_NATIVE_PLUGIN            (gidbits_plugin_get_type())
#define GIDBITS_NATIVE_PLUGIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GIDBITS_TYPE_NATIVE_PLUGIN, GidbitsNativePlugin))
#define GIDBITS_NATIVE_PLUGIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((obj), GIDBITS_TYPE_NATIVE_PLUGIN, GidbitsNativePluginClass))
#define GIDBITS_IS_NATIVE_PLUGIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GIDBITS_TYPE_NATIVE_PLUGIN))
#define GIDBITS_IS_NATIVE_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((obj), GIDBITS_TYPE_NATIVE_PLUGIN))
#define GIDBITS_NATIVE_PLUGIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GIDBITS_TYPE_NATIVE_PLUGIN, GidbitsNativePluginClass))

typedef struct _GidbitsNativePlugin           GidbitsNativePlugin;
typedef struct _GidbitsNativePluginClass      GidbitsNativePluginClass;

#include <gidbits/gidbits-plugin.h>

#include <gmodule.h>

#define GIDBITS_NATIVE_PLUGIN_ABI_VERSION     0x00000001

struct _GidbitsNativePlugin {
	GObject parent;
};

struct _GidbitsNativePluginClass {
	GObjectClass parent;

	void (*_gidbits_reserved_1)(void);
	void (*_gidbits_reserved_2)(void);
	void (*_gidbits_reserved_3)(void);
	void (*_gidbits_reserved_4)(void);
};

G_BEGIN_DECLS

GType gidbits_native_plugin_get_type(void);

gboolean gidbits_native_plugin_use(GidbitsNativePlugin *plugin);
gboolean gidbits_native_plugin_unuse(GidbitsNativePlugin *plugin);

GType gidbits_native_plugin_register_type(GidbitsNativePlugin *plugin, GType parent, const gchar *name, const GTypeInfo *info, GTypeFlags flags);
void gidbits_native_plugin_add_interface(GidbitsNativePlugin *plugin, GType instance_type, GType interface_type, const GInterfaceInfo *interface_info);
GType gidbits_native_plugin_register_enum(GidbitsNativePlugin *plugin, const gchar *name, const GEnumValue *values);
GType gidbits_native_plugin_register_flags(GidbitsNativePlugin *plugin, const gchar *name, const GFlagsValue *values);

GModule *gidbits_native_plugin_get_module(GidbitsNativePlugin *plugin);

G_END_DECLS

#endif /* GIDBITS_NATIVE_PLUGIN_H */

