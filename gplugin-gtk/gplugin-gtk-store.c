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

#include <gplugin-gtk/gplugin-gtk-store.h>

#include <gplugin-gtk/gplugin-gtk-private.h>

#define GPLUGIN_GTK_STORE_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GPLUGIN_GTK_TYPE_STORE, GPluginGtkStorePrivate))

/******************************************************************************
 * Structs
 *****************************************************************************/
typedef struct {
	gboolean dummy;
} GPluginGtkStorePrivate;

/******************************************************************************
 * Enums
 *****************************************************************************/

/******************************************************************************
 * Globals
 *****************************************************************************/

/******************************************************************************
 * GObject Stuff
 *****************************************************************************/
G_DEFINE_TYPE(GPluginGtkStore, gplugin_gtk_store, GTK_TYPE_LIST_STORE);

static void
gplugin_gtk_store_constructed(GObject *obj) {
	G_OBJECT_CLASS(gplugin_gtk_store_parent_class)->constructed(obj);
}

static void
gplugin_gtk_store_dispose(GObject *obj) {
	G_OBJECT_CLASS(gplugin_gtk_store_parent_class)->dispose(obj);
}

static void
gplugin_gtk_store_init(GPluginGtkStore *store) {
	GType *types = (GType *)gplugin_gtk_get_store_column_types();

	gtk_list_store_set_column_types(GTK_LIST_STORE(store),
	                                GPLUGIN_GTK_STORE_N_COLUMNS,
	                                types);
}

static void
gplugin_gtk_store_class_init(GPluginGtkStoreClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	g_type_class_add_private(obj_class, sizeof(GPluginGtkStorePrivate));

	obj_class->constructed = gplugin_gtk_store_constructed;
	obj_class->dispose = gplugin_gtk_store_dispose;

}

/******************************************************************************
 * API
 *****************************************************************************/
GPluginGtkStore *
gplugin_gtk_store_new(void) {
	return GPLUGIN_GTK_STORE(g_object_new(GPLUGIN_GTK_TYPE_STORE, NULL));
}

