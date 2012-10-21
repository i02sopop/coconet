/******************************************************************************
 Copyright (c) 2004-2012 coconet project (see AUTHORS)

 This file is part of Coconet.

 Coconet is free software: you can redistribute it and/or modify it under the
 terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.

 Coconet is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 coconet. If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <string.h>
#include <errno.h>
#include <definitions.h>

/******************************************************************************
 File: error.c
 Function: error()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Shows an error message depending on an error number.
 Input parameters:
	id: Integer. Numbe of error produced.
 Local Variables: None
 Return Value: None
 Calling Functions: None
******************************************************************************/

void error(int id)
{
	system("clear");

	/* Switch the number of error we show one message or other. */
	switch(id) {
	case RES_MEM:
		fprintf(stderr, _("RES_MEM_ERROR"), strerror(errno));
		break;
	case IO:
		fprintf(stderr, _("IO_ERROR"), strerror(errno));
		break;
	case PARAMETERS:
		fprintf(stderr, _("PARAM_ERROR"));
		break;
	case COMMAND:
		fprintf(stderr, _("COMMAND_ERROR"));
		break;
	default:
		fprintf(stderr, _("UNKNOWN_ERROR"), strerror(errno));
		break;
	}

	exit(-1);
}
