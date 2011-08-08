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
#include <gidbits/gidbits-native-plugin.h>

#define GIDBITS_NATIVE_PLUGIN_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GIDBITS_TYPE_NATIVE_PLUGIN, GidbitsNativePluginPrivate))

#define GIDBITS_LOAD_SYMBOL   "gidbits_plugin_load"
#define GIDBITS_UNLOAD_SYMBOL "gidbits_plugin_unload"

/******************************************************************************
 * Structs
 *****************************************************************************/
typedef struct _GidbitsNativePluginPrivate       GidbitsNativePluginPrivate;
typedef struct _GidbitsNativePluginTypeInfo      GidbitsNativePluginTypeInfo;
typedef struct _GidbitsNativePluginInterfaceInfo GidbitsNativePluginInterfaceInfo;

struct _GidbitsNativePluginPrivate {
	GModule *module;

	guint use_count;

	GSList *type_infos;
	GSList *interface_infos;
};

struct _GidbitsNativePluginTypeInfo {
	gboolean loaded;
	GType type;
	GType parent;
	GTypeInfo info;
};

struct _GidbitsNativePluginInterfaceInfo {
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
typedef gboolean (*GidbitsNativePluginLoadFunc)(GidbitsNativePlugin *plugin);
typedef gboolean (*GidbitsNativePluginUnloadFunc)(GidbitsNativePlugin *plugin);

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;

/******************************************************************************
 * Helpers
 *****************************************************************************/
static GidbitsNativePluginTypeInfo *
gidbits_native_plugin_find_type_info(GidbitsNativePlugin *plugin, GType type) {
	GidbitsNativePluginPrivate *priv =
		GIDBITS_NATIVE_PLUGIN_GET_PRIVATE(plugin);
	GSList *l = NULL;

	for(l = priv->type_infos; l; l = l->next) {
		GidbitsNativePluginTypeInfo *info =
			(GidbitsNativePluginTypeInfo *)(l->data);

		if(info->type == type)
			return info;
	}

	return NULL;
}

static GidbitsNativePluginInterfaceInfo *
gidbits_native_plugin_find_interface_info(GidbitsNativePlugin *plugin,
                                          GType instance_type,
                                          GType interface_type)
{
	GidbitsNativePluginPrivate *priv =
		GIDBITS_NATIVE_PLUGIN_GET_PRIVATE(plugin);
	GSList *l = NULL;

	for(l = priv->interface_infos; l; l = l->next) {
		GidbitsNativePluginInterfaceInfo *info =
			(GidbitsNativePluginInterfaceInfo *)(l->data);

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
gidbits_native_plugin_priv_use(GTypePlugin *plugin) {
	GidbitsNativePlugin *native = GIDBITS_NATIVE_PLUGIN(plugin);

	if(!gidbits_native_plugin_use(native)) {
		GidbitsPluginInfo *info =
			gidbits_plugin_get_info(GIDBITS_PLUGIN(native));
		const gchar *name = (info) ? info->name : "(unknown)";

		g_warning("Could not reload previously loaded plugin '%s'\n", name);
	}
}

static void
gidbits_native_plugin_complete_type_info(GTypePlugin *plugin, GType type,
                                         GTypeInfo *info,
                                         GTypeValueTable *value_table)
{
	GidbitsNativePlugin *native = GIDBITS_NATIVE_PLUGIN(plugin);
	GidbitsNativePluginTypeInfo *native_info =
		gidbits_native_plugin_find_type_info(native, type);

	*info = native_info->info;

	if(native_info->info.value_table)
		*value_table = *native_info->info.value_table;
}

static void
gidbits_native_plugin_complete_interface_info(GTypePlugin *plugin,
                                              GType instance_type,
                                              GType interface_type,
                                              GInterfaceInfo *info)
{
	GidbitsNativePlugin *native = GIDBITS_NATIVE_PLUGIN(plugin);
	GidbitsNativePluginInterfaceInfo *iface_info =
		gidbits_native_plugin_find_interface_info(native, instance_type,
		                                          interface_type);

	*info = iface_info->info;
}

static void
gidbits_native_plugin_iface_init(GTypePluginClass *iface) {
	iface->use_plugin = gidbits_native_plugin_priv_use;
	iface->unuse_plugin = (void (*)(GTypePlugin *))gidbits_native_plugin_unuse;
	iface->complete_type_info = gidbits_native_plugin_complete_type_info;
	iface->complete_interface_info =
		gidbits_native_plugin_complete_interface_info;
}

/******************************************************************************
 * GObject Implementation
 *****************************************************************************/
static void
gidbits_native_plugin_get_property(GObject *obj, guint param_id, GValue *value,
                                   GParamSpec *pspec)
{
	GidbitsNativePlugin *plugin = GIDBITS_NATIVE_PLUGIN(obj);

	switch(param_id) {
		case PROP_MODULE:
			g_value_set_pointer(value,
			                    gidbits_native_plugin_get_module(plugin));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gidbits_native_plugin_set_property(GObject *obj, guint param_id,
                                   const GValue *value, GParamSpec *pspec)
{
	GidbitsNativePluginPrivate *priv = GIDBITS_NATIVE_PLUGIN_GET_PRIVATE(obj);

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
gidbits_native_plugin_finalize(GObject *obj) {
	GidbitsNativePluginPrivate *priv = GIDBITS_NATIVE_PLUGIN_GET_PRIVATE(obj);

	g_slist_free(priv->type_infos);
	g_slist_free(priv->interface_infos);

	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static void
gidbits_native_plugin_class_init(GidbitsNativePluginClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	g_type_class_add_private(klass, sizeof(GidbitsNativePluginPrivate));

	obj_class->finalize = gidbits_native_plugin_finalize;
	obj_class->get_property = gidbits_native_plugin_get_property;
	obj_class->set_property = gidbits_native_plugin_set_property;

	g_object_class_install_property(obj_class, PROP_MODULE,
		g_param_spec_pointer("module", "module handle",
		                     "The GModule instance of the plugin",
		                     G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
gidbits_native_plugin_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		static const GTypeInfo info = {
			.class_size = sizeof(GidbitsNativePluginClass),
			.class_init = (GClassInitFunc)gidbits_native_plugin_class_init,
			.instance_size = sizeof(GidbitsNativePlugin),
		};

		static const GInterfaceInfo iface_info = {
			.interface_init = (GInterfaceInitFunc)gidbits_native_plugin_iface_init,
		};

		type = g_type_register_static(GIDBITS_TYPE_PLUGIN,
		                              "GidbitsNativePlugin",
		                              &info, 0);

		g_type_add_interface_static(type, G_TYPE_TYPE_PLUGIN, &iface_info);
	}

	return type;
}

gboolean
gidbits_native_plugin_use(GidbitsNativePlugin *plugin) {
	GidbitsNativePluginPrivate *priv = NULL;

	g_return_val_if_fail(GIDBITS_IS_NATIVE_PLUGIN(plugin), FALSE);

	priv = GIDBITS_NATIVE_PLUGIN_GET_PRIVATE(plugin);

	priv->use_count++;
	if(priv->use_count == 1) {
		GidbitsNativePluginLoadFunc load = NULL;
		GSList *l = NULL;
		gpointer func = NULL;

		if(!g_module_symbol(priv->module, GIDBITS_LOAD_SYMBOL, &func)) {
			priv->use_count--;

			return FALSE;
		}

		load = (GidbitsNativePluginLoadFunc)func;

		if(!(load && load(plugin))) {
			priv->use_count--;

			return FALSE;
		}

		for(l = priv->type_infos; l; l = l->next) {
			GidbitsNativePluginTypeInfo *info =
				(GidbitsNativePluginTypeInfo *)(l->data);

			if(!info->loaded) {
				GidbitsPluginInfo *plugin_info =
					gidbits_plugin_get_info(GIDBITS_PLUGIN(plugin));
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
gidbits_native_plugin_unuse(GidbitsNativePlugin *plugin) {
	GidbitsNativePluginPrivate *priv = NULL;

	g_return_val_if_fail(GIDBITS_IS_NATIVE_PLUGIN(plugin), FALSE);

	priv = GIDBITS_NATIVE_PLUGIN_GET_PRIVATE(plugin);

	g_return_val_if_fail(priv->use_count > 0, FALSE);

	priv->use_count--;

	if(priv->use_count == 0) {
		GidbitsNativePluginUnloadFunc unload = NULL;
		GSList *l = NULL;
		gpointer func = NULL;

		if(!g_module_symbol(priv->module, GIDBITS_UNLOAD_SYMBOL, &func)) {
			priv->use_count++;

			return FALSE;
		}

		unload = (GidbitsNativePluginUnloadFunc)func;

		if(!(unload && unload(plugin))) {
			priv->use_count++;

			return FALSE;
		}

		for(l = priv->type_infos; l; l = l->next) {
			GidbitsNativePluginTypeInfo *info =
				(GidbitsNativePluginTypeInfo *)(l->data);

			info->loaded = FALSE;

			return FALSE;
		}
	}

	return TRUE;
}

GType
gidbits_native_plugin_register_type(GidbitsNativePlugin *plugin, GType parent,
                                    const gchar *name, const GTypeInfo *info,
                                    GTypeFlags flags)
{
	GidbitsNativePluginPrivate *priv = NULL;
	GidbitsNativePluginTypeInfo *type_info = NULL;
	GType type = G_TYPE_INVALID;

	g_return_val_if_fail(GIDBITS_IS_NATIVE_PLUGIN(plugin), G_TYPE_INVALID);
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
		type_info = gidbits_native_plugin_find_type_info(plugin, type);

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
		type_info = g_new(GidbitsNativePluginTypeInfo, 1);

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

	return type_info->type;
}

void
gidbits_native_plugin_add_interface(GidbitsNativePlugin *plugin,
                                    GType instance_type, GType interface_type,
                                    const GInterfaceInfo *interface_info)
{
	GidbitsNativePluginPrivate *priv = NULL;
	GidbitsNativePluginInterfaceInfo *iface_info = NULL;

	g_return_if_fail(GIDBITS_IS_NATIVE_PLUGIN(plugin));
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

		iface_info = gidbits_native_plugin_find_interface_info(plugin,
		                                                       instance_type,
		                                                       interface_type);

		g_return_if_fail(iface_info);
	} else {
		iface_info = g_new0(GidbitsNativePluginInterfaceInfo, 1);

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
gidbits_native_plugin_register_enum(GidbitsNativePlugin *plugin,
                                    const gchar *name,
                                    const GEnumValue *values)
{
	GTypeInfo enum_info = { 0, };

	g_return_val_if_fail(GIDBITS_IS_NATIVE_PLUGIN(plugin), G_TYPE_INVALID);
	g_return_val_if_fail(name, G_TYPE_INVALID);
	g_return_val_if_fail(values, G_TYPE_INVALID);

	g_enum_complete_type_info(G_TYPE_ENUM, &enum_info, values);

	return gidbits_native_plugin_register_type(plugin, G_TYPE_ENUM, name,
	                                           &enum_info, 0);
}

GType
gidbits_native_plugin_register_flags(GidbitsNativePlugin *plugin,
                                     const gchar *name,
                                     const GFlagsValue *values)
{
	GTypeInfo flags_info = { 0, };

	g_return_val_if_fail(GIDBITS_IS_NATIVE_PLUGIN(plugin), G_TYPE_INVALID);
	g_return_val_if_fail(name, G_TYPE_INVALID);
	g_return_val_if_fail(values, G_TYPE_INVALID);

	g_flags_complete_type_info(G_TYPE_FLAGS, &flags_info, values);

	return gidbits_native_plugin_register_type(plugin, G_TYPE_FLAGS, name,
	                                           &flags_info, 0);
}

GModule *
gidbits_native_plugin_get_module(GidbitsNativePlugin *plugin) {
	GidbitsNativePluginPrivate *priv = NULL;

	g_return_val_if_fail(GIDBITS_IS_NATIVE_PLUGIN(plugin), NULL);

	priv = GIDBITS_NATIVE_PLUGIN_GET_PRIVATE(plugin);

	return priv->module;
}

