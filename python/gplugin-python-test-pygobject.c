/*
 * Copyright (C) 2011-2014 Gary Kramlich <grim@reaperworld.com>
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
#include <stdio.h>

#include <glib.h>

#include <Python.h>

#include <pygobject.h>

gint
main(gint argc, gchar *argv[]) {
	wchar_t *wargv[] = { NULL, NULL };
	size_t len;

	/* initialize python */
	if(!Py_IsInitialized())
		Py_InitializeEx(FALSE);

	/* setup wargv */
	len = mbstowcs(NULL, argv[0], 0);
	if(len == (size_t)-1)
		return -1;

	wargv[0] = g_new0(wchar_t, len + 1);
	len = mbstowcs(wargv[0], argv[0], len + 1);
	if(len == (size_t)-1)
		return -1;

	/* setup sys.path */
#if PY_VERSION_HEX < 0x03010300
	PySys_SetArgv(1, wargv);
	PyRun_SimpleString("import sys; sys.path.pop(0)\n");
#else
	PySys_SetArgvEx(1, wargv, 0);
#endif

	g_free(wargv[0]);

	/* initialize pygobject */
	pygobject_init(3, 0, 0);
	if(PyErr_Occurred()) {
		PyErr_Print();
		return -1;
	}

	return 0;
}
