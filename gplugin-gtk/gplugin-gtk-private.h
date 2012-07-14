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
#if !defined(GPLUGIN_GTK_GLOBAL_HEADER_INSIDE) && !defined(GPLUGIN_GTK_COMPILATION)
#error "only <gplugin/gplugin-gtk.h> may be included directly"
#endif
#endif /* __GI_SCANNER__ */

#ifndef GPLUGIN_GTK_PRIVATE_H
#define GPLUGIN_GTK_PRIVATE_H

#include <glib.h>
#include <glib-object.h>

typedef enum {
	GPLUGIN_GTK_STORE_ENABLED_COLUMN,
	GPLUGIN_GTK_STORE_CAN_ENABLED_COLUMN,
	GPLUGIN_GTK_STORE_INFO_COLUMN,
	GPLUGIN_GTK_STORE_PLUGIN_COLUMN,
	GPLUGIN_GTK_STORE_N_COLUMNS,
} GPluginGtkStoreColumns;

G_BEGIN_DECLS

const GType *gplugin_gtk_get_store_column_types(void);

G_END_DECLS

#endif /* GPLUGIN_GTK_PRIVATE_H */

