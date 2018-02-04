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
 * Evolve the networks and nodules populations.                               *
 * @return void                                                               *
 *****************************************************************************/
void
evolvePopulations() {
	int i, j, k, initNumNodules; /* Initial number of nodules of the subpopulation. */
	double networkAptitude; /* Store the average aptitude of the networks. */

	/* Local variables initialization. */
	networkAptitude = 0.0;
	for (i = 0; i < cNetPopulation.numNets; i++)
		cNetPopulation.nets[i]->aptitude = 0;

	/* Evolve the networks until its average aptitude doesn't get enhaced
	 * enough. */
	for (i = 0; measureChange(&networkAptitude, i) == false; i++) {
		initNumNodules = numNodules * (cNodulePopulation.numSubpops - 1);
		if (!i) {
			/* If the subpopulation is new we run the backpropagation to reduce
			 * its error. */
			xlog(0, "Doing the backpropagation to a new nodule subpopulation.\n");
			for (j = 0; j < numNodules; j++)
				backpropagation(initNumNodules + j, numTrain, 5000);

			/* Train by first time the networks to have an initial value of
			 * the nodules and networks aptitude. */
			xlog(0, "Train by first time the networks.\n");
			for (j = 0; j < numTrain; j++) {
				/* Generate the nodules output. */
				for (k = cNodulePopulation.numNodules - numNodules;
					 k < cNodulePopulation.numNodules; k++)
					generateNoduleOutput(inputData[j], k, j, NULL);

				/* Measure the networks aptitude. */
				for (k = 0; k < cNetPopulation.numNets; k++) {
					generateNetOutput(k, j);
					measureNetworkAptitude(outputData[j], k);
				}
			}

			/* Normalize the network aptitude. */
			for (j = 0; j < cNetPopulation.numNets; j++)
				cNetPopulation.nets[j]->aptitude /= numTrain;

			/* Measure and normalize the nodules aptitude. */
			for (j = 0; j < cNodulePopulation.numNodules; j++)
				measureNoduleAptitude(j);
			normalizeNoduleAptitude();
		}

		/* Sort the networks by aptitude. */
		xlog(0, "Sorting networks.\n");
		sortNetworks();

		/* Train the networks by simulate annealing. */
		xlog(0, "Training the networks by simulate annealing.\n");
		for (j = initNumNodules; j < cNodulePopulation.numNodules; j++)
			simulatedAnnealing(j);

		/* Normalize the nodules aptitude. */
		xlog(0, "Normalize the nodules aptitude.\n");
		normalizeNoduleAptitude();

		/* Sort the nodules by aptitude. */
		xlog(0, "Sorting nodules.\n");
		sortNodules();

		/* Create a new subpopulation descendant of the actual one. */
		xlog(0, "Creating new subpopulation.\n");
		copyDescendant();

		/* Make the structural mutation of the nodules. */
		xlog(0, "Nodules mutation.\n");
		for (j = initNumNodules; j < cNodulePopulation.numNodules; j++)
			mutateNodules(j);

		/* Replace the worst nodules by the best ones in the descendant
		 * population. */
		xlog(0, "Replace the worst nodules by the best ones.\n");
		replaceNodules();

		xlog(0, "End of the evolve.\n");
	}
}

/******************************************************************************
 * Run the backpropagation algorithm to a given nodule.                       *
 * @param int nodule: Number of the nodule to work with.                      *
 * @param int numPatterns: Number of input patterns.                          *
 * @param int iter: Number of iterations of the algorithm.                    *
 * @return void                                                               *
 *****************************************************************************/
