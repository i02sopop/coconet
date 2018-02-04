/*********************************************************************************
 * Copyright (c) 2018 coconet project (see AUTHORS)                              *
 *                                                                               *
 * This file is part of Coconet.                                                 *
 *                                                                               *
 * Coconet is free software: you can redistribute it and/or modify it under the  *
 * terms of the GNU General Public License as published by the Free Software     *
 * Foundation, either version 3 of the License, or (at your option) any later    *
 * version.                                                                      *
 *                                                                               *
 * Coconet is distributed in the hope that it will be useful, but WITHOUT ANY    *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR *
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.   *
 *                                                                               *
 * You should have received a copy of the GNU General Public License along with  *
 * coconet. If not, see <http://www.gnu.org/licenses/>.                          *
 ********************************************************************************/

#include <definitions.h>

#include "suites.h"

START_TEST (test_xmalloc)
{
	char *utils;

	utils = (char *)xmalloc(10 * sizeof(char));
	ck_assert(utils != NULL);

	free(utils);
}
END_TEST

START_TEST (test_xcalloc)
{
	char *utils;

	utils = (char *)xcalloc(10, sizeof(char));
	ck_assert(utils != NULL);

	free(utils);
}
END_TEST

START_TEST (test_xrealloc)
{
	char *utils = NULL;

	utils = (char *)xrealloc(utils, sizeof(char));
	ck_assert(utils != NULL);

	utils = (char *)xrealloc(utils, 10 * sizeof(char));
	ck_assert(utils != NULL);

	free(utils);
}
END_TEST

START_TEST (test_xlog)
{
	xlog(0, "number: %d, character: %c, string: %s", 1, 'a', "string\0");
}
END_TEST

Suite *
utils_suite() {
	Suite *s;
	TCase *tc_core;

	s = suite_create("utils");

	/* core test case */
	tc_core = tcase_create("core");

	tcase_add_test(tc_core, test_xmalloc);
	tcase_add_test(tc_core, test_xcalloc);
	tcase_add_test(tc_core, test_xrealloc);
	tcase_add_test(tc_core, test_xlog);
	suite_add_tcase(s, tc_core);

	return s;
}
