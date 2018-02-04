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
#include <time.h>

/******************************************************************************
 * Load the input data from training and configuration files.                 *
 * @param char *config: Name of the configuration file.                       *
 * @param char *training: Name of the training file.                          *
 * @return void                                                               *
 *****************************************************************************/
void
loadFile(char *config, char *training) {
	int i, j;
	FILE *configFile, *trainingFile;

	cNodulePopulation.numSubpops = 0;

	/* We open the configuration and training files. */
	configFile = fopen(config, "r");
	trainingFile = fopen(training, "r");
	if (configFile == NULL || trainingFile == NULL)
		xerror(IO);

	if (fscanf(trainingFile, "$ %d\n", &numTrain) == EOF ||
		fscanf(trainingFile, "$ %d\n", &cNetPopulation.numInputNodes) == EOF ||
		fscanf(trainingFile,"$ %d\n",&cNetPopulation.numOutputNodes) == EOF)
		xerror(IO);

	/* Loading the training data. */
	inputData = (double **)xmalloc(numTrain * sizeof(double));
	outputData = (double **)xmalloc(numTrain * sizeof(double));
	for (i = 0; i < numTrain; i++) {
		inputData[i] = (double *)xmalloc(cNetPopulation.numInputNodes * sizeof(double));
		outputData[i] = (double *)xmalloc(cNetPopulation.numOutputNodes * sizeof(double));
		for (j = 0; j < cNetPopulation.numInputNodes; j++)
			if (fscanf(trainingFile, "%lf", &inputData[i][j]) == EOF)
				xerror(IO);

		for (j = 0; j < cNetPopulation.numOutputNodes; j++)
			if (fscanf(trainingFile, "%lf", &outputData[i][j]) == EOF)
				xerror(IO);
	}

	if (fscanf(configFile, "Networks: %d\n", &maxNetworks) == EOF ||
		fscanf(configFile, "Nodes: %d\n", &maxNodes) == EOF ||
		fscanf(configFile, "Nodules: %d\n", &numNodules) == EOF)
		xerror(IO);

	/* Parameters of the transfer functions. */
	if (fscanf(configFile, "Htan a: %lf\n", &pTransfer.htan_a) == EOF ||
		fscanf(configFile, "Htan b: %lf\n", &pTransfer.htan_b) == EOF ||
		fscanf(configFile, "Logistic a: %lf\n", &pTransfer.logistic_a) == EOF ||
		fscanf(configFile, "Logistic b: %lf\n", &pTransfer.logistic_b) == EOF ||
		fscanf(configFile, "Epsilon: %lf\n", &pTransfer.epsilon) == EOF)
		xerror(IO);

	/* Adjustment for nodule aptitude. */
	if (fscanf(configFile, "replace: %lf\n", &adj.sust) == EOF ||
		fscanf(configFile, "differ: %lf\n", &adj.dif) == EOF ||
		fscanf(configFile, "best: %lf\n", &adj.best) == EOF)
		xerror(IO);

	if (fscanf(configFile, "Networks to pick: %d\n", &selNets) == EOF ||
		fscanf(configFile, "Nodules to pick: %d\n", &nodSel) == EOF)
		xerror(IO);

	if (fscanf(configFile, "Iterations SA: %d\n", &numSAIterantions) == EOF ||
		fscanf(configFile, "To SA: %lf\n", &ToSA) == EOF ||
		fscanf(configFile, "alpha SA: %lf\n", &alphasa) == EOF)
		xerror(IO);

	if (fscanf(configFile, "Min Delta: %d\n", &delta_min) == EOF ||
		fscanf(configFile, "Max Delta: %d\n", &delta_max) == EOF)
		xerror(IO);

	if (fscanf(configFile, "Evolution limit: %lf\n", &evolim) == EOF ||
		fscanf(configFile, "Alpha backpropagation: %lf\n", &alpharet) == EOF)
		xerror(IO);

	if (fclose(configFile) == EOF || fclose(trainingFile) == EOF)
		xerror(IO);

	srandom(time(NULL));
	netTransf = (random() % 2 == 0) ? (func)&HyperbolicTangent : (func)&Logistic;

	/* The max number of networks is as the max number of nodules per subpopulation. */
	if (maxNetworks < numNodules)
		maxNetworks = numNodules;

	if (nodSel > numNodules)
		nodSel = numNodules;
}

