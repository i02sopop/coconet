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
 * Add a new nodule subpopulation to the network population.                     *
 * @returns void                                                                 *
 ********************************************************************************/
void
addNodulesNetworks() {
	/* If it's the first nodes population we create a network per node. */
	if (cNodulePopulation.numSubpops == 1) {
		cNetPopulation.numNets = numNodules;
		cNetPopulation.nets = (network **)malloc(numNodules * sizeof(network));
		if (cNetPopulation.nets == NULL)
			error(RES_MEM);
		createNetworks();
	} else {
		/* We create the new population adding a new node to each network. */
		createDescendant();
	}
}

/*********************************************************************************
 * Create a new network population from a new nodule population and the previous *
 * network population.                                                           *
 * @returns void                                                                 *
 ********************************************************************************/
void
createDescendant() {
	int i, j;
	int net; /* Selection of a network of the present network. */
	int node; /* Selection of a nodule from the new subpopulation. */
	network **dnet; /* New network population. Descend of the actual population. */

	/* Creation of the new network. */
	dnet = (network **)malloc(maxNetworks * sizeof(network));
	if (dnet == NULL)
		error(RES_MEM);

	for (i = 0; i < maxNetworks; i++) {
		dnet[i] = (network *)malloc(sizeof(network));
		if (dnet[i] == NULL)
			error(RES_MEM);

		dnet[i]->aptitude = 0;
		dnet[i]->outValues = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
		dnet[i]->nodules = (nodule **)malloc(cNodulePopulation.numSubpops * sizeof(nodule));
		if (dnet[i]->outValues == NULL || dnet[i]->nodules == NULL)
			error(RES_MEM);

		/* Select a new network randomly. */
		net = random() % cNetPopulation.numNets;

		/* Random selection of the node to add. */
		node = random() % numNodules;

		/* Creation of the new network. */
		for (j = 0; j < cNodulePopulation.numSubpops - 1; j++)
			dnet[i]->nodules[j] = cNetPopulation.nets[net]->nodules[j];

		dnet[i]->nodules[cNodulePopulation.numSubpops - 1] =
			cNodulePopulation.nodules[(cNodulePopulation.numSubpops - 1) * numNodules + node];
	}

	/* We fix the new network. */
	for (i = 0; i < cNetPopulation.numNets; i++)
		freeNetwork(cNetPopulation.nets[i]);

	free(cNetPopulation.nets);
	cNetPopulation.nets = dnet;
	dnet = NULL;
}

/*********************************************************************************
 * Function to free a given network.                                             *
 * @param network* net: Network to delete.                                       *
 * @returns void                                                                 *
 ********************************************************************************/
void
freeNetwork(network *net) {
	int i;

	/* We delete all the network. */
	for (i = 0; i < cNodulePopulation.numSubpops; i++)
		net->nodules[i] = NULL;

	free(net->nodules);
	free(net->outValues);
	free(net);
}

/*********************************************************************************
 * Create a new nodule subpopulation.                                            *
 * @return void.                                                                 *
 ********************************************************************************/
