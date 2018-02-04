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

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <definitions.h>

/**/
void *
xmalloc(size_t size) {
	void *output = malloc(size);
	if (size != 0 && output == NULL) {
		xerror(RES_MEM);
	}

	return output;
}

void *
xcalloc(size_t nmemb, size_t size) {
	void *output = calloc(nmemb, size);
	if (nmemb != 0 && size != 0 && output == NULL) {
		xerror(RES_MEM);
	}

	return output;
}

void *
xrealloc(void *ptr, size_t size) {
	void *output = realloc(ptr, size);
	if (size != 0 && output == NULL) {
		xerror(RES_MEM);
		return ptr;
	}

	return output;
}

void
xlog(int level, char *fmt, ...) {
	va_list argv;

	va_start(argv, fmt);
	switch (level) {
	case 0:
		vfprintf(stdout, _(fmt), argv);
		break;
	case 1:
		vfprintf(stdout, _(fmt), argv);
		break;
	case 2:
		vfprintf(stderr, _(fmt), argv);
		break;
	}
	va_end(argv);
}

/* Shows an error message depending on an error number. */
void
xerror(int id) {
	system("clear");

	/* Switch the number of error we show one message or other. */
	switch(id) {
	case RES_MEM:
		xlog(2, "Error reserving memory: %s\n", strerror(errno));
		break;
	case IO:
		xlog(2, "Input/output error: %s\n", strerror(errno));
		break;
	case PARAMETERS:
		xlog(2, "Error passing the parameters to a function. Either a parameter"
			 " format is not rigth or is out of range.\n");
		break;
	case COMMAND:
		xlog(2, "Error in the calling command. the format is:\n"
			 "\tcoconet conf_file training_file generalization_file [-o output_file]\n"
			 " where conf_file is the configuration file of the problem, training_file is "
			 "the file that containsthe training data for the networks and "
			 "generalization_file is the file that contains the generalization data to "
			 "measure the networks. Output_file is optional and contains the name of the "
			 "output file.\n");
		break;
	default:
		xlog(2, "Unknown error: %s\n", strerror(errno));
		break;
	}

	exit(-1);
}
