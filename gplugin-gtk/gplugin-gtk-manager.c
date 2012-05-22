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

#include <gplugin-gtk/gplugin-gtk-manager.h>

static void
gplugin_gtk_manager_init(GPluginGtkManager *mgr) {
}

static void
gplugin_gtk_manager_class_init(GPluginGtkManagerClass *klass) {
}

G_DEFINE_TYPE(GPluginGtkManager, gplugin_gtk_manager, GTK_TYPE_BOX);

GtkWidget *
gplugin_gtk_manager_new(void) {
	return GTK_WIDGET(g_object_new(GPLUGIN_GTK_TYPE_MANAGER, NULL));
}

