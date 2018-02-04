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

#include <stdlib.h>

#include "suites.h"

int
main() {
	int number_failed;
	Suite *suite;
	SRunner *runner;

	suite = utils_suite();
	runner = srunner_create(suite);

	srunner_run_all(runner, CK_NORMAL);
	number_failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

