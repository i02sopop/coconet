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

#include <definitions.h>

/*********************************************************************************
 * Copy the actual nodule population into a new descendant population.           *
 * @return void                                                                  *
 ********************************************************************************/
void
copyDescendant() {
	int i, j, k;
	int noduleOrig; /* Nodule to copy. */
	int initialNodule; /* Initial nodule of the descendant. */

	/* Initialization of variables. */
	initialNodule = numNodules * (cNodulePopulation.numSubpops - 1);
	descend = (nodule **)malloc(numNodules * sizeof(nodule));
	if (descend == NULL)
		error(RES_MEM);

	/* Copy the nodules to the new population. */
	for (i = 0; i < numNodules; i++) {
		descend[i] = (nodule *)malloc(sizeof(nodule));
		if (descend[i] == NULL)
			error(RES_MEM);

		/* Copy the nodule from the old to the new population. */
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
		if (descend[i]->inConn == NULL || descend[i]->inWeights == NULL ||
		   descend[i]->outConn == NULL || descend[i]->outWeights == NULL ||
		   descend[i]->transf == NULL || descend[i]->partialOutputs == NULL)
			error(RES_MEM);

		for (j = 0; j < cNetPopulation.numInputNodes; j++) {
			descend[i]->inConn[j] = (int *)malloc(maxNodes * sizeof(int));
			descend[i]->inWeights[j] = (double *)malloc(maxNodes * sizeof(double));
			if (descend[i]->inConn[j] == NULL ||
			   descend[i]->inWeights[j] == NULL)
				error(RES_MEM);

			for (k = 0; k < maxNodes; k++) {
				descend[i]->inConn[j][k] = cNodulePopulation.nodules[noduleOrig]->inConn[j][k];
				descend[i]->inWeights[j][k] = cNodulePopulation.nodules[noduleOrig]->inWeights[j][k];
			}
		}

		for (j = 0; j < maxNodes; j++) {
			descend[i]->outConn[j] = (int *)malloc(cNetPopulation.numOutputNodes * sizeof(int));
			descend[i]->outWeights[j] = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
			if (descend[i]->outConn[j] == NULL ||
				descend[i]->outWeights[j] == NULL)
				error(RES_MEM);

			descend[i]->transf[j] = cNodulePopulation.nodules[noduleOrig]->transf[j];

			for (k = 0; k < cNetPopulation.numOutputNodes; k++) {
				descend[i]->outConn[j][k] = cNodulePopulation.nodules[noduleOrig]->outConn[j][k];
				descend[i]->outWeights[j][k] = cNodulePopulation.nodules[noduleOrig]->outWeights[j][k];
			}
		}

		for (j = 0; j < numTrain; j++) {
			descend[i]->partialOutputs[j] = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
			if (descend[i]->partialOutputs[j] == NULL)
				error(RES_MEM);

			for (k = 0; k < cNetPopulation.numOutputNodes; k++)
				descend[i]->partialOutputs[j][k] =
				    cNodulePopulation.nodules[noduleOrig]->partialOutputs[j][k];
		}
	}
}

/*********************************************************************************
 * Make an estructural mutation in a given nodule.                               *
 * @param int nodule: Number of nodule to mutate.                                *
 * @return void                                                                  *
 ********************************************************************************/
void
mutateNodules(int nodule) {
	int i, j, num;

	/* Make the estructural mutation. */
	/* Delete nodes. */
	num = (int)(delta_min + doubleRandom() *
				(1 - cNodulePopulation.nodules[nodule]->aptitude) *
				(delta_min - delta_max));
	if (num < 0)
		num = 0;
	else if (cNodulePopulation.nodules[nodule]->nodes < num)
		num = cNodulePopulation.nodules[nodule]->nodes;
	else if (num > 0)
		deleteNode(nodule, num);

	/* Add nodes. */
	num = (int)(delta_min + doubleRandom() *
				(1 - cNodulePopulation.nodules[nodule]->aptitude) *
				(delta_min - delta_max));
	if (num < 0)
		num = 0;
	else if ((cNodulePopulation.nodules[nodule]->nodes + num) > maxNodes)
		num = maxNodes - cNodulePopulation.nodules[nodule]->nodes;
	else if (num > 0)
		addNode(nodule, num); /* Add a new node. */

	/* Delete connections. */
	num = (int)(delta_min + doubleRandom() *
				(1 - cNodulePopulation.nodules[nodule]->aptitude) *
				(delta_min - delta_max));
	for (i = 0; i < num && cNodulePopulation.nodules[nodule]->nodes > 0; i++) {
		j = random() % (cNetPopulation.numInputNodes + cNetPopulation.numOutputNodes);
		deleteConnection(nodule, j);
	}

	/* Add connections. */
	num = (int)(delta_min + doubleRandom() *
				(1 - cNodulePopulation.nodules[nodule]->aptitude) *
				(delta_min - delta_max));
	for (i = 0; i < num && cNodulePopulation.nodules[nodule]->nodes > 0; i++)
		addConnection(nodule);

	/* Make the calculations over the modified nodule, both its partial outputs
	 * and aptitude. */
	measureNoduleChange(nodule);
}