/******************************************************************************
 * Read the input data to measure the generalization.                         *
 * @param char *filename: Name of the generalization file.                    *
 * @return void                                                               *
 *****************************************************************************/
void
readGeneralization(char *filename) {
	int i, j;
	FILE *file;

	/* Open the generalization file. */
	if ((file = fopen(filename, "r")) == NULL)
		xerror(IO);

	/* Load the generalization data. */
	if (fscanf(file, "$ %d\n", &numGeneral) == EOF ||
		fscanf(file, "$ %d\n", &cNetPopulation.numInputNodes) == EOF ||
		fscanf(file, "$ %d\n", &cNetPopulation.numOutputNodes) == EOF)
		xerror(IO);

	for (i = 0; i < numTrain; i++) {
		free(inputData[i]);
		free(outputData[i]);
	}

	free(inputData);
	free(outputData);

	inputData = (double **)xmalloc(numGeneral * sizeof(double));
	outputData = (double **)xmalloc(numGeneral * sizeof(double));
	for (i = 0; i < numGeneral; i++) {
		inputData[i] =
			(double *)xmalloc(cNetPopulation.numInputNodes * sizeof(double));
		outputData[i] =
			(double *)xmalloc(cNetPopulation.numOutputNodes * sizeof(double));
		for (j = 0; j < cNetPopulation.numInputNodes; j++)
			if (fscanf(file, "%lf", &inputData[i][j]) == EOF)
				xerror(IO);

		for (j = 0; j < cNetPopulation.numOutputNodes; j++)
			if(fscanf(file, "%lf", &outputData[i][j]) == EOF)
				xerror(IO);
	}

	/* Close the file. */
	if (fclose(file) == EOF)
		xerror(IO);
}

/******************************************************************************
 * Export the network who fits best to the problem to an output file.         *
 * @param char *filename: Name of the output file.                            *
 * @return void.                                                              *
 *****************************************************************************/
