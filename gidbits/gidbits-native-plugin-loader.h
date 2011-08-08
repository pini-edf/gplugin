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

#ifndef GIDBITS_NATIVE_PLUGIN_LOADER_H
#define GIDBITS_NATIVE_PLUGIN_LOADER_H

#define GIDBITS_TYPE_NATIVE_PLUGIN_LOADER            (gidbits_plugin_loader_get_type())
#define GIDBITS_NATIVE_PLUGIN_LOADER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GIDBITS_TYPE_NATIVE_PLUGIN_LOADER, GidbitsNativePluginLoader))
#define GIDBITS_NATIVE_PLUGIN_LOADER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((obj), GIDBITS_TYPE_NATIVE_PLUGIN_LOADER, GidbitsNativePluginLoaderClass))
#define GIDBITS_IS_NATIVE_PLUGIN_LOADER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GIDBITS_TYPE_NATIVE_PLUGIN_LOADER))
#define GIDBITS_IS_NATIVE_PLUGIN_LOADER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((obj), GIDBITS_TYPE_NATIVE_PLUGIN_LOADER))
#define GIDBITS_NATIVE_PLUGIN_LOADER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GIDBITS_TYPE_NATIVE_PLUGIN_LOADER, GidbitsNativePluginLoaderClass))

typedef struct _GidbitsNativePluginLoader            GidbitsNativePluginLoader;
typedef struct _GidbitsNativePluginLoaderClass       GidbitsNativePluginLoaderClass;

#include <glib.h>
#include <glib-object.h>

#include <gidbits/gidbits-plugin-loader.h>

struct _GidbitsNativePluginLoader {
	GObject parent;
};

struct _GidbitsNativePluginLoaderClass {
	GObjectClass parent;

	void (*_gidbits_reserved_1)(void);
	void (*_gidbits_reserved_2)(void);
	void (*_gidbits_reserved_3)(void);
	void (*_gidbits_reserved_4)(void);
};

G_BEGIN_DECLS

GType gidbits_native_plugin_loader_get_type(void);

G_END_DECLS

#endif /* GIDBITS_NATIVE_PLUGIN_LOADER_H */

