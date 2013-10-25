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

#ifndef __GI_SCANNER__ /* hide this bit from g-ir-scanner */
#if !defined(GPLUGIN_GTK_GLOBAL_HEADER_INSIDE) && !defined(GPLUGIN_GTK_COMPILATION)
#error "only <gplugin/gplugin-gtk.h> may be included directly"
#endif
#endif /* __GI_SCANNER__ */

#ifndef GPLUGIN_GTK_PLUGIN_INFO_H
#define GPLUGIN_GTK_PLUGIN_INFO_H

#define GPLUGIN_GTK_TYPE_PLUGIN_INFO            (gplugin_gtk_plugin_info_get_type())
#define GPLUGIN_GTK_PLUGIN_INFO(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GPLUGIN_GTK_TYPE_PLUGIN_INFO, GPluginGtkPluginInfo))
#define GPLUGIN_GTK_PLUGIN_INFO_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), GPLUGIN_GTK_TYPE_PLUGIN_INFO, GPluginGtkPluginInfoClass))
#define GPLUGIN_GTK_IS_PLUGIN_INFO(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GPLUGIN_GTK_TYPE_PLUGIN_INFO))
#define GPLUGIN_GTK_IS_PLUGIN_INFO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), GPLUGIN_GTK_TYPE_PLUGIN_INFO))
#define GPLUING_GTK_PLUGIN_INFO_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GPLUGIN_GTK_TYPE_PLUGIN_INFO, GPluginGtkPluginInfoClass))

typedef struct _GPluginGtkPluginInfo           GPluginGtkPluginInfo;
typedef struct _GPluginGtkPluginInfoClass      GPluginGtkPluginInfoClass;

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

struct _GPluginGtkPluginInfo {
	GtkGrid parent;

	void (*_gplugin_reserved1)(void);
	void (*_gplugin_reserved2)(void);
	void (*_gplugin_reserved3)(void);
	void (*_gplugin_reserved4)(void);
};

struct _GPluginGtkPluginInfoClass {
	GtkGridClass parent;

	void (*_gplugin_reserved1)(void);
	void (*_gplugin_reserved2)(void);
	void (*_gplugin_reserved3)(void);
	void (*_gplugin_reserved4)(void);
};

G_BEGIN_DECLS

GType gplugin_gtk_plugin_info_get_type(void);

GtkWidget *gplugin_gtk_plugin_info_new(void);

void gplugin_gtk_plugin_info_set_show_internal(GPluginGtkPluginInfo *view, gboolean show_internal);
gboolean gplugin_gtk_plugin_info_get_show_internal(const GPluginGtkPluginInfo *view);

G_END_DECLS

#endif /* GPLUGIN_GTK_PLUGIN_INFO_H */