void
exportBestNetwork(char *filename) {
	int i, j, k, l, nodes, idmax, connections;
	double *aptitude, max;
	FILE *out;

	aptitude = (double *)xmalloc(5 * sizeof(double));
	for (i = 0; i < cNodulePopulation.numNodules; i++) {
		for (j = 0; j < numTrain; j++)
			free(cNodulePopulation.nodules[i]->partialOutputs[j]);
		free(cNodulePopulation.nodules[i]->partialOutputs);

		cNodulePopulation.nodules[i]->partialOutputs =
			(double **)xmalloc(numGeneral * sizeof(double));
		for (j = 0; j < numGeneral; j++){
			cNodulePopulation.nodules[i]->partialOutputs[j] =
				(double *)xmalloc(cNetPopulation.numOutputNodes * sizeof(double));
			for (k = 0; k < cNetPopulation.numOutputNodes; k++)
				cNodulePopulation.nodules[i]->partialOutputs[j][k] = 0.0;
		}
	}

	/* Open the output file. */
	if ((out = fopen(filename,"w")) == NULL)
		xerror(IO);

	/* Store the aptitude of the selected networks. */
	for (i = 0; i < 5; i++){
		aptitude[i] = cNetPopulation.nets[i]->aptitude;
		cNetPopulation.nets[i]->aptitude = 0.0;
	}

	/* Calculate the network generalization. */
	for (i = 0; i < numGeneral; i++) {
		/* Generate the nodule partial outputs from the generaliation data. */
		for (j = 0; j < cNodulePopulation.numNodules; j++)
			generateNoduleOutput(inputData[i], j, i, NULL);

		for (j = 0; j < 5; j++) {
			/* Generate the network outputs from the nodules outputs. */
			generateNetOutput(j, i);

			/* Measure the network aptitude from the generated outputs. */
			measureNetworkAptitude(outputData[i], j);
		}
	}

	/* Normalize the generalization aptitude. */
	for (i = 0; i < 5; i++)
		cNetPopulation.nets[i]->aptitude /= numGeneral;

	/* We pick the network with best generalization from the 5 best by aptitude.
	 * Other selection parameter is the number of nodes in the network, the
	 * aptitude of the network and the number of connections of the network. */
	max = 0.0;
	idmax = 0;
	for (i = 0; i < 5; i++) {
		if (cNetPopulation.nets[i]->aptitude > max) {
			max = cNetPopulation.nets[i]->aptitude;
			idmax = i;
		} else if (cNetPopulation.nets[i]->aptitude == max) {
			if (aptitude[idmax] < aptitude[i]) {
				max = cNetPopulation.nets[i]->aptitude;
				idmax = i;
			} else if (aptitude[idmax] == aptitude[i]) {
				nodes = 0;
				for (j = 0; j < cNodulePopulation.numSubpops; j++)
					nodes += (cNetPopulation.nets[idmax]->nodules[j]->nodes -
							  cNetPopulation.nets[i]->nodules[j]->nodes);

				if (nodes > 0) {
					max = cNetPopulation.nets[i]->aptitude;
					idmax = i;
				} else if (nodes == 0) {
					connections = 0;
					for (l = 0; l < cNodulePopulation.numSubpops; l++) {
						for (j = 0; j < cNetPopulation.numInputNodes; j++)
							for (k = 0; k < cNetPopulation.nets[idmax]->nodules[l]->nodes; k++)
								if (cNetPopulation.nets[idmax]->nodules[l]->inConn[j][k] == 1)
									connections++;
						for (j = 0; j < cNetPopulation.nets[idmax]->nodules[l]->nodes; j++)
							for (k = 0; k < cNetPopulation.numOutputNodes; k++)
								if (cNetPopulation.nets[idmax]->nodules[l]->outConn[j][k] == 1)
									connections++;
						for (j = 0; j < cNetPopulation.numInputNodes; j++)
							for (k = 0; k < cNetPopulation.nets[i]->nodules[l]->nodes; k++)
								if (cNetPopulation.nets[i]->nodules[l]->inConn[j][k] == 1)
									connections--;
						for (j = 0; j < cNetPopulation.nets[i]->nodules[l]->nodes; j++)
							for (k = 0; k < cNetPopulation.numOutputNodes; k++)
								if (cNetPopulation.nets[i]->nodules[l]->outConn[j][k] == 1)
									connections--;
					}

					if (connections > 0) {
						max = cNetPopulation.nets[i]->aptitude;
						idmax = i;
					}
				}
			}
		}
	}

	/* Write in an output file the aptitude, generalization and number of input,
	 * hidden and output nodes of the best network and its number of input and
	 * output connections. */
	if (fprintf(out, _("Aptitude: %lf\n"), aptitude[idmax]) == EOF ||
		fprintf(out, _("Generalization: %lf\n"), cNetPopulation.nets[idmax]->aptitude) == EOF ||
		fprintf(out, _("Number of Nodules: %d\n"), cNodulePopulation.numSubpops) == EOF ||
		fprintf(out, _("Number of Input Nodes: %d\n"), cNetPopulation.numInputNodes) == EOF)
		xerror(IO);

	nodes = 0;
	for (i = 0; i < cNodulePopulation.numSubpops; i++)
		nodes += cNetPopulation.nets[idmax]->nodules[i]->nodes;

	if (fprintf(out, _("Number of Hidden Nodes: %d\n"), nodes) == EOF ||
		fprintf(out, _("Number of Output Nodes: %d\n"), cNetPopulation.numOutputNodes) == EOF)
		xerror(IO);

	connections = 0;
	for (i = 0; i < cNodulePopulation.numSubpops; i++)
		for (j = 0; j < cNetPopulation.numInputNodes; j++)
			for (k = 0; k < cNetPopulation.nets[idmax]->nodules[i]->nodes; k++)
				if (cNetPopulation.nets[idmax]->nodules[i]->inConn[j][k] == 1)
					connections++;

	if (fprintf(out, _("Number of Input Connections: %d\n"), connections) == EOF)
		xerror(IO);

	connections = 0;
	for (i = 0; i < cNodulePopulation.numSubpops; i++)
		for (j = 0; j < cNetPopulation.nets[idmax]->nodules[i]->nodes; j++)
			for (k = 0; k < cNetPopulation.numOutputNodes; k++)
				if (cNetPopulation.nets[idmax]->nodules[i]->outConn[j][k] == 1)
					connections++;

	if (fprintf(out, _("Number of Output Connections: %d\n"), connections) == EOF)
		xerror(IO);

	/* Close the output file. */
	if (fclose(out) == EOF)
		xerror(IO);
	free(aptitude);
}
