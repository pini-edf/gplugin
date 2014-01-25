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
#ifndef GPLUGIN_SEED_LOADER_H
#define GPLUGIN_SEED_LOADER_H

#define GPLUGIN_TYPE_SEED_LOADER            (gplugin_seed_loader_get_type())
#define GPLUGIN_SEED_LOADER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GPLUGIN_TYPE_SEED_LOADER, GPluginSeedLoader))
#define GPLUGIN_SEED_LOADER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((obj), GPLUGIN_TYPE_SEED_LOADER, GPluginSeedLoaderClass))
#define GPLUGIN_IS_SEED_LOADER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GPLUGIN_TYPE_SEED_LOADER))
#define GPLUGIN_IS_SEED_LOADER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((obj), GPLUGIN_TYPE_SEED_LOADER))
#define GPLUGIN_SEED_LOADER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GPLUGIN_TYPE_SEED_LOADER, GPluginSeedLoaderClass))

typedef struct _GPluginSeedLoader            GPluginSeedLoader;
typedef struct _GPluginSeedLoaderClass       GPluginSeedLoaderClass;

#include <gplugin.h>
#include <gplugin-native.h>

struct _GPluginSeedLoader {
	GPluginLoader parent;

	void (*_gplugin_reserved_1)(void);
	void (*_gplugin_reserved_2)(void);
	void (*_gplugin_reserved_3)(void);
	void (*_gplugin_reserved_4)(void);
};

struct _GPluginSeedLoaderClass {
	GPluginLoaderClass parent;

	void (*_gplugin_reserved_1)(void);
	void (*_gplugin_reserved_2)(void);
	void (*_gplugin_reserved_3)(void);
	void (*_gplugin_reserved_4)(void);
};

G_BEGIN_DECLS

void gplugin_seed_loader_register(GPluginNativePlugin *plugin);
GType gplugin_seed_loader_get_type(void);

G_END_DECLS

#endif /* GPLUGIN_SEED_LOADER_H */

