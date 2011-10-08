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
#include <gplugin/gplugin-native-plugin.h>

#include <gplugin/gplugin-plugin-loader.h>
#include <gplugin/gplugin-plugin-manager.h>

#define GPLUGIN_NATIVE_PLUGIN_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GPLUGIN_TYPE_NATIVE_PLUGIN, GPluginNativePluginPrivate))

#define GPLUGIN_LOAD_SYMBOL   "gplugin_plugin_load"
#define GPLUGIN_UNLOAD_SYMBOL "gplugin_plugin_unload"

/******************************************************************************
 * Structs
 *****************************************************************************/
typedef struct _GPluginNativePluginPrivate       GPluginNativePluginPrivate;
typedef struct _GPluginNativePluginTypeInfo      GPluginNativePluginTypeInfo;
typedef struct _GPluginNativePluginInterfaceInfo GPluginNativePluginInterfaceInfo;

struct _GPluginNativePluginPrivate {
	GModule *module;

	guint use_count;

	GSList *type_infos;
	GSList *interface_infos;
};

struct _GPluginNativePluginTypeInfo {
	gboolean loaded;
	GType type;
	GType parent;
	GTypeInfo info;
};

struct _GPluginNativePluginInterfaceInfo {
	gboolean loaded;
	GType instance_type;
	GType interface_type;
	GInterfaceInfo info;
};

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	PROP_MODULE,
	PROP_LAST,
};

/******************************************************************************
 * Functions
 *****************************************************************************/
typedef gboolean (*GPluginNativePluginLoadFunc)(GPluginNativePlugin *plugin);
typedef gboolean (*GPluginNativePluginUnloadFunc)(GPluginNativePlugin *plugin);

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;

/******************************************************************************
 * Helpers
 *****************************************************************************/
static GPluginNativePluginTypeInfo *
gplugin_native_plugin_find_type_info(GPluginNativePlugin *plugin, GType type) {
	GPluginNativePluginPrivate *priv =
		GPLUGIN_NATIVE_PLUGIN_GET_PRIVATE(plugin);
	GSList *l = NULL;

	for(l = priv->type_infos; l; l = l->next) {
		GPluginNativePluginTypeInfo *info =
			(GPluginNativePluginTypeInfo *)(l->data);

		if(info->type == type)
			return info;
	}

	return NULL;
}

static GPluginNativePluginInterfaceInfo *
gplugin_native_plugin_find_interface_info(GPluginNativePlugin *plugin,
                                          GType instance_type,
                                          GType interface_type)
{
	GPluginNativePluginPrivate *priv =
		GPLUGIN_NATIVE_PLUGIN_GET_PRIVATE(plugin);
	GSList *l = NULL;

	for(l = priv->interface_infos; l; l = l->next) {
		GPluginNativePluginInterfaceInfo *info =
			(GPluginNativePluginInterfaceInfo *)(l->data);

		if(info->instance_type == instance_type &&
		   info->interface_type == interface_type)
		{
			return info;
		}
	}

	return NULL;
}

/******************************************************************************
 * GTypePlugin Implementation
 *****************************************************************************/
static void
gplugin_native_plugin_priv_use(GTypePlugin *plugin) {
	GPluginNativePlugin *native = GPLUGIN_NATIVE_PLUGIN(plugin);

	if(!gplugin_native_plugin_use(native)) {
		GPluginPluginInfo *info =
			gplugin_plugin_get_info(GPLUGIN_PLUGIN(native));
		const gchar *name = (info) ? info->name : "(unknown)";

		g_warning("Could not reload previously loaded plugin '%s'\n", name);
	}
}

static void
gplugin_native_plugin_complete_type_info(GTypePlugin *plugin, GType type,
                                         GTypeInfo *info,
                                         GTypeValueTable *value_table)
{
	GPluginNativePlugin *native = GPLUGIN_NATIVE_PLUGIN(plugin);
	GPluginNativePluginTypeInfo *native_info =
		gplugin_native_plugin_find_type_info(native, type);

	*info = native_info->info;

	if(native_info->info.value_table)
		*value_table = *native_info->info.value_table;
}