/*********************************************************************************
 * Add a connection to a given nodule.                                           *
 * @param int nodule: Number of nodule to modify.                                *
 * @return void                                                                  *
 ********************************************************************************/
void
addConnection(int nodule) {
	int i, j, num, pos;
	int sel; /* Type of connection to add (input, internal or output). */
	int *origin; /* Possible origin nodes for the connection. */
	int *destination; /* Possible destination nodes for the connection. */

	num = 0;
	sel = random() % 2;
	origin = (int *)malloc(sizeof(int));
	destination = (int *)malloc(sizeof(int));
	if (origin == NULL || destination == NULL)
		error(RES_MEM);

	if (sel == 0) {
		/* Add an input connection. */
		for (i = 0; i < cNetPopulation.numInputNodes; i++)
			for (j = 0; j < cNodulePopulation.nodules[nodule]->nodes; j++)
				if (cNodulePopulation.nodules[nodule]->inConn[i][j] == 0) {
					origin = (int *)realloc(origin, (num + 1) * sizeof(int));
					destination = (int *)realloc(destination, (num + 1) * sizeof(int));
					origin[num] = i;
					destination[num] = j;
					num++;
				}

		if (num != 0) {
			pos = random() % num;
			cNodulePopulation.nodules[nodule]->inConn[origin[pos]][destination[pos]] = 1;
			cNodulePopulation.nodules[nodule]->inWeights[origin[pos]][destination[pos]] = doubleRandom() / 2;
			free(origin);
			free(destination);
		}
	}

	if ((sel == 1 && num == 0) || num == 0) {
		/* Add an output connection. */
		for (i = 0; i < cNodulePopulation.nodules[nodule]->nodes; i++)
			for (j = 0; j < cNetPopulation.numOutputNodes; j++)
				if (cNodulePopulation.nodules[nodule]->outConn[i][j] == 0) {
					origin = (int *)realloc(origin, (num + 1) * sizeof(int));
					destination = (int *)realloc(destination, (num + 1) * sizeof(int));
					origin[num] = i;
					destination[num] = j;
					num++;
				}

		if (num != 0) {
			pos = random() % num;
			cNodulePopulation.nodules[nodule]->outConn[origin[pos]][destination[pos]] = 1;
			cNodulePopulation.nodules[nodule]->outWeights[origin[pos]][destination[pos]] = doubleRandom() / 2;
			free(origin);
			free(destination);
		}
	}

	if (num == 0) {
		free(origin);
		free(destination);
	}
}

/*********************************************************************************
 * Add some nodes to a nodule.                                                   *
 * @param int nodule: Number of nodule to work with.                             *
 * @param int nodes: Number of nodes to add.                                     *
 * @return void                                                                  *
 ********************************************************************************/
void
addNode(int nodule, int nodes) {
	int i;

	for (i = 0; i < nodes; i++) {
		/* Assign the transfer function to the node. */
		cNodulePopulation.nodules[nodule]->transf[cNodulePopulation.nodules[nodule]->nodes + i] =
			((random() % 2) == 0) ? (func)&HyperbolicTangent : (func)&Logistic;
	}

	/* Update the number of nodes. */
	cNodulePopulation.nodules[nodule]->nodes += nodes;
}

/*********************************************************************************
 * Delete a connection of a given node.                                          *
 * @param int nodule: Number of nodule to work with.                             *
 * @param int node: Node to delete the connection from.                          *
 * @return void                                                                  *
 ********************************************************************************/
void
deleteConnection(int nodule, int node) {
	int i, sel; /* Connection to delete. XXX: Rename it. */

	if (node < cNetPopulation.numInputNodes){
		/* Delete an input connection. */
		for (i = 0; i < cNodulePopulation.nodules[nodule]->nodes &&
				 cNodulePopulation.nodules[nodule]->inConn[node][i] != 1; i++);

		if (i < cNodulePopulation.nodules[nodule]->nodes){
			sel = random() % cNodulePopulation.nodules[nodule]->nodes;
			for (; cNodulePopulation.nodules[nodule]->inConn[node][sel] != 1;
				sel = random() % cNodulePopulation.nodules[nodule]->nodes);
			cNodulePopulation.nodules[nodule]->inConn[node][sel] = 0;
			cNodulePopulation.nodules[nodule]->inWeights[node][sel] = 0;
		}
	} else {
		/* Delete an output connection. */
		node -= cNetPopulation.numInputNodes;
		for (i = 0; i < cNodulePopulation.nodules[nodule]->nodes &&
				 cNodulePopulation.nodules[nodule]->outConn[i][node] != 1; i++);

		if (i < cNodulePopulation.nodules[nodule]->nodes) {
			sel = random() % cNodulePopulation.nodules[nodule]->nodes;
			for (; cNodulePopulation.nodules[nodule]->outConn[sel][node] != 1;
				sel = random() % cNodulePopulation.nodules[nodule]->nodes);
			cNodulePopulation.nodules[nodule]->outConn[sel][node] = 0;
			cNodulePopulation.nodules[nodule]->outWeights[sel][node] = 0;
		}
	}
}

