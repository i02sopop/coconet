/******************************************************************************
 Copyright (c) 2004-2013 coconet project (see AUTHORS)

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
 File: freeMemory.c
 Function: freeNodule()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Free the memory allocated by a nodule.
 Input Parameters:
 	nodule: Nodulo. Nodule to free.
 Local Variables:
 	i: Integer. Counter.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void freeNodule(nodule *nodule)
{
	int i;

	/*
	  We free the memory allocated by the matrix and vectors of the given
	  nodule.
	*/
	for(i = 0; i < maxNodes; i++) {
		free(nodule->outConn[i]);
		free(nodule->outWeights[i]);
	} for(i = 0; i < cNetPopulation.numInputNodes; i++) {
		free(nodule->inConn[i]);
		free(nodule->inWeights[i]);
	} for(i = 0; i < cNetPopulation.numOutputNodes; i++)
		  nodule->transf = NULL;
	for(i = 0; i < numTrain; i++)
		free(nodule->partialOutputs[i]);

	free(nodule->inConn);
	free(nodule->outConn);
	free(nodule->inWeights);
	free(nodule->outWeights);
	free(nodule->transf);
	free(nodule->partialOutputs);
}

/******************************************************************************
 File: freeMemory.c
 Function: adjustMatrix()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Adjust the matriz size of the last nodule subpopulation.
 Input Parameters: None
 Local Variables:
 	i: Integer. Counter.
 	j: Integer. Counter.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void adjustMatrix()
{
	int i, j;

	/* We adjust the matriz size of the last nodule subpopulation. */
	for(i = numNodules * (cNodulePopulation.numSubpops - 1); i < cNodulePopulation.numNodules; i++) {
		for(j = 0; j < cNetPopulation.numInputNodes; j++) {
			cNodulePopulation.nodules[i]->inConn[j] = (int *)realloc(cNodulePopulation.nodules[i]->inConn[j], cNodulePopulation.nodules[i]->nodes * sizeof(int));
			cNodulePopulation.nodules[i]->inWeights[j] = (double *)realloc(cNodulePopulation.nodules[i]->inWeights[j], cNodulePopulation.nodules[i]->nodes * sizeof(double));
		}

		cNodulePopulation.nodules[i]->outConn = (int **)realloc(cNodulePopulation.nodules[i]->outConn, cNodulePopulation.nodules[i]->nodes * sizeof(int));
		cNodulePopulation.nodules[i]->outWeights = (double **)realloc(cNodulePopulation.nodules[i]->outWeights, cNodulePopulation.nodules[i]->nodes * sizeof(double));
		cNodulePopulation.nodules[i]->transf = (func *)realloc(cNodulePopulation.nodules[i]->transf, cNodulePopulation.nodules[i]->nodes * sizeof(func));
	}
}
