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

#include <definitions.h>

/******************************************************************************
 File: print.c
 Function: printNodule()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Print the information related with a given nodule to an output
              file.
 Input Parameters:
 	imp: Nodule. Nodule to print.
 	iter: Integer. Number of input patterns.
 Local Variables:
 	i: Integer. Counter.
 	j: Integer. Counter.
 	fOutput: Pointer to file. Output file.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void printNodule(nodule *imp, int iter)
{
	int i, j;
	FILE *fOutput;

	/*
	  We open the output file and print the nodule information.
	  TODO: Get the output filename from configuration.
	*/
	fOutput = fopen("nodules.txt", "a");
	fprintf(fOutput, "id: %d\nNumber of nodes: %d\naptitude: %lf\n\nInput connections matrix:", imp->id, imp->nodes, imp->aptitude);
	for(i = 0; i < cNetPopulation.numInputNodes; i++) {
		fprintf(fOutput, "\n");
		for(j = 0; j < imp->nodes; j++)
			fprintf(fOutput, "%d ", imp->inConn[i][j]);
	}

	fprintf(fOutput, "\nOutput connections matrix:");
	for(i = 0; i < imp->nodes; i++) {
		fprintf(fOutput, "\n");
		for(j = 0; j < cNetPopulation.numOutputNodes; j++)
			fprintf(fOutput, "%d ", imp->outConn[i][j]);
	}

	fprintf(fOutput, "\nInput weights matrix:");
	for(i = 0; i < cNetPopulation.numInputNodes; i++) {
		fprintf(fOutput, "\n");
		for(j = 0; j < imp->nodes; j++)
			fprintf(fOutput, "%lf ", imp->inWeights[i][j]);
	}

	fprintf(fOutput, "\nOutput weights matrix:");
	for(i = 0; i < imp->nodes; i++) {
		fprintf(fOutput, "\n");
		for(j = 0; j < cNetPopulation.numOutputNodes; j++)
			fprintf(fOutput, "%lf ", imp->outWeights[i][j]);
	}

	fprintf(fOutput, "\nPartial outputs\n");
	for(i = 0; i < iter; i++)
		for(j = 0; j < cNetPopulation.numOutputNodes; j++)
			fprintf(fOutput, "%lf ", imp->partialOutputs[i][j]);
	fprintf(fOutput, "\n\n");

	/* We close the putput file. */
	fclose(fOutput);
}

/******************************************************************************
 File: print.c
 Function: printNetwork()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Print the network data into an output file.
 Input Parameters:
   net: Red. Network to print.
 Local Variables:
   i: Integer. Counter.
   fOutput: Pointer to file. Output file.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void printNetwork(network *net)
{
	int i;
	FILE *fOutput;

	/*
	  We open the output file.
	  TODO: Get the output file from somewhere in the config.
	*/
	fOutput = fopen("net.txt", "a");

	/* We print the net data. */
	fprintf(fOutput, "nodules: ");
	for(i = 0; i < cNodulePopulation.numSubpops; i++)
		fprintf(fOutput, "%d ", net->nodules[i]->id);

	fprintf(fOutput, "\naptitude: %lf\n", net->aptitude);
	fprintf(fOutput, "Output values:\n");
	for(i = 0; i < cNetPopulation.numOutputNodes; i++)
		fprintf(fOutput, "%lf ", net->outValues[i]);
	fprintf(fOutput, "\n");

	if(netTransf == (func)&Logistic)
		fprintf(fOutput, "Logistic\n");
	else
		fprintf(fOutput, "HyperbolicTangent\n");

	/* We close the output file. */
	fclose(fOutput);
}