/*********************************************************************************
 * Delete a given number of nodes from a nodule.                                 *
 * @param int nodule: Number of nodule to work with.                             *
 * @param int nodes: Number of nodes to delete.                                  *
 * @return void                                                                  *
 ********************************************************************************/
void
deleteNode(int nodule, int nodes) {
	int i, j, k, node; /* Node to delete. XXX: Rename it. */
  
	for (k = 0; k < nodes; k++) {
		node = random() % cNodulePopulation.nodules[nodule]->nodes;

		/* All the nodes after the node to delete have to go back a position. */
		for (i = node + 1; i < cNodulePopulation.nodules[nodule]->nodes; i++) {
			cNodulePopulation.nodules[nodule]->transf[i - 1] =
				cNodulePopulation.nodules[nodule]->transf[i];
			for (j = 0; j < cNetPopulation.numOutputNodes; j++) {
				cNodulePopulation.nodules[nodule]->outConn[i - 1][j] =
					cNodulePopulation.nodules[nodule]->outConn[i][j];
				cNodulePopulation.nodules[nodule]->outWeights[i - 1][j] =
					cNodulePopulation.nodules[nodule]->outWeights[i][j];
				cNodulePopulation.nodules[nodule]->outConn[i][j] = 0;
				cNodulePopulation.nodules[nodule]->outWeights[i][j] = 0;
			}
		}

		for (j = 0; j < cNetPopulation.numOutputNodes; j++) {
			cNodulePopulation.nodules[nodule]->outConn[i - 1][j] = 0;
			cNodulePopulation.nodules[nodule]->outWeights[i - 1][j] = 0;
		}

		for (i = 0; i < cNetPopulation.numInputNodes; i++) {
			for (j = node + 1; j < cNodulePopulation.nodules[nodule]->nodes; j++) {
				cNodulePopulation.nodules[nodule]->inConn[i][j - 1] =
					cNodulePopulation.nodules[nodule]->inConn[i][j];
				cNodulePopulation.nodules[nodule]->inWeights[i][j - 1] =
					cNodulePopulation.nodules[nodule]->inWeights[i][j];
				cNodulePopulation.nodules[nodule]->inConn[i][j] = 0;
				cNodulePopulation.nodules[nodule]->inWeights[i][j] = 0;
			}

			cNodulePopulation.nodules[nodule]->inConn[i][j-1] = 0;
			cNodulePopulation.nodules[nodule]->inWeights[i][j-1] = 0;
		}
  
		cNodulePopulation.nodules[nodule]->nodes--;
	}
}

/*********************************************************************************
 * Replace the worst nodules of a population by the best nodules of its          *
 * descendant population.                                                        *
 * @return void                                                                  *
 ********************************************************************************/
void
replaceNodules() {
	int i, j, *id; /* Store the nodules id in order to sort them by aptitude. */
	double *order; /* Store the nodules aptitude in order to sort them. */

	/* Order the nodules by aptitude. */
	id = (int *)malloc(numNodules * sizeof(int));
	order = (double *)malloc(numNodules * sizeof(double));
	if (id == NULL || order == NULL)
		error(RES_MEM);

	for (i = 0; i < numNodules; i++) {
		id[i] = ((cNodulePopulation.numSubpops - 1) * numNodules) + i;
		order[i] =
			cNodulePopulation.nodules[(cNodulePopulation.numSubpops - 1) * numNodules + i]->aptitude;
	}

	for (i = 0; i < numNodules; i++)
		for (j = i; j < numNodules; j++)
			if (order[i] < order[j]) {
				id[i] = ((cNodulePopulation.numSubpops - 1) * numNodules) + j;
				id[j] = ((cNodulePopulation.numSubpops - 1) * numNodules) + i;
				order[i] = cNodulePopulation.nodules[((cNodulePopulation.numSubpops - 1) * numNodules) + j]->aptitude;
				order[j] = cNodulePopulation.nodules[((cNodulePopulation.numSubpops - 1) * numNodules) + i]->aptitude;
			}

	/* Replace the best nodules of the descendant subpopulation by the worst
	 * nodules of the original sub-population. */
	for (i = numNodules; i > nodSel; i--){
		for (j = 0; j < cNetPopulation.numNets; j++)
			if (cNetPopulation.nets[j]->nodules[cNodulePopulation.numSubpops - 1] ==
				cNodulePopulation.nodules[id[i - 1]])
				cNetPopulation.nets[j]->nodules[cNodulePopulation.numSubpops - 1] =
					descend[i - nodSel - 1];
		freeNodule(cNodulePopulation.nodules[id[i - 1]]);
		free(cNodulePopulation.nodules[id[i - 1]]);
		cNodulePopulation.nodules[id[i - 1]] = descend[i - nodSel - 1];
		descend[i - nodSel - 1] = NULL;
	}

	for (i = numNodules - nodSel; i < numNodules; i++)
		freeNodule(descend[i]);
	free(descend);
	descend = NULL;
	sortNodules();
	free(id);
	free(order);
}
