/*********************************************************************************
 * Copyright (c) 2004-2016 coconet project (see AUTHORS)                         *
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

#include <string.h>
#include <errno.h>
#include <definitions.h>

/*********************************************************************************
 * Shows an error message depending on an error number.                          *
 * @param int id: Numbe of error produced.                                       *
 * @return void.                                                                 *
 ********************************************************************************/
void
error(int id) {
	system("clear");

	/* Switch the number of error we show one message or other. */
	switch(id) {
	case RES_MEM:
		fprintf(stderr, _("Error reserving memory. %s\n"), strerror(errno));
		break;
	case IO:
		fprintf(stderr, _("Input/output error. %s\n"), strerror(errno));
		break;
	case PARAMETERS:
		fprintf(stderr, _("Error passing the parameters to a function. Either a "
						  "parameter format is not rigth or is out of range.\n"));
		break;
	case COMMAND:
		fprintf(stderr, _("Error in the calling command. the format is:\n"
						  "\tcoconet conf_file training_file generalization_file [-o output_file]\n"
						  " where conf_file is the configuration file of the problem, training_file is "
						  "the file that containsthe training data for the networks and "
						  "generalization_file is the file that contains the generalization data to "
						  "measure the networks. Output_file is optional and contains the name of the "
"output file.\n"));
		break;
	default:
		fprintf(stderr, _("Unknown error. %s\n"), strerror(errno));
		break;
	}

	exit(-1);
}
