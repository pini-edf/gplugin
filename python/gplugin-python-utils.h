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
#ifndef GPLUGIN_PYTHON_UTILS_H
#define GPLUGIN_PYTHON_UTILS_H

#include <glib.h>

G_BEGIN_DECLS

gchar *gplugin_python_filename_to_module(const gchar *filename);

gboolean gplugin_python_add_module_path(const gchar *module_path);

G_END_DECLS

#endif /* GPLUGIN_PYTHON_UTILS_H */
