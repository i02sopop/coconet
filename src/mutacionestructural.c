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
 Function: copyDescendant()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Copy the actual nodule population into a new descendant
              population.
 Input Parameters: None
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   k: Integer. Counter.
   noduleOrig: Integer. Nodule to copy.
   initialNodule: Integer. Initial nodule of the descendant.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
******************************************************************************/

void copyDescendant()
{
	int i, j, k, noduleOrig, initialNodule;

	/* Initialization of variables. */
	initialNodule = numNodules * (cNodulePopulation.numSubpops - 1);
	descend = (nodule **)malloc(numNodules * sizeof(nodule));
	if(descend == NULL)
		error(RES_MEM);

	/* We copy the nodules to the new population. */
	for(i = 0; i < numNodules; i++) {
		descend[i] = (nodule *)malloc(sizeof(nodule));
		if(descend[i] == NULL)
			error(RES_MEM);

		/* We copy the nodule from the old to the new population. */
		noduleOrig = initialNodule + i;
		descend[i]->id = cNodulePopulation.nodules[noduleOrig]->id;
		descend[i]->nodes = cNodulePopulation.nodules[noduleOrig]->nodes;
		descend[i]->aptitude = cNodulePopulation.nodules[noduleOrig]->aptitude;

		descend[i]->inConn = (int **)malloc(cNetPopulation.numInputNodes * sizeof(int));
		descend[i]->inWeights = (double **)malloc(cNetPopulation.numInputNodes * sizeof(double));
		descend[i]->outConn = (int **)malloc(maxNodes * sizeof(int));
		descend[i]->outWeights = (double **)malloc(maxNodes * sizeof(double));
		descend[i]->transf = (func *)malloc(maxNodes * sizeof(func));
		descend[i]->partialOutputs = (double **)malloc(numTrain * sizeof(double));
		if(descend[i]->inConn == NULL ||
		   descend[i]->inWeights == NULL ||
		   descend[i]->outConn == NULL ||
		   descend[i]->outWeights == NULL ||
		   descend[i]->transf == NULL ||
		   descend[i]->partialOutputs == NULL)
			error(RES_MEM);

		for(j = 0; j < cNetPopulation.numInputNodes; j++) {
			descend[i]->inConn[j] = (int *)malloc(maxNodes * sizeof(int));
			descend[i]->inWeights[j] = (double *)malloc(maxNodes * sizeof(double));
			if(descend[i]->inConn[j] == NULL ||
			   descend[i]->inWeights[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < maxNodes; k++) {
				descend[i]->inConn[j][k] = cNodulePopulation.nodules[noduleOrig]->inConn[j][k];
				descend[i]->inWeights[j][k] = cNodulePopulation.nodules[noduleOrig]->inWeights[j][k];
			} /* end for */
		} /* end for */

		for(j = 0; j < maxNodes; j++) {
			descend[i]->outConn[j] = (int *)malloc(cNetPopulation.numOutputNodes * sizeof(int));
			descend[i]->outWeights[j] = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
			if(descend[i]->outConn[j] == NULL ||
				descend[i]->outWeights[j] == NULL)
				error(RES_MEM);

			descend[i]->transf[j] = cNodulePopulation.nodules[noduleOrig]->transf[j];

			for(k = 0; k < cNetPopulation.numOutputNodes; k++) {
				descend[i]->outConn[j][k] = cNodulePopulation.nodules[noduleOrig]->outConn[j][k];
				descend[i]->outWeights[j][k] = cNodulePopulation.nodules[noduleOrig]->outWeights[j][k];
			} /* end for */
		} /* end for */

		for(j = 0; j < numTrain; j++) {
			descend[i]->partialOutputs[j] = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
			if(descend[i]->partialOutputs[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < cNetPopulation.numOutputNodes; k++)
				descend[i]->partialOutputs[j][k] = cNodulePopulation.nodules[noduleOrig]->partialOutputs[j][k];
		} /* end for */
	} /* end for */
}

/******************************************************************************
 File: mutacionEstructural.c
 Function: mutasteNodules()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Make an estructural mutation in a given nodule.
 Input Parameters:
   nodule: Integer. Number of nodule to mutate.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   num: Integer. Number of mutation elements.
 Return Value: None
 Calling Functions: 
   addNode(): Add a node to the nodule.
   deleteNode(): Delete a node from the nodule.
   addConnection(): Add a connection to the nodule.
   deleteConnection(): Delete a connection from the nodule.
   measureNoduleChange(): Calculate the nodules aptitude variation after the
                        mutation.
   doubleRandom(): Returns a random float of double precision.
******************************************************************************/

void mutasteNodules(int nodule)
{
	int i, j, num;

	/* We made the estructural mutation. */
	/* Deletion of nodes. */
	num = (int)(delta_min + doubleRandom() *
				(1 - cNodulePopulation.nodules[nodule]->aptitude) *
				(delta_min - delta_max));
	if(num < 0)
		num = 0;
	else if(cNodulePopulation.nodules[nodule]->nodes < num)
		num = cNodulePopulation.nodules[nodule]->nodes;
	else if(num > 0)
		deleteNode(nodule, num);

	/* Add nodes. */
	num = (int)(delta_min + doubleRandom() *
				(1 - cNodulePopulation.nodules[nodule]->aptitude) *
				(delta_min - delta_max));
	if(num < 0)
		num = 0;
	else if((cNodulePopulation.nodules[nodule]->nodes + num) > maxNodes)
		num = maxNodes - cNodulePopulation.nodules[nodule]->nodes;
	else if(num > 0)
		addNode(nodule, num); /* We add a new node,*/

	/* Delete connections. */
	num = (int)(delta_min + doubleRandom() *
				(1 - cNodulePopulation.nodules[nodule]->aptitude) *
				(delta_min - delta_max));
	for(i = 0; i < num && cNodulePopulation.nodules[nodule]->nodes > 0; i++) {
		j = random() % (cNetPopulation.numInputNodes + cNetPopulation.numOutputNodes);
		deleteConnection(nodule, j);
	}

	/* Add connections. */
	num = (int)(delta_min + doubleRandom() *
				(1 - cNodulePopulation.nodules[nodule]->aptitude) *
				(delta_min - delta_max));
	for(i = 0; i < num && cNodulePopulation.nodules[nodule]->nodes > 0; i++)
		addConnection(nodule);

	/*
	  We make the calculations over the modified nodule, both his partial
	  outputs and his aptitude.
	*/
	measureNoduleChange(nodule);
}

/******************************************************************************
 File: mutacionEstructural.c
 Function: addConnection()
 Author: Pablo Álvarez de Sotomayor Posadillo
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

void addConnection(int nodule)
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
		for(i = 0; i < cNetPopulation.numInputNodes; i++)
			for(j = 0; j < cNodulePopulation.nodules[nodule]->nodes; j++)
				if(cNodulePopulation.nodules[nodule]->inConn[i][j] == 0) {
					origin = (int *)realloc(origin, (num + 1) * sizeof(int));
					destination = (int *)realloc(destination, (num + 1) * sizeof(int));
					origin[num] = i;
					destination[num] = j;
					num++;
				} /* end if */

		if(num != 0){
			pos = random() % num;
			cNodulePopulation.nodules[nodule]->inConn[origin[pos]][destination[pos]] = 1;
			cNodulePopulation.nodules[nodule]->inWeights[origin[pos]][destination[pos]] = doubleRandom() / 2;
			free(origin);
			free(destination);
		} /* end if */
	} /* end if */

	if((sel == 1 && num == 0) || num == 0) {
		/* We add an output connection. */
		for(i = 0; i < cNodulePopulation.nodules[nodule]->nodes; i++)
			for(j = 0; j < cNetPopulation.numOutputNodes; j++)
				if(cNodulePopulation.nodules[nodule]->outConn[i][j] == 0) {
					origin = (int *)realloc(origin, (num + 1) * sizeof(int));
					destination = (int *)realloc(destination, (num + 1) * sizeof(int));
					origin[num] = i;
					destination[num] = j;
					num++;
				} /* end if */

		if(num != 0) {
			pos = random() % num;
			cNodulePopulation.nodules[nodule]->outConn[origin[pos]][destination[pos]] = 1;
			cNodulePopulation.nodules[nodule]->outWeights[origin[pos]][destination[pos]] = doubleRandom() / 2;
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
 Function: addNode()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Add some nodes to a nodule.
 Input Parameters:
   nodule: Integer. Number of nodule to work with.
   nodes: Integer. Number of nodes to add.
 Local Variables:
   i: Integer. Counter
 Return Value: None
 Calling Functions: None
*******************************************************************************/

void addNode(int nodule, int nodes)
{
	int i;

	for(i = 0; i < nodes; i++) {
		/* We assign the transfer function to the node. */
		if((random() % 2) == 0)
			cNodulePopulation.nodules[nodule]->transf[cNodulePopulation.nodules[nodule]->nodes + i] = (func)&HyperbolicTangent;
		else
			cNodulePopulation.nodules[nodule]->transf[cNodulePopulation.nodules[nodule]->nodes + i] = (func)&Logistic;
	}

	/* We update the number of nodes. */
	cNodulePopulation.nodules[nodule]->nodes += nodes;
}

/*******************************************************************************
 File: mutacionEstructural.c
 Function: deleteConnection()
 Author: Pablo Álvarez de Sotomayor Posadillo
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

void deleteConnection(int nodule, int node)
{
	int i, sel;

	if(node < cNetPopulation.numInputNodes){
		/* We delete an input connection. */
		for(i = 0; i < cNodulePopulation.nodules[nodule]->nodes &&
				cNodulePopulation.nodules[nodule]->inConn[node][i] != 1; i++);

		if(i < cNodulePopulation.nodules[nodule]->nodes){
			sel = random() % cNodulePopulation.nodules[nodule]->nodes;
			for(; cNodulePopulation.nodules[nodule]->inConn[node][sel] != 1;
				sel = random() % cNodulePopulation.nodules[nodule]->nodes);
			cNodulePopulation.nodules[nodule]->inConn[node][sel] = 0;
			cNodulePopulation.nodules[nodule]->inWeights[node][sel] = 0;
		} /* end if */
	} else {
		/* We delete an output connection. */
		node -= cNetPopulation.numInputNodes;
		for(i = 0; i < cNodulePopulation.nodules[nodule]->nodes &&
				cNodulePopulation.nodules[nodule]->outConn[i][node] != 1; i++);

		if(i < cNodulePopulation.nodules[nodule]->nodes) {
			sel = random() % cNodulePopulation.nodules[nodule]->nodes;
			for(; cNodulePopulation.nodules[nodule]->outConn[sel][node] != 1;
				sel = random() % cNodulePopulation.nodules[nodule]->nodes);
			cNodulePopulation.nodules[nodule]->outConn[sel][node] = 0;
			cNodulePopulation.nodules[nodule]->outWeights[sel][node] = 0;
		} /* end if */
	} /* end if */
}

/********************************************************************************
 File: mutacionEstructural.c
 Function: deleteNode()
 Author: Pablo Álvarez de Sotomayor Posadillo
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

void deleteNode(int nodule, int nodes)
{
	int i, j, k, node;
  
	for(k = 0; k < nodes; k++) {
		node = random() % cNodulePopulation.nodules[nodule]->nodes;

		/* All the nodes after the node to delete have to go back a position. */
		for(i = node + 1; i < cNodulePopulation.nodules[nodule]->nodes; i++) {
			cNodulePopulation.nodules[nodule]->transf[i - 1] =
				cNodulePopulation.nodules[nodule]->transf[i];
			for(j = 0; j < cNetPopulation.numOutputNodes; j++) {
				cNodulePopulation.nodules[nodule]->outConn[i - 1][j] =
					cNodulePopulation.nodules[nodule]->outConn[i][j];
				cNodulePopulation.nodules[nodule]->outWeights[i - 1][j] =
					cNodulePopulation.nodules[nodule]->outWeights[i][j];
				cNodulePopulation.nodules[nodule]->outConn[i][j] = 0;
				cNodulePopulation.nodules[nodule]->outWeights[i][j] = 0;
			} /* end for */
		} /* end for */

		for(j = 0; j < cNetPopulation.numOutputNodes; j++) {
			cNodulePopulation.nodules[nodule]->outConn[i - 1][j] = 0;
			cNodulePopulation.nodules[nodule]->outWeights[i - 1][j] = 0;
		} /* end for */

		for(i = 0; i < cNetPopulation.numInputNodes; i++) {
			for(j = node + 1; j < cNodulePopulation.nodules[nodule]->nodes; j++) {
				cNodulePopulation.nodules[nodule]->inConn[i][j - 1] =
					cNodulePopulation.nodules[nodule]->inConn[i][j];
				cNodulePopulation.nodules[nodule]->inWeights[i][j - 1] =
					cNodulePopulation.nodules[nodule]->inWeights[i][j];
				cNodulePopulation.nodules[nodule]->inConn[i][j] = 0;
				cNodulePopulation.nodules[nodule]->inWeights[i][j] = 0;
			} /* end for */

			cNodulePopulation.nodules[nodule]->inConn[i][j-1] = 0;
			cNodulePopulation.nodules[nodule]->inWeights[i][j-1] = 0;
		} /* end for */
  
		cNodulePopulation.nodules[nodule]->nodes--;
	} /* end for */
}

