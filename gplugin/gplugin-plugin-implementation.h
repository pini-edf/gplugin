/*
 * Copyright (C) 2013 Ankit Vani <a@nevitus.org>
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

#ifndef GPLUGIN_PLUGIN_IMPLEMENTATION_H
#define GPLUGIN_PLUGIN_IMPLEMENTATION_H

#define GPLUGIN_TYPE_PLUGIN_IMPLEMENTATION            (gplugin_plugin_implementation_get_type())
#define GPLUGIN_PLUGIN_IMPLEMENTATION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GPLUGIN_TYPE_PLUGIN_IMPLEMENTATION, GPluginPluginImplementation))
#define GPLUGIN_PLUGIN_IMPLEMENTATION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), GPLUGIN_TYPE_PLUGIN_IMPLEMENTATION, GPluginPluginImplementationClass))
#define GPLUGIN_IS_PLUGIN_IMPLEMENTATION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GPLUGIN_TYPE_PLUGIN_IMPLEMENTATION))
#define GPLUGIN_IS_PLUGIN_IMPLEMENTATION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), GPLUGIN_TYPE_PLUGIN_IMPLEMENTATION))
#define GPLUGIN_PLUGIN_IMPLEMENTATION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GPLUGIN_TYPE_PLUGIN_IMPLEMENTATION, GPluginPluginImplementationClass))

typedef struct _GPluginPluginImplementation          GPluginPluginImplementation;
typedef struct _GPluginPluginImplementationClass     GPluginPluginImplementationClass;

#include <glib.h>
#include <glib-object.h>

#include <gplugin/gplugin-plugin.h>

struct _GPluginPluginImplementation {
	GObject gparent;
};

struct _GPluginPluginImplementationClass {
	GObjectClass gparent;

	void (*_gplugin_reserved_1)(void);
	void (*_gplugin_reserved_2)(void);
	void (*_gplugin_reserved_3)(void);
	void (*_gplugin_reserved_4)(void);
};

G_BEGIN_DECLS

GType gplugin_plugin_implementation_get_type(void);

GPluginPlugin *gplugin_plugin_implementation_get_plugin(GPluginPluginImplementation *implementation);

G_END_DECLS

#endif /* GPLUGIN_PLUGIN_IMPLEMENTATION_H */
