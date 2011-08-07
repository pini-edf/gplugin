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

#ifndef GIDBITS_PLUGINS_H
#define GIDBITS_PLUGINS_H

#include <glib.h>

G_BEGIN_DECLS

void gidbits_plugins_add_path(const gchar *path_str);
void gidbits_plugins_remove_path(const gchar *path_str);
GList *gidbits_plugins_get_paths(void);

G_END_DECLS

#endif /* GIDBITS_PLUGINS_H */