void
backpropagation(int nodule, int numPatterns, int iter) {
	int i, j, k;
	int pattern; /* Pattern number selected to run the backpropagation. */
	int numNodes; /* Number of nodes of the nodule. */
	double *initialError; /* Store the initial error of each nodule. */
	double **F_W; /* Store the change to apply over the nodule weights. */
	double *out; /* Store the output of each node of the nodule. */
	double **weights; /* Store the connection weights of the nodule. */
	func *transf; /* Store the transfer functions of each node of the nodule. */

	/* Initializate the local variables. */
	numNodes = cNodulePopulation.nodules[nodule]->nodes +
		cNetPopulation.numInputNodes + cNetPopulation.numOutputNodes;
	transf = (func *)xmalloc((cNetPopulation.numOutputNodes +
							  cNodulePopulation.nodules[nodule]->nodes) * sizeof(func));
	out = (double *)xmalloc(numNodes * sizeof(double));
	F_W = (double **)xmalloc(numNodes * sizeof(double));
	weights = (double **)xmalloc(numNodes * sizeof(double));
	initialError = (double *)xmalloc(cNetPopulation.numOutputNodes * sizeof(double));
	for (i = 0; i < numNodes; i++) {
		F_W[i] = (double *)xmalloc(numNodes * sizeof(double));
		weights[i] = (double *)xmalloc(numNodes * sizeof(double));
		for (j = 0; j < numNodes; j++) {
			F_W[i][j] = 0.0;
			weights[i][j] = 0.0;
		}
	}

	for (i = 0; i < cNodulePopulation.nodules[nodule]->nodes; i++)
		transf[i] = cNodulePopulation.nodules[nodule]->transf[i];

	for (;i < cNodulePopulation.nodules[nodule]->nodes + cNetPopulation.numOutputNodes; i++)
		transf[i] = netTransf;

	/* Backpropagation. */
	for (k = 0; k < iter; k++) {
		/* Pick a pattern. */
		pattern = random() % numPatterns;
		generateNoduleOutput(inputData[pattern], nodule, pattern, out);

		/* Assign the weight to the weights matrix. */
		for (j = 0; j < cNetPopulation.numOutputNodes; j++)
			initialError[j] = out[j + cNetPopulation.numInputNodes + cNodulePopulation.nodules[nodule]->nodes] -
				outputData[pattern][j];

		for (i = 0; i < cNodulePopulation.nodules[nodule]->nodes; i++)
			for (j = 0; j < cNetPopulation.numInputNodes; j++)
				weights[i + cNetPopulation.numInputNodes][j] =
					cNodulePopulation.nodules[nodule]->inWeights[j][i];

		for (i = 0; i < cNetPopulation.numOutputNodes; i++)
			for (j = 0; j < cNodulePopulation.nodules[nodule]->nodes; j++)
				weights[i + cNetPopulation.numInputNodes + cNodulePopulation.nodules[nodule]->nodes][j + cNetPopulation.numInputNodes] =
					cNodulePopulation.nodules[nodule]->outWeights[j][i];

		/* Obtain the weight change. */
		changeWeight(initialError, weights, out, F_W,
					 cNodulePopulation.nodules[nodule]->nodes, transf);

		/* Update the weigths. */
		for (i = 0; i < cNodulePopulation.nodules[nodule]->nodes; i++)
			for (j = 0; j < cNetPopulation.numInputNodes; j++)
				cNodulePopulation.nodules[nodule]->inWeights[j][i] -= alpharet *
					F_W[i + cNetPopulation.numInputNodes][j];

		for (i = 0; i < cNetPopulation.numOutputNodes; i++)
			for (j = 0; j < cNodulePopulation.nodules[nodule]->nodes; j++)
				cNodulePopulation.nodules[nodule]->outWeights[j][i] -= alpharet *
					F_W[i + cNetPopulation.numInputNodes + cNodulePopulation.nodules[nodule]->nodes][j + cNetPopulation.numInputNodes];
	}

	/* Free memory. */
	for (i = 0; i < numNodes; i++) {
		free(weights[i]);
		free(F_W[i]);
	}

	free(weights);
	free(F_W);
	free(initialError);
	free(out);
	free(transf);
}

/*********************************************************************************
 * Calculate the differential value to apply to each weight to reduce the error  *
 * of a nodule.                                                                  *
 * @param double *initialError: Store the initial error of the nodule.           *
 * @param double **weights: Store all the connection weights of the nodule.      *
 * @param double *out: Store the output of each node of the nodule.              *
 * @param double **F_W: Store the change to apply in each nodule weight.         *
 * @param int nodes: Number of nodes at the nodule.                              *
 * @param func *transf: Store the transfer functions of each node of the nodule. *
 * @return void                                                                  *
 ********************************************************************************/
void
changeWeight(double *initialError, double **weights, double *out, double **F_W,
			 int nodes, func *transf) {
	int i, j, numNodes; /* Number of input and hidden nodes of the nodule. */
	double *nodeError; /* Output error of a nodule. */
	double *netError; /* Network error in a given node. */

	/* Variable initialization. */
	numNodes = cNetPopulation.numInputNodes + nodes;
	nodeError = (double *)xmalloc((numNodes + cNetPopulation.numOutputNodes) *
								  sizeof(double));
	netError = (double *)xmalloc((numNodes + cNetPopulation.numOutputNodes) *
								 sizeof(double));
	for (i = 0; i < numNodes; i++)
		nodeError[i] = 0.0;

	for (i = 0; i < cNetPopulation.numOutputNodes; i++)
		nodeError[numNodes + i] = initialError[i];

	/* Get the updates. */
	for (i = numNodes + cNetPopulation.numOutputNodes - 1;
		 i >= cNetPopulation.numInputNodes; i--) {
		for (j = i + 1; j < numNodes + cNetPopulation.numOutputNodes; j++)
			nodeError[i] += weights[j][i] * netError[j];

		if (transf[i - cNetPopulation.numInputNodes] == (func)&Logistic)
			netError[i] = nodeError[i] * pTransfer.logistic_b * out[i] *
				(1.0 - out[i] / pTransfer.logistic_a);
		else if (transf[i - cNetPopulation.numInputNodes] == (func)&HyperbolicTangent)
			netError[i] = nodeError[i] * (pTransfer.htan_b / pTransfer.htan_a) *
				(pTransfer.htan_a - out[i]) * (pTransfer.htan_a + out[i]);

		for (j = 0; j < i; j++)
			F_W[i][j] = netError[i] * out[j];
	}
}

