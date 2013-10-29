/*
 * Copyright (C) 2011-2013 Gary Kramlich <grim@reaperworld.com>
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

#include "gplugin-lua-loader.h"

#include "gplugin-lua-plugin.h"

#include <glib/gi18n.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;
static GType type_real = 0;

/******************************************************************************
 * Helpers
 *****************************************************************************/
static gboolean
_gplugin_lua_loader_load_unload_plugin(GPluginLoader *loader,
                                       GPluginPlugin *plugin,
                                       const gchar *function, GError **error)
{
	gboolean ret;
	lua_State *L = gplugin_lua_plugin_get_state(GPLUGIN_LUA_PLUGIN(plugin));

	lua_getglobal(L, function);
	lua_pushlightuserdata(L, plugin);
	if(lua_pcall(L, 1, 1, 0) != 0) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "%s", lua_tostring(L, -1));
		}

		return FALSE;
	}

	if(!lua_isboolean(L, -1)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "%s", lua_tostring(L, -1));
		}

		return FALSE;
	}

	ret = lua_toboolean(L, -1);

	return ret;
}

/******************************************************************************
 * GPluginLoaderInterface API
 *****************************************************************************/
static GPluginPlugin *
gplugin_lua_loader_query(GPluginLoader *loader, const gchar *filename,
                         GError **error)
{
	GPluginPlugin *plugin = NULL;
	GPluginPluginInfo *info = NULL;
	lua_State *L = NULL;
	gint ret;

	L = luaL_newstate();
	luaL_openlibs(L);

	ret = luaL_loadfile(L, filename);
	if(ret != 0) {
		if(error)
			*error = g_error_new(GPLUGIN_DOMAIN, 0, "Failed to load file %s",
			                     filename);
		return NULL;
	}

	/* run the script */
	if(lua_pcall(L, 0, 0, 0) != 0) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "%s", lua_tostring(L, -1));
		}

		return NULL;
	}

	lua_getglobal(L, "gplugin_query");
	if(lua_pcall(L, 0, 1, 0) != 0) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "%s", lua_tostring(L, -1));
		}

		return NULL;
	}

	if(!lua_isuserdata(L, -1)) {
		if(error) {
			*error = g_error_new(GPLUGIN_DOMAIN, 0,
			                     "islightuserdata %s", lua_tostring(L, -1));
		}

		return NULL;
	}

	lua_getfield(L, -1, "_native");
	info = lua_touserdata(L, -1);
	lua_pop(L, 1);

	plugin = g_object_new(GPLUGIN_TYPE_LUA_PLUGIN,
	                      "filename", filename,
	                      "loader", loader,
	                      "state", L,
	                      "info", info,
	                      NULL);

	return plugin;
}

static gboolean
gplugin_lua_loader_load(GPluginLoader *loader, GPluginPlugin *plugin,
                        GError **error)
{
	return _gplugin_lua_loader_load_unload_plugin(loader, plugin,
	                                              "gplugin_load", error);
}

static gboolean
gplugin_lua_loader_unload(GPluginLoader *loader, GPluginPlugin *plugin,
                          GError **error)
{
	return _gplugin_lua_loader_load_unload_plugin(loader, plugin,
	                                              "gplugin_unload", error);
}

/******************************************************************************
 * GObject Stuff
 *****************************************************************************/
static void
gplugin_lua_loader_class_init(GPluginLuaLoaderClass *klass) {
	GPluginLoaderClass *loader_class = GPLUGIN_LOADER_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	loader_class->supported_extensions = g_slist_append(NULL, "lua");
	loader_class->query = gplugin_lua_loader_query;
	loader_class->load = gplugin_lua_loader_load;
	loader_class->unload = gplugin_lua_loader_unload;
}

/******************************************************************************
 * API
 *****************************************************************************/
void
gplugin_lua_loader_register(GPluginNativePlugin *plugin) {
	if(g_once_init_enter(&type_real)) {
		GType type = 0;

		static const GTypeInfo info = {
			.class_size = sizeof(GPluginLuaLoaderClass),
			.class_init = (GClassInitFunc)gplugin_lua_loader_class_init,
			.instance_size = sizeof(GPluginLuaLoader),
		};

		type = gplugin_native_plugin_register_type(plugin,
		                                           GPLUGIN_TYPE_LOADER,
		                                           "GPluginLuaLoader",
		                                           &info,
		                                           0);

		g_once_init_leave(&type_real, type);
	}
}

GType
gplugin_lua_loader_get_type(void) {
	if(G_UNLIKELY(type_real == 0)) {
		g_warning("gplugin_lua_loader_get_type was called before "
		          "the type was registered!\n");
	}

	return type_real;
}

