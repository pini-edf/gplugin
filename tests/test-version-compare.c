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

#include <gplugin.h>

/******************************************************************************
 * Tests
 *****************************************************************************/
/* bad versions */
static void
test_gplugin_version_null__null(void) {
	if(g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDERR))
		gplugin_version_compare(NULL, NULL, NULL);

	g_test_trap_assert_failed();
	g_test_trap_assert_stderr("*gplugin_version_compare*assertion*");
}

static void
test_gplugin_version_null__1_2_3(void) {
	if(g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDERR))
		gplugin_version_compare(NULL, "1.2.3", NULL);

	g_test_trap_assert_failed();
	g_test_trap_assert_stderr("*gplugin_version_compare*assertion*");
}

static void
test_gplugin_version_1_2_3__null(void) {
	if(g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDERR)) {
		gplugin_version_compare("1.2.3", NULL, NULL);
	}

	g_test_trap_assert_failed();
	g_test_trap_assert_stderr("*gplugin_version_compare*assertion*");
}

static void
test_gplugin_version_abc__1_2_3(void) {
	GError *error = NULL;
	gint t = 0;

	if(g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDERR)) {
		t = gplugin_version_compare("abc", "1.2.3", &error);

		g_assert(t == 1);
		g_assert_no_error(error);
	}

	g_test_trap_assert_failed();
	g_test_trap_assert_stderr("*assertion*");
}

static void
test_gplugin_version_1_2_3__abc(void) {
	GError *error = NULL;
	gint t = 0;

	if(g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDERR)) {
		t = gplugin_version_compare("1.2.3", "abc", &error);

		g_assert(t == -1);
		g_assert_no_error(error);
	}

	g_test_trap_assert_failed();
	g_test_trap_assert_stderr("*assertion*");
}

/* major version tests */
static void
test_gplugin_version_1_0_0__0_0_0(void) {
	g_assert(gplugin_version_compare("1.0.0", "0.0.0", NULL) == -1);
}

static void
test_gplugin_version_1_0_0__1_0_0(void) {
	g_assert(gplugin_version_compare("1.0.0", "1.0.0", NULL) == 0);
}

static void
test_gplugin_version_0_0_0__1_0_0(void) {
	g_assert(gplugin_version_compare("0.0.0", "1.0.0", NULL) == 1);
}

/* minor version tests */
static void
test_gplugin_version_0_1_0__0_0_0(void) {
	g_assert(gplugin_version_compare("0.1.0", "0.0.0", NULL) == -1);
}

static void
test_gplugin_version_0_1_0__0_1_0(void) {
	g_assert(gplugin_version_compare("0.1.0", "0.1.0", NULL) == 0);
}

static void
test_gplugin_version_0_0_0__0_1_0(void) {
	g_assert(gplugin_version_compare("0.0.0", "0.1.0", NULL) == 1);
}

/* micro version tests */
static void
test_gplugin_version_0_0_1__0_0_0(void) {
	g_assert(gplugin_version_compare("0.1.0", "0.0.0", NULL) == -1);
}

static void
test_gplugin_version_0_0_1__0_0_1(void) {
	g_assert(gplugin_version_compare("0.1.0", "0.1.0", NULL) == 0);
}

static void
test_gplugin_version_0_0_0__0_0_1(void) {
	g_assert(gplugin_version_compare("0.0.0", "0.1.0", NULL) == 1);
}

/******************************************************************************
 * Main
 *****************************************************************************/
gint
main(gint argc, gchar **argv) {
	g_test_init(&argc, &argv, NULL);

	gplugin_init();

	/* bad versions */
	g_test_add_func("/version-check/null__null",
	                test_gplugin_version_null__null);
	g_test_add_func("/version-check/null__1_2_3",
	                test_gplugin_version_null__1_2_3);
	g_test_add_func("/version-check/1_2_3__null",
	                test_gplugin_version_1_2_3__null);
	g_test_add_func("/version-check/abc__1_2_3",
	                test_gplugin_version_abc__1_2_3);
	g_test_add_func("/version-check/1_2_3__abc",
	                test_gplugin_version_1_2_3__abc);

	/* major version */
	g_test_add_func("/version-check/1_0_0__0_0_0",
	                test_gplugin_version_1_0_0__0_0_0);
	g_test_add_func("/version-check/1_0_0__1_0_0",
	                test_gplugin_version_1_0_0__1_0_0);
	g_test_add_func("/version-check/0_0_0__1_0_0",
	                test_gplugin_version_0_0_0__1_0_0);

	/* minor version */
	g_test_add_func("/version-check/0_1_0__0_0_0",
	                test_gplugin_version_0_1_0__0_0_0);
	g_test_add_func("/version-check/0_1_0__0_1_0",
	                test_gplugin_version_0_1_0__0_1_0);
	g_test_add_func("/version-check/0_0_0__0_1_0",
	                test_gplugin_version_0_0_0__0_1_0);

	/* micro version */
	g_test_add_func("/version-check/0_0_1__0_0_0",
	                test_gplugin_version_0_0_1__0_0_0);
	g_test_add_func("/version-check/0_0_1__0_0_1",
	                test_gplugin_version_0_0_1__0_0_1);
	g_test_add_func("/version-check/0_0_0__0_0_1",
	                test_gplugin_version_0_0_0__0_0_1);

	return g_test_run();
}