static void
gplugin_native_plugin_complete_interface_info(GTypePlugin *plugin,
                                              GType instance_type,
                                              GType interface_type,
                                              GInterfaceInfo *info)
{
	GPluginNativePlugin *native = GPLUGIN_NATIVE_PLUGIN(plugin);
	GPluginNativePluginInterfaceInfo *iface_info =
		gplugin_native_plugin_find_interface_info(native, instance_type,
		                                          interface_type);

	*info = iface_info->info;
}

static void
gplugin_native_plugin_iface_init(GTypePluginClass *iface) {
	iface->use_plugin = gplugin_native_plugin_priv_use;
	iface->unuse_plugin = (void (*)(GTypePlugin *))gplugin_native_plugin_unuse;
	iface->complete_type_info = gplugin_native_plugin_complete_type_info;
	iface->complete_interface_info =
		gplugin_native_plugin_complete_interface_info;
}

/******************************************************************************
 * GObject Implementation
 *****************************************************************************/
static void
gplugin_native_plugin_get_property(GObject *obj, guint param_id, GValue *value,
                                   GParamSpec *pspec)
{
	GPluginNativePlugin *plugin = GPLUGIN_NATIVE_PLUGIN(obj);

	switch(param_id) {
		case PROP_MODULE:
			g_value_set_pointer(value,
			                    gplugin_native_plugin_get_module(plugin));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_native_plugin_set_property(GObject *obj, guint param_id,
                                   const GValue *value, GParamSpec *pspec)
{
	GPluginNativePluginPrivate *priv = GPLUGIN_NATIVE_PLUGIN_GET_PRIVATE(obj);

	switch(param_id) {
		case PROP_MODULE:
			priv->module = g_value_get_pointer(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_native_plugin_finalize(GObject *obj) {
	GPluginNativePluginPrivate *priv = GPLUGIN_NATIVE_PLUGIN_GET_PRIVATE(obj);

	g_slist_free(priv->type_infos);
	g_slist_free(priv->interface_infos);

	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static void
gplugin_native_plugin_class_init(GPluginNativePluginClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	g_type_class_add_private(klass, sizeof(GPluginNativePluginPrivate));

	obj_class->finalize = gplugin_native_plugin_finalize;
	obj_class->get_property = gplugin_native_plugin_get_property;
	obj_class->set_property = gplugin_native_plugin_set_property;

	g_object_class_install_property(obj_class, PROP_MODULE,
		g_param_spec_pointer("module", "module handle",
		                     "The GModule instance of the plugin",
		                     G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
gplugin_native_plugin_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		static const GTypeInfo info = {
			.class_size = sizeof(GPluginNativePluginClass),
			.class_init = (GClassInitFunc)gplugin_native_plugin_class_init,
			.instance_size = sizeof(GPluginNativePlugin),
		};

		static const GInterfaceInfo iface_info = {
			.interface_init = (GInterfaceInitFunc)gplugin_native_plugin_iface_init,
		};

		type = g_type_register_static(GPLUGIN_TYPE_PLUGIN,
		                              "GPluginNativePlugin",
		                              &info, 0);

		g_type_add_interface_static(type, G_TYPE_TYPE_PLUGIN, &iface_info);
	}

	return type;
}

gboolean
gplugin_native_plugin_use(GPluginNativePlugin *plugin) {
	GPluginNativePluginPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_NATIVE_PLUGIN(plugin), FALSE);

	priv = GPLUGIN_NATIVE_PLUGIN_GET_PRIVATE(plugin);

	priv->use_count++;
	if(priv->use_count == 1) {
		GPluginNativePluginLoadFunc load = NULL;
		GSList *l = NULL;
		gpointer func = NULL;

		if(!g_module_symbol(priv->module, GPLUGIN_LOAD_SYMBOL, &func)) {
			priv->use_count--;

			return FALSE;
		}

		load = (GPluginNativePluginLoadFunc)func;

		if(!(load && load(plugin))) {
			priv->use_count--;

			return FALSE;
		}

		for(l = priv->type_infos; l; l = l->next) {
			GPluginNativePluginTypeInfo *info =
				(GPluginNativePluginTypeInfo *)(l->data);

			if(!info->loaded) {
				GPluginPluginInfo *plugin_info =
					gplugin_plugin_get_info(GPLUGIN_PLUGIN(plugin));
				const gchar *name = (plugin_info) ? plugin_info->name
				                                  : "(unknown)";

				g_warning("plugin '%s' failed to register type '%s'\n",
				          name, g_type_name(info->type));

				return FALSE;
			}
		}
	}

	return TRUE;
}

gboolean
gplugin_native_plugin_unuse(GPluginNativePlugin *plugin) {
	GPluginNativePluginPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_NATIVE_PLUGIN(plugin), FALSE);

	priv = GPLUGIN_NATIVE_PLUGIN_GET_PRIVATE(plugin);

	g_return_val_if_fail(priv->use_count > 0, FALSE);

	priv->use_count--;

	if(priv->use_count == 0) {
		GPluginNativePluginUnloadFunc unload = NULL;
		GSList *l = NULL;
		gpointer func = NULL;

		if(!g_module_symbol(priv->module, GPLUGIN_UNLOAD_SYMBOL, &func)) {
			priv->use_count++;

			return FALSE;
		}

		unload = (GPluginNativePluginUnloadFunc)func;

		if(!(unload && unload(plugin))) {
			priv->use_count++;

			return FALSE;
		}

		for(l = priv->type_infos; l; l = l->next) {
			GPluginNativePluginTypeInfo *info =
				(GPluginNativePluginTypeInfo *)(l->data);

			info->loaded = FALSE;

			return FALSE;
		}
	}

	return TRUE;
}

GType
gplugin_native_plugin_register_type(GPluginNativePlugin *plugin, GType parent,
                                    const gchar *name, const GTypeInfo *info,
                                    GTypeFlags flags)
{
	GPluginNativePluginPrivate *priv = NULL;
	GPluginNativePluginTypeInfo *type_info = NULL;
	GType type = G_TYPE_INVALID;

	g_return_val_if_fail(GPLUGIN_IS_NATIVE_PLUGIN(plugin), G_TYPE_INVALID);
	g_return_val_if_fail(name, G_TYPE_INVALID);
	g_return_val_if_fail(info, G_TYPE_INVALID);

	type = g_type_from_name(name);

	if(type != G_TYPE_INVALID) {
		/* this type is already loaded, let's make sure it's the same plugin
		 * loading it again.
		 */
		GTypePlugin *old = g_type_get_plugin(type);

		if(old != G_TYPE_PLUGIN(plugin)) {
			g_warning("Two different plugins tried to register '%s'\n",
			          name);

			return G_TYPE_INVALID;
		}

		/* The same plugin is reloading the type */
		type_info = gplugin_native_plugin_find_type_info(plugin, type);

		if(type_info->parent != parent) {
			/* eek, them bastards tried to reparent this type! */

			const gchar *parent_name = g_type_name(parent);

			g_warning("Type '%s' was recreated with a different parent type.\n"
			          "(was '%s', now '%s')\n",
			          name, g_type_name(type_info->parent),
			          (parent_name) ? parent_name : "(unknown)");

			return G_TYPE_INVALID;
		}

		/* we need to free the old vtable if the old type had one */
		if(type_info->info.value_table)
			g_free((GTypeValueTable *)type_info->info.value_table);
	} else {
		/* the type hasn't been loaded before, so now we need to add it */
		type_info = g_new(GPluginNativePluginTypeInfo, 1);

		type_info->parent = parent;
		type_info->type = g_type_register_dynamic(parent, name,
		                                          G_TYPE_PLUGIN(plugin),
		                                          flags);

		priv->type_infos = g_slist_prepend(priv->type_infos, type_info);
	}

	/* ok, now finish up */
	type_info->loaded = TRUE;
	type_info->info = *info;

	if(info->value_table) {
		type_info->info.value_table = g_memdup(info->value_table,
		                                       sizeof(GTypeValueTable));
	}

	if(g_type_is_a(type_info->type, GPLUGIN_TYPE_PLUGIN_LOADER) &&
	   !G_TYPE_IS_ABSTRACT(type_info->type))
	{
		gplugin_plugin_manager_register_loader(type_info->type);
	}

	return type_info->type;
}

void
gplugin_native_plugin_add_interface(GPluginNativePlugin *plugin,
                                    GType instance_type, GType interface_type,
                                    const GInterfaceInfo *interface_info)
{
	GPluginNativePluginPrivate *priv = NULL;
	GPluginNativePluginInterfaceInfo *iface_info = NULL;

	g_return_if_fail(GPLUGIN_IS_NATIVE_PLUGIN(plugin));
	g_return_if_fail(interface_info);

	if(g_type_is_a(instance_type, interface_type)) {
		GTypePlugin *old = g_type_interface_get_plugin(instance_type,
		                                               interface_type);

		if(!old) {
			g_warning("Interface '%s' for '%s' was previously registered "
			          "statically or for a parent type.",
			          g_type_name(interface_type),
			          g_type_name(instance_type));
			return;
		} else if(old != G_TYPE_PLUGIN(plugin)) {
			g_warning("Two different plugins tried to register interface "
			          "'%s' for '%s'\n",
			          g_type_name(interface_type),
			          g_type_name(instance_type));
			return;
		}

		iface_info = gplugin_native_plugin_find_interface_info(plugin,
		                                                       instance_type,
		                                                       interface_type);

		g_return_if_fail(iface_info);
	} else {
		iface_info = g_new0(GPluginNativePluginInterfaceInfo, 1);

		iface_info->instance_type = instance_type;
		iface_info->interface_type = interface_type;

		g_type_add_interface_dynamic(instance_type, interface_type,
		                             G_TYPE_PLUGIN(plugin));

		priv->interface_infos = g_slist_prepend(priv->interface_infos,
		                                        iface_info);
	}

	iface_info->loaded = TRUE;
	iface_info->info = *interface_info;
}

GType
gplugin_native_plugin_register_enum(GPluginNativePlugin *plugin,
                                    const gchar *name,
                                    const GEnumValue *values)
{
	GTypeInfo enum_info = { 0, };

	g_return_val_if_fail(GPLUGIN_IS_NATIVE_PLUGIN(plugin), G_TYPE_INVALID);
	g_return_val_if_fail(name, G_TYPE_INVALID);
	g_return_val_if_fail(values, G_TYPE_INVALID);

	g_enum_complete_type_info(G_TYPE_ENUM, &enum_info, values);

	return gplugin_native_plugin_register_type(plugin, G_TYPE_ENUM, name,
	                                           &enum_info, 0);
}

GType
gplugin_native_plugin_register_flags(GPluginNativePlugin *plugin,
                                     const gchar *name,
                                     const GFlagsValue *values)
{
	GTypeInfo flags_info = { 0, };

	g_return_val_if_fail(GPLUGIN_IS_NATIVE_PLUGIN(plugin), G_TYPE_INVALID);
	g_return_val_if_fail(name, G_TYPE_INVALID);
	g_return_val_if_fail(values, G_TYPE_INVALID);

	g_flags_complete_type_info(G_TYPE_FLAGS, &flags_info, values);

	return gplugin_native_plugin_register_type(plugin, G_TYPE_FLAGS, name,
	                                           &flags_info, 0);
}

GModule *
gplugin_native_plugin_get_module(GPluginNativePlugin *plugin) {
	GPluginNativePluginPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_NATIVE_PLUGIN(plugin), NULL);

	priv = GPLUGIN_NATIVE_PLUGIN_GET_PRIVATE(plugin);

	return priv->module;
}