/********************************************************************************
 File: mutacionEstructural.c
 Function: replaceNodules()
 Author: Pablo Álvarez de Sotomayor Posadillo
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
   freeNodule(): Free the memory ocuppied by a given nodule.
   sortNodules(): Sort the nodules of the population.
   error(): Function to show an error message depending on an error number.
*******************************************************************************/

void replaceNodules()
{
	int i, j, *id;
	double *order;

	/* We order the nodules by aptitude. */
	id = (int *)malloc(numNodules * sizeof(int));
	order = (double *)malloc(numNodules * sizeof(double));
	if(id == NULL || order == NULL)
		error(RES_MEM);

	for(i = 0; i < numNodules; i++) {
		id[i] = ((cNodulePopulation.numSubpops - 1) * numNodules) + i;
		order[i] =
			cNodulePopulation.nodules[(cNodulePopulation.numSubpops - 1) * numNodules + i]->aptitude;
	}

	for(i = 0; i < numNodules; i++)
		for(j = i; j < numNodules; j++)
			if(order[i] < order[j]) {
				id[i] = ((cNodulePopulation.numSubpops - 1) * numNodules) + j;
				id[j] = ((cNodulePopulation.numSubpops - 1) * numNodules) + i;
				order[i] = cNodulePopulation.nodules[((cNodulePopulation.numSubpops - 1) * numNodules) + j]->aptitude;
				order[j] = cNodulePopulation.nodules[((cNodulePopulation.numSubpops - 1) * numNodules) + i]->aptitude;
			}

	/*
	  We substitude the best nodules of the descendant subpopulation by the worst
	  nodules of the original subpopulation. 
	*/

	for(i = numNodules; i > nodSel; i--){
		for(j = 0; j < cNetPopulation.numNets; j++)
			if(cNetPopulation.nets[j]->nodules[cNodulePopulation.numSubpops - 1] ==
			   cNodulePopulation.nodules[id[i - 1]])
				cNetPopulation.nets[j]->nodules[cNodulePopulation.numSubpops - 1] =
					descend[i - nodSel - 1];
		freeNodule(cNodulePopulation.nodules[id[i - 1]]);
		free(cNodulePopulation.nodules[id[i - 1]]);
		cNodulePopulation.nodules[id[i - 1]] = descend[i - nodSel - 1];
		descend[i - nodSel - 1] = NULL;
	}

	for(i = numNodules - nodSel; i < numNodules; i++)
		freeNodule(descend[i]);
	free(descend);
	descend = NULL;
	sortNodules();
	free(id);
	free(order);
}
