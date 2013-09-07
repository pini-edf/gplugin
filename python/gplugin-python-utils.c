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

#include <gplugin/gplugin.h>

#include "gplugin-python-utils.h"

#include <string.h>

#include <Python.h>

gchar *
gplugin_python_filename_to_module(const gchar *filename) {
	gchar *base = NULL;
	gchar *e = NULL, *r = NULL;

	g_return_val_if_fail(filename != NULL, NULL);

	/* first make sure we just have a filename */
	base = g_path_get_basename(filename);

	/* now find the last . for the extension */
	e = g_utf8_strrchr(base, g_utf8_strlen(base, -1), g_utf8_get_char("."));
	if(e == NULL) {
		return base;
	}

	/* now copy the module name into r */
	r = g_malloc(e - base + 1);
	memcpy(r, base, e - base);
	r[e - base] = 0;

	/* free the basename */
	g_free(base);

	return r;
}

gboolean
gplugin_python_add_module_path(const gchar *module_path) {
	PyObject *sys_path = NULL, *path = NULL;
	gboolean ret = FALSE;

	sys_path = PySys_GetObject("path");

	path = PyString_FromString(module_path);

	if(PySequence_Contains(sys_path, path) == 0) {
		PyList_Insert(sys_path, 0, path);
		ret = TRUE;
	}

	Py_DECREF(path);

	return ret;
}

GError *
gplugin_python_exception_to_gerror(void) {
	GError *error = NULL;
	PyObject *type = NULL, *value = NULL, *trace = NULL;
	PyObject *type_name = NULL, *value_str = NULL;

	if(!PyErr_Occurred())
		return NULL;

	PyErr_Fetch(&type, &value, &trace);
	if(type == NULL)
		return NULL;

	PyErr_NormalizeException(&type, &value, &trace);
	Py_XDECREF(trace);

	type_name = PyObject_GetAttrString(type, "__name__");
	Py_DECREF(type);

	value_str = PyObject_Str(value);
	Py_DECREF(value);

	error = g_error_new(GPLUGIN_DOMAIN, 0, "%s: %s",
	                    PyString_AsString(type_name),
	                    PyString_AsString(value_str));

	Py_DECREF(type_name);
	Py_DECREF(value_str);

	return error;
}

