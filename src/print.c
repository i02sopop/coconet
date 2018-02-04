/*********************************************************************************
 * Copyright (c) 2004-2018 coconet project (see AUTHORS)                         *
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

/******************************************************************************
 * Print the information related with a given nodule to an output file.       *
 * @param nodule *imp: Nodule to print.                                       *
 * @param int iter: Number of input patterns.                                 *
 * @return void                                                               *
 *****************************************************************************/
void
printNodule(nodule *imp, int iter) {
	int i, j;
	FILE *fOutput;

	/* Open the output file and print the nodule information.
	 * TODO: Get the output filename from configuration. */
	fOutput = fopen("nodules.txt", "a");
	fprintf(fOutput, _("id: %d\nNumber of nodes: %d\nAptitude: %lf\n\nInput connections matrix:"),
			imp->id, imp->nodes, imp->aptitude);
	for (i = 0; i < cNetPopulation.numInputNodes; i++) {
		fprintf(fOutput, "\n");
		for (j = 0; j < imp->nodes; j++)
			fprintf(fOutput, "%d ", imp->inConn[i][j]);
	}

	fprintf(fOutput, _("\nOutput connections matrix:"));
	for (i = 0; i < imp->nodes; i++) {
		fprintf(fOutput, "\n");
		for (j = 0; j < cNetPopulation.numOutputNodes; j++)
			fprintf(fOutput, "%d ", imp->outConn[i][j]);
	}

	fprintf(fOutput, _("\nInput weights matrix:"));
	for (i = 0; i < cNetPopulation.numInputNodes; i++) {
		fprintf(fOutput, "\n");
		for (j = 0; j < imp->nodes; j++)
			fprintf(fOutput, "%lf ", imp->inWeights[i][j]);
	}

	fprintf(fOutput, _("\nOutput weights matrix:"));
	for(i = 0; i < imp->nodes; i++) {
		fprintf(fOutput, "\n");
		for(j = 0; j < cNetPopulation.numOutputNodes; j++)
			fprintf(fOutput, "%lf ", imp->outWeights[i][j]);
	}

	fprintf(fOutput, _("\nPartial outputs\n"));
	for (i = 0; i < iter; i++)
		for (j = 0; j < cNetPopulation.numOutputNodes; j++)
			fprintf(fOutput, "%lf ", imp->partialOutputs[i][j]);
	fprintf(fOutput, "\n\n");

	/* Close the output file. */
	fclose(fOutput);
}

/******************************************************************************
 * Print the network data into an output file.                                *
 * @param network *net: Network to print.                                     *
 * @return void                                                               *
 *****************************************************************************/
void
printNetwork(network *net) {
	int i;
	FILE *fOutput;

	/* Open the output file.
	 * TODO: Get the output file from somewhere in the config. */
	fOutput = fopen("net.txt", "a");

	/* Print the net data. */
	fprintf(fOutput, _("Nodules: "));
	for (i = 0; i < cNodulePopulation.numSubpops; i++)
		fprintf(fOutput, "%d ", net->nodules[i]->id);

	fprintf(fOutput, "\n");
	fprintf(fOutput, _("Aptitude: %lf\n"), net->aptitude);
	fprintf(fOutput, _("\nOutput values:\n"));
	for (i = 0; i < cNetPopulation.numOutputNodes; i++)
		fprintf(fOutput, "%lf ", net->outValues[i]);

	if (netTransf == (func)&Logistic)
		fprintf(fOutput, _("\nLogistic\n"));
	else
		fprintf(fOutput, _("\nHyperbolicTangent\n"));

	/* Close the output file. */
	fclose(fOutput);
}
