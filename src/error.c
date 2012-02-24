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
#include <definiciones.h>

/******************************************************************************
 File: error.c
 Function: error()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Shows an error message depending on an error number.
 Input parameters:
	errorId: Integer. Numbe of error produced.
 Local Variables: None
 Return Value: None
 Calling Functions: None
******************************************************************************/

void error(int errorId)
{
	system("clear");

	/* Switch the number of error we show one message or other. */
	switch(errorId) {
	case RES_MEM:
		fprintf(stderr, "Error reserving memory. %s\n", strerror(errno));
		break;
	case IO:
		fprintf(stderr, "Input/output error. %s\n", strerror(errno));
		break;
	case PARAMETROS:
		fprintf(stderr, "Error inserting the parameters into a function. "
				"Either the parameters format is not rigth or is out of range."
				"\n");
		break;
	case COMANDO:
		fprintf(stderr,"Error in the calling command. the format is:\n\t"
				"coconet conf_file training_file generalization_file "
				"[-o output_file]\n where conf_file is the configuration "
				"file of the problem, training_file is the file that contains"
				"the training data for the networks and generalization_file is"
				" the file that contains the generalization data to measure "
				"the networks. Output_file is the name of the output file, and"
				" is optional\n");
		break;
	default:
		fprintf(stderr,"Unknown error. %s\n", strerror(errno));
		break;
	}

	exit(1);
}
