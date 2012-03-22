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

#ifndef GPLUGIN_PLUGIN_MANAGER_H
#define GPLUGIN_PLUGIN_MANAGER_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

void gplugin_plugin_manager_add_path(const gchar *path_str);
void gplugin_plugin_manager_remove_path(const gchar *path_str);
GList *gplugin_plugin_manager_get_paths(void);

void gplugin_plugin_manager_register_loader(GType type);
void gplugin_plugin_manager_unregister_loader(GType type);

void gplugin_plugin_manager_refresh(void);

GSList *gplugin_plugin_manager_find_plugins(const gchar *id);
void gplugin_plugin_manager_free_plugin_list(GSList *plugins_list);

G_END_DECLS

#endif /* GPLUGIN_PLUGIN_MANAGER_H */

