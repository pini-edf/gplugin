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
#error "only <gpluin/gtk.h> may be included directly"
#endif
#endif /* __GI_SCANNER__ */

#ifndef GPLUGIN_GTK_MANAGER_H
#define GPLUGIN_GTK_MANAGER_H

#define GPLUGIN_GTK_TYPE_MANAGER            (gplugin_gtk_manager_get_type())
#define GPLUGIN_GTK_MANAGER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GPLUGIN_GTK_TYPE_MANAGER, GPluginGtkManager))
#define GPLUGIN_GTK_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), GPLUGIN_GTK_TYPE_MANAGER, GPluginGtkManagerClass))
#define GPLUGIN_GTK_IS_MANAGER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GPLUGIN_GTK_TYPE_MANAGER))
#define GPLUGIN_GTK_IS_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), GPLUGIN_GTK_TYPE_MANAGER))
#define GPLUING_GTK_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GPLUGIN_GTK_TYPE_MANAGER, GPluginGtkManagerClass))

typedef struct _GPluginGtkManager           GPluginGtkManager;
typedef struct _GPluginGtkManagerClass      GPluginGtkManagerClass;

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

struct _GPluginGtkManager {
	GtkBox parent;
};

struct _GPluginGtkManagerClass {
	GtkBoxClass parent;
};

G_BEGIN_DECLS

GtkWidget *gplugin_gtk_manager_new(void);

G_END_DECLS

#endif /* GPLUGIN_GTK_MANAGER_H */

