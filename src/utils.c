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

#include <definitions.h>

void *
xmalloc(size_t size) {
	void *output = malloc(size);
	if (size != 0 && output == NULL) {
		error(RES_MEM);
	}

	return output;
}

void *
xcalloc(size_t nmemb, size_t size) {
	void *output = calloc(nmemb, size);
	if (nmemb != 0 && size != 0 && output == NULL) {
		error(RES_MEM);
	}

	return output;
}

void *
xrealloc(void *ptr, size_t size) {
	void *output = realloc(ptr, size);
	if (size != 0 && output == NULL) {
		error(RES_MEM);
		return ptr;
	}

	return output;
}
