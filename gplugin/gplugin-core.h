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
#if !defined(GPLUGIN_H_INSIDE) && !defined(GPLUGIN_COMPILATION)
#error "only <gplugin.h> may be included directly"
#endif

#ifndef GPLUGIN_CORE_H
#define GPLUGIN_CORE_H

#include <glib.h>

#define GPLUGIN_DOMAIN    (g_quark_from_static_string("gplugin"))

G_BEGIN_DECLS

void gplugin_init(void);

G_END_DECLS

#endif /* GPLUGIN_CORE_H */

