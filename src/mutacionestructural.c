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
 File: mutacionEstructural.c
 Function: copiarDescendencia()
 Author: Pablo �lvarez de Sotomayor Posadillo
 Description: Copy the actual nodule population into a new descendant
              population.
 Input Parameters: None
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   k: Integer. Counter.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
******************************************************************************/

void copiarDescendencia()
{
	int i, j, k, noduleOrig, initialNodule;

	/* Initialization of variables. */
	initialNodule = numNodules * (nodulePopulation.n_subpobl - 1);
	descendencia = (nodule **)malloc(numNodules * sizeof(nodule));
	if(descendencia == NULL)
		error(RES_MEM);

	/* We copy the nodules to the new population. */
	for(i = 0; i < numNodules; i++) {
		descendencia[i] = (nodule *)malloc(sizeof(nodule));
		if(descendencia[i] == NULL)
			error(RES_MEM);

		/* We copy the nodule from the old to the new population. */
		noduleOrig = initialNodule + i;
		descendencia[i]->id = nodulePopulation.nodulos[noduleOrig]->id;
		descendencia[i]->nodes = nodulePopulation.nodulos[noduleOrig]->nodes;
		descendencia[i]->aptitude = nodulePopulation.nodulos[noduleOrig]->aptitude;

		descendencia[i]->inConn = (int **)malloc(netPopulation.n_nodos_entrada * sizeof(int));
		descendencia[i]->inWeights = (double **)malloc(netPopulation.n_nodos_entrada * sizeof(double));
		descendencia[i]->outConn = (int **)malloc(maxNodes * sizeof(int));
		descendencia[i]->outWeights = (double **)malloc(maxNodes * sizeof(double));
		descendencia[i]->transf = (func *)malloc(maxNodes * sizeof(func));
		descendencia[i]->partialOutputs = (double **)malloc(n_entrenamiento * sizeof(double));
		if(descendencia[i]->inConn == NULL ||
		   descendencia[i]->inWeights == NULL ||
		   descendencia[i]->outConn == NULL ||
		   descendencia[i]->outWeights == NULL ||
		   descendencia[i]->transf == NULL ||
		   descendencia[i]->partialOutputs == NULL)
			error(RES_MEM);

		for(j = 0; j < netPopulation.n_nodos_entrada; j++) {
			descendencia[i]->inConn[j] = (int *)malloc(maxNodes * sizeof(int));
			descendencia[i]->inWeights[j] = (double *)malloc(maxNodes * sizeof(double));
			if(descendencia[i]->inConn[j] == NULL ||
				descendencia[i]->inWeights[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < maxNodes; k++) {
				descendencia[i]->inConn[j][k] = nodulePopulation.nodulos[noduleOrig]->inConn[j][k];
				descendencia[i]->inWeights[j][k] = nodulePopulation.nodulos[noduleOrig]->inWeights[j][k];
			} /* end for */
		} /* end for */

		for(j = 0; j < maxNodes; j++) {
			descendencia[i]->outConn[j] = (int *)malloc(netPopulation.n_nodos_salida * sizeof(int));
			descendencia[i]->outWeights[j] = (double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
			if(descendencia[i]->outConn[j] == NULL ||
				descendencia[i]->outWeights[j] == NULL)
				error(RES_MEM);

			descendencia[i]->transf[j] = nodulePopulation.nodulos[noduleOrig]->transf[j];

			for(k = 0; k < netPopulation.n_nodos_salida; k++) {
				descendencia[i]->outConn[j][k] = nodulePopulation.nodulos[noduleOrig]->outConn[j][k];
				descendencia[i]->outWeights[j][k] = nodulePopulation.nodulos[noduleOrig]->outWeights[j][k];
			} /* end for */
		} /* end for */

		for(j = 0; j < n_entrenamiento; j++) {
			descendencia[i]->partialOutputs[j] = (double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
			if(descendencia[i]->partialOutputs[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < netPopulation.n_nodos_salida; k++)
				descendencia[i]->partialOutputs[j][k] = nodulePopulation.nodulos[noduleOrig]->partialOutputs[j][k];
		} /* end for */
	} /* end for */
}

/******************************************************************************
 File: mutacionEstructural.c
 Function: mutarNodulos()
 Author: Pablo �lvarez de Sotomayor Posadillo
 Description: Make an estructural mutation in a given nodule.
 Input Parameters:
   nodule: Integer. Number of nodule to mutate.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   num: Integer. Number of mutation elements.
 Return Value: None
 Calling Functions: 
   anadirNodo(): Add a node to the nodule.
   borrarNodo(): Delete a node from the nodule.
   anadirConexion(): Add a connection to the nodule.
   borrarConexion(): Delete a connection from the nodule.
   medirCambioNodulo(): Calculate the nodules aptitude variation after the
                        mutation.
   doubleRandom(): Returns a random float of double precision.
******************************************************************************/

void mutarNodulos(int nodule)
{
	int i, j, num;

	/* We made the estructural mutation. */
	/* Deletion of nodes. */
	num = (int)(delta_min + doubleRandom() * (1 - nodulePopulation.nodulos[nodule]->aptitude)
				* (delta_min - delta_max));
	if(num < 0)
		num = 0;
	else if(nodulePopulation.nodulos[nodule]->nodes < num)
		num = nodulePopulation.nodulos[nodule]->nodes;
	else if(num > 0)
		borrarNodo(nodule, num);

	/* Add nodes. */
	num = (int)(delta_min + doubleRandom() * (1 - nodulePopulation.nodulos[nodule]->aptitude)
				* (delta_min - delta_max));
	if(num < 0)
		num = 0;
	else if((nodulePopulation.nodulos[nodule]->nodes + num) > maxNodes)
		num = maxNodes - nodulePopulation.nodulos[nodule]->nodes;
	else if(num > 0)
		anadirNodo(nodule, num); /* We add a new node,*/

	/* Delete connections. */
	num = (int)(delta_min + doubleRandom() * (1 - nodulePopulation.nodulos[nodule]->aptitude)
				* (delta_min - delta_max));
	if(num < 0)
		num = 0;

	for(i = 0; i < num && nodulePopulation.nodulos[nodule]->nodes > 0; i++) {
		j = random() % (netPopulation.n_nodos_entrada + netPopulation.n_nodos_salida);
		borrarConexion(nodule, j);
	}

	/* Add connections. */
	num = (int)(delta_min + doubleRandom() * (1 - nodulePopulation.nodulos[nodule]->aptitude)
				* (delta_min - delta_max));
	if(num < 0)
		num = 0;

	for(i = 0; i < num && nodulePopulation.nodulos[nodule]->nodes > 0; i++)
		anadirConexion(nodule);

	/*
	  We make the calculations over the modified nodule, both his partial
	  outputs and his aptitude.
	*/
	medirCambioNodulo(nodule);
}

/******************************************************************************
 File: mutacionEstructural.c
 Function: anadirConexion()
 Author: Pablo �lvarez de Sotomayor Posadillo
 Description: Add a connection to a given nodule.
 Input Parameters:
   nodule: Integer. Number of nodule to modify.
 Local Variables:
   i. Integer. Counter.
   j: Integer. Counter.
   sel: Integer. Type of connection to add (input, internal or outpunt).
   origin: Vector if integers. Possible origin nodes for the connection.
   destination: Vector of integers. Possible destination nodes for the connection.
   num: Integer. Counter.
   pos: Integer. Position to add the connection.
 Return Value: None
 Calling Functions
   error(): Function to show an error message depending on an error number.
   doubleRandom(): Returns a random float of double precision.
******************************************************************************/

void anadirConexion(int nodule)
{
	int i, j, sel, *origin, *destination, num, pos;

	num = 0;
	sel = random() % 2;
	origin = (int *)malloc(sizeof(int));
	destination = (int *)malloc(sizeof(int));
	if(origin == NULL || destination == NULL)
		error(RES_MEM);

	if(sel == 0) {
		/* We add an input connection. */
		for(i = 0; i < netPopulation.n_nodos_entrada; i++)
			for(j = 0; j < nodulePopulation.nodulos[nodule]->nodes; j++)
				if(nodulePopulation.nodulos[nodule]->inConn[i][j] == 0) {
					origin = (int *)realloc(origin, (num + 1) * sizeof(int));
					destination = (int *)realloc(destination, (num + 1) * sizeof(int));
					origin[num] = i;
					destination[num] = j;
					num++;
				} /* end if */

		if(num != 0){
			pos = random() % num;
			nodulePopulation.nodulos[nodule]->inConn[origin[pos]][destination[pos]] = 1;
			nodulePopulation.nodulos[nodule]->inWeights[origin[pos]][destination[pos]] = doubleRandom() / 2;
			free(origin);
			free(destination);
		} /* end if */
	} /* end if */

	if((sel == 1 && num == 0) || num == 0) {
		/* We add an output connection. */
		for(i = 0; i < nodulePopulation.nodulos[nodule]->nodes; i++)
			for(j = 0; j < netPopulation.n_nodos_salida; j++)
				if(nodulePopulation.nodulos[nodule]->outConn[i][j] == 0) {
					origin = (int *)realloc(origin, (num + 1) * sizeof(int));
					destination = (int *)realloc(destination, (num + 1) * sizeof(int));
					origin[num] = i;
					destination[num] = j;
					num++;
				} /* end if */

		if(num != 0) {
			pos = random() % num;
			nodulePopulation.nodulos[nodule]->outConn[origin[pos]][destination[pos]] = 1;
			nodulePopulation.nodulos[nodule]->outWeights[origin[pos]][destination[pos]] = doubleRandom() / 2;
			free(origin);
			free(destination);
		} /* end if */
	} /* end if */

	if(num == 0) {
		free(origin);
		free(destination);
	} /* end if */
}

/********************************************************************************
 File: mutacionEstructural.c
 Function: anadirNodo()
 Author: Pablo �lvarez de Sotomayor Posadillo
 Description: Add some nodes to a nodule.
 Input Parameters:
   nodule: Integer. Number of nodule to work with.
   nodes: Integer. Number of nodes to add.
 Local Variables:
   i: Integer. Counter
 Return Value: None
 Calling Functions: None
*******************************************************************************/

void anadirNodo(int nodule, int nodes)
{
	int i;

	for(i = 0; i < nodes; i++) {
		/* We assign the transfer function to the node. */
		if((random() % 2) == 0)
			nodulePopulation.nodulos[nodule]->transf[nodulePopulation.nodulos[nodule]->nodes + i]
				= (func)&HyperbolicTangent;
		else
			nodulePopulation.nodulos[nodule]->transf[nodulePopulation.nodulos[nodule]->nodes + i]
				= (func)&Logistic;
	}

	/* We update the number of nodes. */
	nodulePopulation.nodulos[nodule]->nodes += nodes;
}

/*******************************************************************************
 File: mutacionEstructural.c
 Function: borrarConexion()
 Author: Pablo �lvarez de Sotomayor Posadillo
 Description: Delete a connection of a given node.
 Input Parameters:
   nodule: Integer. Number of nodule to work with.
   node: Integer. Node to delete the connection from.
 Local Variables:
   i: Integer. Counter
   sel: Integer. Connection to delete.
 Return Value: None
 Calling Functions: None
*******************************************************************************/

void borrarConexion(int nodule, int node)
{
	int i, sel;

	if(node < netPopulation.n_nodos_entrada){
		/* We delete an input connection. */
		for(i = 0; i < nodulePopulation.nodulos[nodule]->nodes &&
				nodulePopulation.nodulos[nodule]->inConn[node][i] != 1; i++);

		if(i < nodulePopulation.nodulos[nodule]->nodes){
			sel = random() % nodulePopulation.nodulos[nodule]->nodes;
			for(; nodulePopulation.nodulos[nodule]->inConn[node][sel] != 1;
				sel = random() % nodulePopulation.nodulos[nodule]->nodes);
			nodulePopulation.nodulos[nodule]->inConn[node][sel] = 0;
			nodulePopulation.nodulos[nodule]->inWeights[node][sel] = 0;
		} /* end if */
	} else {
		/* We delete an output connection. */
		node -= netPopulation.n_nodos_entrada;
		for(i = 0; i < nodulePopulation.nodulos[nodule]->nodes &&
				nodulePopulation.nodulos[nodule]->outConn[i][node] != 1; i++);

		if(i < nodulePopulation.nodulos[nodule]->nodes) {
			sel = random() % nodulePopulation.nodulos[nodule]->nodes;
			for(; nodulePopulation.nodulos[nodule]->outConn[sel][node] != 1;
				sel = random() % nodulePopulation.nodulos[nodule]->nodes);
			nodulePopulation.nodulos[nodule]->outConn[sel][node] = 0;
			nodulePopulation.nodulos[nodule]->outWeights[sel][node] = 0;
		} /* end if */
	} /* end if */
}

/********************************************************************************
 File: mutacionEstructural.c
 Function: borrarNodo()
 Author: Pablo �lvarez de Sotomayor Posadillo
 Description: Delete a given number of nodes from a nodule.
 Input Parameters:
   nodule: Integer. Number of nodule to work with.
   nodes. Integer. Number of nodes to delete.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   k: Integer. Counter.
   node: Integer. Node to delete.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
*******************************************************************************/

void borrarNodo(int nodule, int nodes)
{
	int i, j, k, node;
  
	for(k = 0; k < nodes; k++) {
		node = random() % nodulePopulation.nodulos[nodule]->nodes;

		/* All the nodes after the node to delete have to go back a position. */
		for(i = node + 1; i < nodulePopulation.nodulos[nodule]->nodes; i++) {
			nodulePopulation.nodulos[nodule]->transf[i - 1] =
				nodulePopulation.nodulos[nodule]->transf[i];
			for(j = 0; j < netPopulation.n_nodos_salida; j++) {
				nodulePopulation.nodulos[nodule]->outConn[i - 1][j] =
					nodulePopulation.nodulos[nodule]->outConn[i][j];
				nodulePopulation.nodulos[nodule]->outWeights[i - 1][j] =
					nodulePopulation.nodulos[nodule]->outWeights[i][j];
				nodulePopulation.nodulos[nodule]->outConn[i][j] = 0;
				nodulePopulation.nodulos[nodule]->outWeights[i][j] = 0;
			} /* end for */
		} /* end for */

		for(j = 0; j < netPopulation.n_nodos_salida; j++) {
			nodulePopulation.nodulos[nodule]->outConn[i - 1][j] = 0;
			nodulePopulation.nodulos[nodule]->outWeights[i - 1][j] = 0;
		} /* end for */

		for(i = 0; i < netPopulation.n_nodos_entrada; i++) {
			for(j = node + 1; j < nodulePopulation.nodulos[nodule]->nodes; j++) {
				nodulePopulation.nodulos[nodule]->inConn[i][j - 1] =
					nodulePopulation.nodulos[nodule]->inConn[i][j];
				nodulePopulation.nodulos[nodule]->inWeights[i][j - 1] =
					nodulePopulation.nodulos[nodule]->inWeights[i][j];
				nodulePopulation.nodulos[nodule]->inConn[i][j] = 0;
				nodulePopulation.nodulos[nodule]->inWeights[i][j] = 0;
			} /* end for */

			nodulePopulation.nodulos[nodule]->inConn[i][j-1] = 0;
			nodulePopulation.nodulos[nodule]->inWeights[i][j-1] = 0;
		} /* end for */
  
		nodulePopulation.nodulos[nodule]->nodes--;
	} /* end for */
}

/********************************************************************************
 File: mutacionEstructural.c
 Function: sustituirNodulos()
 Author: Pablo �lvarez de Sotomayor Posadillo
 Description: Substitude the worst nodules of a population with the bset nodules
              of its descendant population.
 Input Parameters: None
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   id: Vector of Integers. Store the nodules id in order to sort them by
       aptitude.
   order: Vector of floats. Store the nodules aptitude in order to sort them.
 Return Value: None
 Calling Functions
   liberarNodulo(): Free the memory ocuppied by a given nodule.
   ordenarNodulos(): Sort the nodules of the population.
   error(): Function to show an error message depending on an error number.
*******************************************************************************/

void sustituirNodulos()
{
	int i, j, *id;
	double *order;

	/* We order the nodules by aptitude. */
	id = (int *)malloc(numNodules * sizeof(int));
	order = (double *)malloc(numNodules * sizeof(double));
	if(id == NULL || order == NULL)
		error(RES_MEM);

	for(i = 0; i < numNodules; i++) {
		id[i] = ((nodulePopulation.n_subpobl - 1) * numNodules) + i;
		order[i] =
			nodulePopulation.nodulos[(nodulePopulation.n_subpobl - 1) * numNodules + i]->aptitude;
	}

	for(i = 0; i < numNodules; i++)
		for(j = i; j < numNodules; j++)
			if(order[i] < order[j]) {
				id[i] = ((nodulePopulation.n_subpobl - 1) * numNodules) + j;
				id[j] = ((nodulePopulation.n_subpobl-1) * numNodules) + i;
				order[i] = nodulePopulation.nodulos[((nodulePopulation.n_subpobl - 1) * numNodules) + j]->aptitude;
				order[j] = nodulePopulation.nodulos[((nodulePopulation.n_subpobl - 1) * numNodules) + i]->aptitude;
			}

	/*
	  We substitude the best nodules of the descendant subpopulation by the worst
	  nodules of the original subpopulation. 
	*/
	for(i = numNodules; i > nodsel; i--){
		for(j = 0; j < netPopulation.n_redes; j++)
			if(netPopulation.redes[j]->nodulos[nodulePopulation.n_subpobl - 1] ==
			   nodulePopulation.nodulos[id[i - 1]])
				netPopulation.redes[j]->nodulos[nodulePopulation.n_subpobl - 1] =
					descendencia[i - nodsel - 1];
		liberarNodulo(nodulePopulation.nodulos[id[i - 1]]);
		free(nodulePopulation.nodulos[id[i - 1]]);
		nodulePopulation.nodulos[id[i - 1]] = descendencia[i - nodsel - 1];
		descendencia[i - nodsel - 1] = NULL;
	}

	for(i = numNodules - nodsel; i < numNodules; i++)
		liberarNodulo(descendencia[i]);
	free(descendencia);
	descendencia = NULL;
	ordenarNodulos();
	free(id);
	free(order);
}