/******************************************************************************
 * Run the simulated annealing over a given nodule.                           *
 * @param int nodule: Number of nodule to run over.                           *
 * @return void                                                               *
 *****************************************************************************/
void
simulatedAnnealing(int nodule) {
	int i, j, k, steps; /* Number of iterations of the annealing. */
	double T; /* Temperature of the simulated annealing. */
	double *weights; /* Old weights. */
	double oldAptitude; /* Old nodule aptitude. */

	/* Variable initialization. */
	T = ToSA;
	weights = NULL;
	oldAptitude = cNodulePopulation.nodules[nodule]->aptitude;

	/* Simulated annealing. */
	for (steps = 0; steps < numSAIterantions; steps++) {
		/* Keep the old weights. */
		k = 0;

		/* Input weights. */
		for (i = 0; i < cNetPopulation.numInputNodes; i++)
			for (j = 0; j < cNodulePopulation.nodules[nodule]->nodes; j++)
				if (cNodulePopulation.nodules[nodule]->inConn[i][j]) {
					weights = (double *)realloc(weights, (k + 1) * sizeof(double));
					weights[k] = cNodulePopulation.nodules[nodule]->inWeights[i][j];
					k++;
				}

		/* Output weights. */
		for (i = 0; i < cNodulePopulation.nodules[nodule]->nodes; i++)
			for (j = 0; j < cNetPopulation.numOutputNodes; j++)
				if (cNodulePopulation.nodules[nodule]->outConn[i][j]) {
					weights = (double *)realloc(weights, (k + 1) * sizeof(double));
					weights[k] = cNodulePopulation.nodules[nodule]->outWeights[i][j];
					k++;
				}

		/* Make a randon step. */
		randomStep(nodule);

		/* Calculate the aptitude of the new nodule. */
		measureNoduleChange(nodule);

		/* If the aptitude is worst we reject the change. */
		if ((oldAptitude > cNodulePopulation.nodules[nodule]->aptitude) &&
			doubleRandom() < (oldAptitude - cNodulePopulation.nodules[nodule]->aptitude) * T) {
			/* Restore the old weights. */
			k = 0;

			/* Input weights. */
			for (i = 0; i < cNetPopulation.numInputNodes; i++)
				for (j = 0; j < cNodulePopulation.nodules[nodule]->nodes; j++)
					if (cNodulePopulation.nodules[nodule]->inConn[i][j]) {
						cNodulePopulation.nodules[nodule]->inWeights[i][j] = weights[k];
						k++;
					}

			/* Output weights. */
			for (i = 0; i < cNodulePopulation.nodules[nodule]->nodes; i++)
				for (j = 0; j < cNetPopulation.numOutputNodes; j++)
					if (cNodulePopulation.nodules[nodule]->outConn[i][j]) {
						cNodulePopulation.nodules[nodule]->outWeights[i][j] = weights[k];
						k++;
					}

			/* Recalculate the aptitude of the nodule. */
			measureNoduleChange(nodule);
		} else {
			oldAptitude = cNodulePopulation.nodules[nodule]->aptitude;
		}

		/* Update the aptitude temperature. */
		T = alphasa * T;
	}

	free(weights);
}

/******************************************************************************
 * Make a randon step in each nodule weight.                                  *
 * @param int nodule: Number of the nodule to work with.                      *
 * @return void                                                               *
 *****************************************************************************/
void
randomStep(int nodule) {
	int i, j;

	/* Randon step in input connection weights. */
	for (i = 0; i < cNetPopulation.numInputNodes; i++)
		for (j = 0; j < cNodulePopulation.nodules[nodule]->nodes; j++)
			if (cNodulePopulation.nodules[nodule]->inConn[i][j])
				cNodulePopulation.nodules[nodule]->inWeights[i][j] +=
					Normal(0.0, 1.0);

	/* Random step in output connection weights.  */
	for (i = 0; i < cNodulePopulation.nodules[nodule]->nodes; i++)
		for (j = 0; j < cNetPopulation.numOutputNodes; j++)
			if (cNodulePopulation.nodules[nodule]->outConn[i][j])
				cNodulePopulation.nodules[nodule]->outWeights[i][j] +=
					Normal(0.0, 1.0);
}
