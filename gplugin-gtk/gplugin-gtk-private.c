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

#include <gplugin-gtk/gplugin-gtk-private.h>

static const GType column_types[] = {
	G_TYPE_BOOLEAN,
	G_TYPE_BOOLEAN,
	G_TYPE_BOXED,
	G_TYPE_OBJECT,
};

G_STATIC_ASSERT(G_N_ELEMENTS(column_types) == GPLUGIN_GTK_STORE_N_COLUMNS);

/******************************************************************************
 * API
 *****************************************************************************/
const GType *
gplugin_gtk_get_store_column_types(void) {
	return column_types;
}

