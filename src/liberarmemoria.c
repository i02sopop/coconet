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
 File: liberarMemoria.c
 Function: liberarNodulo()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Free the memory allocated by a nodule.
 Input Parameters:
 	nodule: Nodulo. Nodule to free.
 Local Variables:
 	i: Integer. Counter.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void liberarNodulo(nodule *nodule)
{
	int i;

	/*
	  We free the memory allocated by the matrix and vectors of the given
	  nodule.
	*/
	for(i = 0; i < maxNodes; i++) {
		free(nodule->outConn[i]);
		free(nodule->outWeights[i]);
	} for(i = 0; i < netPopulation.n_nodos_entrada; i++) {
		free(nodule->inConn[i]);
		free(nodule->inWeights[i]);
	} for(i = 0; i < netPopulation.n_nodos_salida; i++)
		  nodule->transf = NULL;
	for(i = 0; i < n_entrenamiento; i++)
		free(nodule->partialOutputs[i]);

	free(nodule->inConn);
	free(nodule->outConn);
	free(nodule->inWeights);
	free(nodule->outWeights);
	free(nodule->transf);
	free(nodule->partialOutputs);
}

/******************************************************************************
 File: liberarMemoria.c
 Function: ajustarMatrices()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Adjust the matriz size of the last nodule subpopulation.
 Input Parameters: None
 Local Variables:
 	i: Integer. Counter.
 	j: Integer. Counter.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void ajustarMatrices()
{
	int i, j;

	/* We adjust the matriz size of the last nodule subpopulation. */
	for(i = numNodules * (nodulePopulation.n_subpobl - 1); i < nodulePopulation.n_nodulos; i++) {
		for(j = 0; j < netPopulation.n_nodos_entrada; j++) {
			nodulePopulation.nodulos[i]->inConn[j] = (int *)realloc(nodulePopulation.nodulos[i]->inConn[j], nodulePopulation.nodulos[i]->nodes * sizeof(int));
			nodulePopulation.nodulos[i]->inWeights[j] = (double *)realloc(nodulePopulation.nodulos[i]->inWeights[j], nodulePopulation.nodulos[i]->nodes * sizeof(double));
		}

		nodulePopulation.nodulos[i]->outConn = (int **)realloc(nodulePopulation.nodulos[i]->outConn, nodulePopulation.nodulos[i]->nodes * sizeof(int));
		nodulePopulation.nodulos[i]->outWeights = (double **)realloc(nodulePopulation.nodulos[i]->outWeights, nodulePopulation.nodulos[i]->nodes * sizeof(double));
		nodulePopulation.nodulos[i]->transf = (func *)realloc(nodulePopulation.nodulos[i]->transf, nodulePopulation.nodulos[i]->nodes * sizeof(func));
	}
}
