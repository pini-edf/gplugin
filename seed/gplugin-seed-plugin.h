/*
 * Copyright (C) 2011-2014 Gary Kramlich <grim@reaperworld.com>
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
#ifndef GPLUGIN_SEED_PLUGIN_H
#define GPLUGIN_SEED_PLUGIN_H

#define GPLUGIN_TYPE_SEED_PLUGIN            (gplugin_seed_plugin_get_type())
#define GPLUGIN_SEED_PLUGIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GPLUGIN_TYPE_SEED_PLUGIN, GPluginSeedPlugin))
#define GPLUGIN_SEED_PLUGIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((obj), GPLUGIN_TYPE_SEED_PLUGIN, GPluginSeedPluginClass))
#define GPLUGIN_IS_SEED_PLUGIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GPLUGIN_TYPE_SEED_PLUGIN))
#define GPLUGIN_IS_SEED_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((obj), GPLUGIN_TYPE_SEED_PLUGIN))
#define GPLUGIN_SEED_PLUGIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GPLUGIN_TYPE_SEED_PLUGIN, GPluginSeedPluginClass))

typedef struct _GPluginSeedPlugin            GPluginSeedPlugin;
typedef struct _GPluginSeedPluginClass       GPluginSeedPluginClass;

#include <gplugin.h>
#include <gplugin-native.h>

#include <seed.h>

struct _GPluginSeedPlugin {
	GPluginPlugin parent;
};

struct _GPluginSeedPluginClass {
	GPluginPluginClass parent;

	void (*_gplugin_reserved_1)(void);
	void (*_gplugin_reserved_2)(void);
	void (*_gplugin_reserved_3)(void);
	void (*_gplugin_reserved_4)(void);
};

G_BEGIN_DECLS

void gplugin_seed_plugin_register(GPluginNativePlugin *plugin);
GType gplugin_seed_plugin_get_type(void);

SeedContext gplugin_seed_plugin_get_context(const GPluginSeedPlugin *plugin);
SeedObject gplugin_seed_plugin_get_load_func(const GPluginSeedPlugin *plugin);
SeedObject gplugin_seed_plugin_get_unload_func(const GPluginSeedPlugin *plugin);

G_END_DECLS

#endif /* GPLUGIN_SEED_PLUGIN_H */