void
createNodules() {
	int i, j, k;

	/* We create the nodule subpopulation. */
	for (i = cNodulePopulation.numNodules - numNodules; i < cNodulePopulation.numNodules; i++) {
		cNodulePopulation.nodules[i] = (nodule *)malloc(sizeof(nodule));
		if (cNodulePopulation.nodules[i] == NULL)
			error(RES_MEM);

		/* Nodule id. */
		cNodulePopulation.nodules[i]->id = i;

		/* Number of hidden nodes of the nodule. */
		cNodulePopulation.nodules[i]->nodes = random() % maxNodes;

		/* Initial aptitude. */
		cNodulePopulation.nodules[i]->aptitude = 0;

		if(cNodulePopulation.nodules[i]->nodes == 0)
			cNodulePopulation.nodules[i]->nodes++;

		/* Memory allocation for the creation of connections and weights. */
		/* Connections from input to hidden nodes. */
		cNodulePopulation.nodules[i]->inConn = (int **)malloc(cNetPopulation.numInputNodes * sizeof(int));

		/* Connections from hidden to output nodes. */
		cNodulePopulation.nodules[i]->outConn = (int **)malloc(maxNodes * sizeof(int));

		/* Weights of connections between input and hidden nodes. */
		cNodulePopulation.nodules[i]->inWeights = (double **)malloc(cNetPopulation.numInputNodes * sizeof(double));

		/* Weights of connections between hidden and output nodes. */
		cNodulePopulation.nodules[i]->outWeights = (double **)malloc(maxNodes * sizeof(double));
		cNodulePopulation.nodules[i]->partialOutputs = (double **)malloc(numTrain * sizeof(double));
		cNodulePopulation.nodules[i]->transf = (func *)malloc(maxNodes * sizeof(func));

		if (cNodulePopulation.nodules[i]->inConn == NULL || cNodulePopulation.nodules[i]->outConn == NULL ||
			cNodulePopulation.nodules[i]->inWeights == NULL || cNodulePopulation.nodules[i]->outWeights == NULL ||
			cNodulePopulation.nodules[i]->partialOutputs == NULL || cNodulePopulation.nodules[i]->transf == NULL)
			error(RES_MEM);

		/* Creation of connections and weigths. */
		/* Entry connections and weights. */
		for (j = 0; j < cNetPopulation.numInputNodes; j++) {
			cNodulePopulation.nodules[i]->inConn[j] = (int *)malloc(maxNodes * sizeof(int));
			cNodulePopulation.nodules[i]->inWeights[j] = (double *)malloc(maxNodes * sizeof(double));
			if (cNodulePopulation.nodules[i]->inConn[j] == NULL ||
			    cNodulePopulation.nodules[i]->inWeights[j] == NULL)
				error(RES_MEM);

			for (k = 0; k < maxNodes; k++) {
				if (random() % 2 == 1 && k < cNodulePopulation.nodules[i]->nodes) {
					cNodulePopulation.nodules[i]->inConn[j][k] = 1;
					cNodulePopulation.nodules[i]->inWeights[j][k] = doubleRandom() / 2;
				} else {
					cNodulePopulation.nodules[i]->inConn[j][k] = 0;
					cNodulePopulation.nodules[i]->inWeights[j][k] = 0.0;
				}
			}
		}

		/* Output connections and weights. */
		for (j = 0; j < maxNodes; j++) {
			cNodulePopulation.nodules[i]->outConn[j] = (int *)malloc(cNetPopulation.numOutputNodes * sizeof(int));
			cNodulePopulation.nodules[i]->outWeights[j] = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
			if (cNodulePopulation.nodules[i]->outConn[j] == NULL ||
			    cNodulePopulation.nodules[i]->outWeights[j] == NULL)
				error(RES_MEM);

			for (k = 0; k < cNetPopulation.numOutputNodes; k++) {
				if (random() % 2 == 1 && j < cNodulePopulation.nodules[i]->nodes) {
					cNodulePopulation.nodules[i]->outConn[j][k] = 1;
					cNodulePopulation.nodules[i]->outWeights[j][k] = doubleRandom() / 2;
				} else {
					cNodulePopulation.nodules[i]->outConn[j][k] = 0;
					cNodulePopulation.nodules[i]->outWeights[j][k] = 0.0;
				}
			}
		}

		/* Partial outputs. */
		for (j = 0; j < numTrain; j++) {
			cNodulePopulation.nodules[i]->partialOutputs[j] = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
			if (cNodulePopulation.nodules[i]->partialOutputs[j] == NULL)
				error(RES_MEM);

			for (k = 0; k < cNetPopulation.numOutputNodes; k++)
				cNodulePopulation.nodules[i]->partialOutputs[j][k] = 0.0;
		}

		/* Assign the transfer function to each node. */
		for(j = 0; j < cNodulePopulation.nodules[i]->nodes; j++)
			cNodulePopulation.nodules[i]->transf[j] = (random() % 2 == 0) ? (func)&HyperbolicTangent : (func)&Logistic;
	}
}

/*********************************************************************************
 * Create or expand the nodule population.                                       *
 * @return void.                                                                 *
 ********************************************************************************/
void
createNodulePopulation() {
	/* We update the number of subpopulations and nodules of the nodule
	 * population. */
	cNodulePopulation.numSubpops++;
	cNodulePopulation.numNodules = cNodulePopulation.numSubpops * numNodules;

	/* Memory allocation for the new subpopulation. */
	cNodulePopulation.nodules = (cNodulePopulation.numSubpops == 1) ?
		(nodule **)malloc(numNodules * sizeof(nodule)) :
		(nodule **)realloc(cNodulePopulation.nodules, cNodulePopulation.numNodules * sizeof(nodule));
	if (cNodulePopulation.nodules == NULL)
		error(RES_MEM);

	/* We create the nodules of the new subpopulation. */
	createNodules();
}

/*********************************************************************************
 * Create a new network population.                                              *
 * @return void.                                                                 *
 ********************************************************************************/
void
createNetworks() {
	int i, j;

	for (i = 0; i < numNodules; i++) {
		if ((cNetPopulation.nets[i] = (network *)malloc(sizeof(network))) == NULL)
			error(RES_MEM);

		/* Allocation of memory. */
		cNetPopulation.nets[i]->nodules = (nodule **)malloc(sizeof(nodule));
		cNetPopulation.nets[i]->outValues = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
		if (cNetPopulation.nets[i]->nodules == NULL || cNetPopulation.nets[i]->outValues == NULL)
			error(RES_MEM);

		/* Initialization of variables. */
		cNetPopulation.nets[i]->aptitude = 0.0;
		cNetPopulation.nets[i]->nodules[0] = cNodulePopulation.nodules[i];
		for (j = 0; j < cNetPopulation.numOutputNodes; j++)
			cNetPopulation.nets[i]->outValues[j] = 0.0;
	}
}
