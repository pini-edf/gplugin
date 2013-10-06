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

#ifndef GPLUGIN_PRIVATE_H
#define GPLUGIN_PRIVATE_H

#include <glib.h>
#include <glib-object.h>

#include <gplugin/gplugin-plugin.h>

G_BEGIN_DECLS

gchar *gplugin_plugin_get_internal_filename(GPluginPlugin *plugin);

void gplugin_plugin_manager_private_init(void);
void gplugin_plugin_manager_private_uninit(void);

gboolean gplugin_boolean_accumulator(GSignalInvocationHint *hint, GValue *return_accu, const GValue *handler_return, gpointer data);

G_END_DECLS

#endif /* GPLUGIN_PRIVATE_H */

