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
#include <definitions.h>

/*********************************************************************************
 * Manage the program flow.                                                      *
 * @param int argc: Number of arguments of the program.                          *
 * @param char **argv: Arguments passed to the program.                          *
 * @returns 0 with no errors and -1 otherwise.                                   *
 ********************************************************************************/
int
main(int argc, char **argv) {
	int i;
	double netAptitude = 0.0; /* Store the average flair of the networks. */

	/* Set gettext configuration. */
	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	/* Checking the number of arguments. */
	if(argc < 4 || (argc > 4 && (argc != 6 || strcmp(argv[4], "-o") != 0)))
		error(COMMAND);

	/* We load the config and training files. */
	loadFile(argv[1], argv[2]);
	if(nodSel > numNodules)
		nodSel = numNodules;

	/* Scaling of input and output data from the training file. */
	fprintf(stderr, _("Scaling input data.\n"));
	scaleInputData(numTrain);
	fprintf(stderr, _("Scaling output data.\n"));
	if (netTransf == (func)&Logistic)
		scaleOutputData(outputData,
						numTrain,
						0.0 + pTransfer.epsilon,
						pTransfer.logistic_a - pTransfer.epsilon);
	else
		scaleOutputData(outputData,
						numTrain,
						pTransfer.epsilon - pTransfer.htan_a,
						pTransfer.htan_a - pTransfer.epsilon);

	/* We evolve the networks and nodes ppulations. */
	for (i = 0; measureChange(&netAptitude, i) == false; i++) {
		/* We create a new population of nodes. */
		fprintf(stderr, _("Species %d\n"), i + 1);
		createNodulePopulation();

		/* We add the new population of nodes to the previous population of
		   networks. */
		fprintf(stderr,_("Add nodes to networks.\n"));
		addNodulesNetworks();

		/* We evolve the populations of networks and nodes. */
		fprintf(stderr, _("Evolve populations.\n"));
		evolvePopulations();

		/* We adjust the size of the last node's subpopulation matrix. */
		fprintf(stderr, _("Ajust matrix\n"));
		adjustMatrix();
	}

	/* We order the networks by its flair. */
	fprintf(stderr, _("Sorting networks.\n"));
	sortNetworks();

	/* We read the data of the generalization file. */
	fprintf(stderr, _("Read the generalization data.\n"));
	readGeneralization(argv[3]);

	/* We scale the input and output data from the generalization file. */
	fprintf(stderr, _("Scaling input data.\n"));
	scaleInputData(numGeneral);
	fprintf(stderr, _("Scaling output data.\n"));
	if (netTransf == (func)&Logistic)
		scaleOutputData(outputData,
						numGeneral,
						0.0 + pTransfer.epsilon,
						pTransfer.logistic_a - pTransfer.epsilon);
	else
		scaleOutputData(outputData,
						numGeneral,
						-pTransfer.htan_a + pTransfer.epsilon,
						pTransfer.htan_a - pTransfer.epsilon);

	/* We export the best network found. */
	fprintf(stderr, _("Exporting the best network.\n"));
	if (argc == 6)
		exportBestNetwork(argv[5]);
	else
		exportBestNetwork("output.txt");
	fprintf(stderr, _("End\n"));

	return 0;
}
