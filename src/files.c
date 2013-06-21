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
#include <time.h>

/******************************************************************************
 File: files.c
 Function: loadFile()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Load the input data from training and configuration files.
 Input Parameters:
   config: String. Name of the configuration file.
   training: String. Name of the training file.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   configFile: Pointer to the configuration file.
   trainingFile: Pointer to the training file.
 Return Value: None
 Calling Functions:
   error(): Function to print an error message.
******************************************************************************/

void loadFile(char *config, char *training) {
	int i, j;
	FILE *configFile, *trainingFile;

	cNodulePopulation.numSubpops = 0;

	/* We open the configuration and training files. */
	configFile = fopen(config, "r");
	trainingFile = fopen(training, "r");
	if(configFile == NULL || trainingFile == NULL)
		error(IO);

	if(fscanf(trainingFile, "$ %d\n", &numTrain) == EOF ||
	   fscanf(trainingFile, "$ %d\n", &cNetPopulation.numInputNodes) == EOF ||
	   fscanf(trainingFile,"$ %d\n",&cNetPopulation.numOutputNodes) == EOF)
		error(IO);

	/* Loading the training data. */
	inputData = (double **)malloc(numTrain * sizeof(double));
	outputData = (double **)malloc(numTrain * sizeof(double));
	if(inputData == NULL || outputData == NULL)
		error(RES_MEM);

	for(i = 0; i < numTrain; i++) {
		inputData[i] = (double *)malloc(cNetPopulation.numInputNodes * sizeof(double));
		outputData[i] = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
		if(inputData[i] == NULL || outputData[i] == NULL)
			error(RES_MEM);

		for(j = 0; j < cNetPopulation.numInputNodes; j++)
			if(fscanf(trainingFile, "%lf", &inputData[i][j]) == EOF)
				error(IO);

		for(j = 0; j < cNetPopulation.numOutputNodes; j++)
			if(fscanf(trainingFile, "%lf", &outputData[i][j]) == EOF)
				error(IO);
	}

	if(fscanf(configFile, "Networks: %d\n", &maxNetworks) == EOF ||
	   fscanf(configFile, "Nodes: %d\n", &maxNodes) == EOF ||
	   fscanf(configFile, "Nodules: %d\n", &numNodules) == EOF)
		error(IO);

	/* Parameters of the transfer functions. */
	if(fscanf(configFile, "Htan a: %lf\n", &pTransfer.htan_a) == EOF ||
	   fscanf(configFile, "Htan b: %lf\n", &pTransfer.htan_b) == EOF ||
	   fscanf(configFile, "Logistic a: %lf\n", &pTransfer.logistic_a) == EOF ||
	   fscanf(configFile, "Logistic b: %lf\n", &pTransfer.logistic_b) == EOF ||
	   fscanf(configFile, "Epsilon: %lf\n", &pTransfer.epsilon) == EOF)
		error(IO);

	/* Adjustment for nodule aptitude. */
	if(fscanf(configFile, "replace: %lf\n", &adj.sust) == EOF ||
	   fscanf(configFile, "differ: %lf\n", &adj.dif) == EOF ||
	   fscanf(configFile, "best: %lf\n", &adj.best) == EOF)
		error(IO);

	if(fscanf(configFile, "Networks to pick: %d\n", &selNets) == EOF ||
	   fscanf(configFile, "Nodules to pick: %d\n", &nodSel) == EOF)
		error(IO);

	if(fscanf(configFile, "Iterations SA: %d\n", &numSAIterantions) == EOF ||
	   fscanf(configFile, "To SA: %lf\n", &ToSA) == EOF ||
	   fscanf(configFile, "alpha SA: %lf\n", &alphasa) == EOF)
		error(IO);

	if(fscanf(configFile, "Min Delta: %d\n", &delta_min) == EOF ||
	   fscanf(configFile, "Max Delta: %d\n", &delta_max) == EOF)
		error(IO);

	if(fscanf(configFile, "Evolution limit: %lf\n", &evolim) == EOF ||
	   fscanf(configFile, "Alpha backpropagation: %lf\n", &alpharet) == EOF)
		error(IO);

	if(fclose(configFile) == EOF || fclose(trainingFile) == EOF)
		error(IO);

	srandom(time(NULL));
	netTransf = (random() % 2 == 0) ? (func)&HyperbolicTangent : (func)&Logistic;

	/* The max number of networks is as the max number of nodules per subpopulation. */
	if(maxNetworks < numNodules)
		maxNetworks = numNodules;

	if(nodSel > numNodules)
		nodSel = numNodules;
}

/******************************************************************************
 File: files.c
 Function: readGeneralization()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Read the input data to measure the generalization.
 Input Parameters:
   filename: String. Name of the generalization file.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   file: Pointer to file. File that contains the input data.
 Return Value: None
 Calling Functions:
   error(): Function to print an error message.
******************************************************************************/

void readGeneralization(char *filename) {
	int i, j;
	FILE *file;

	/* We open the generalization file. */
	if((file = fopen(filename, "r")) == NULL)
		error(IO);

	/* We load the generalization data. */
	if(fscanf(file, "$ %d\n", &numGeneral) == EOF ||
	   fscanf(file, "$ %d\n", &cNetPopulation.numInputNodes) == EOF ||
	   fscanf(file, "$ %d\n", &cNetPopulation.numOutputNodes) == EOF)
		error(IO);

	for(i = 0; i < numTrain; i++) {
		free(inputData[i]);
		free(outputData[i]);
	}
	free(inputData);
	free(outputData);

	inputData = (double **)malloc(numGeneral * sizeof(double));
	outputData = (double **)malloc(numGeneral * sizeof(double));
	if(inputData == NULL || outputData == NULL)
		error(RES_MEM);

	for(i = 0; i < numGeneral; i++) {
		inputData[i] = (double *)malloc(cNetPopulation.numInputNodes * sizeof(double));
		outputData[i] = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
		if(inputData[i] == NULL || outputData[i] == NULL)
			error(RES_MEM);

		for(j = 0; j < cNetPopulation.numInputNodes; j++)
			if(fscanf(file, "%lf", &inputData[i][j]) == EOF)
				error(IO);

		for(j = 0; j < cNetPopulation.numOutputNodes; j++)
			if(fscanf(file, "%lf", &outputData[i][j]) == EOF)
				error(IO);
	}

	/* We close the file. */
	if(fclose(file) == EOF)
		error(IO);
}

/******************************************************************************
 File: files.c
 Function: exportBestNetwork()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Export the network who fits best to the problem to an output
              file.
 Input Parameters:
   filename: String. Name of the output file.
 Local Variables:
   i: Integer. Counter
   j: Integer. Counter.
   k: Integer. Counter
   l: Integer. Counter.
   nodes: Integer. Store the number of nodes at the network.
   max: Float. Maximum generalization.
   idmax: Integer. Network id with maximum generalization.
   connections: Integer. Number of connections of the network.
   aptitud: Float. Store the aptitude to measure the generalization.
   out: Pointer to file. Output file
 Return Value: None
 Calling Functions:
   generateNetOutput(): Generate the network output from the nodules output.
   generateNoduleOutput(): Generate the nodule output from an input parameter.
   measureNetworkAptitude(): Measure the net aptitude from the generated outputs.
   error(): Function to print an error message.
******************************************************************************/

void exportBestNetwork(char *filename)
{
	int i, j, k, l, nodes, idmax, connections;
	double *aptitude, max;
	FILE *out;

	if((aptitude = (double *)malloc(5 * sizeof(double))) == NULL)
		error(RES_MEM);

	for(i = 0; i < cNodulePopulation.numNodules; i++) {
		for(j = 0; j < numTrain; j++)
			free(cNodulePopulation.nodules[i]->partialOutputs[j]);
		free(cNodulePopulation.nodules[i]->partialOutputs);

		cNodulePopulation.nodules[i]->partialOutputs = (double **)malloc(numGeneral * sizeof(double));
		if(cNodulePopulation.nodules[i]->partialOutputs == NULL)
			error(RES_MEM);

		for(j = 0; j < numGeneral; j++){
			cNodulePopulation.nodules[i]->partialOutputs[j] = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
			if(cNodulePopulation.nodules[i]->partialOutputs[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < cNetPopulation.numOutputNodes; k++)
				cNodulePopulation.nodules[i]->partialOutputs[j][k] = 0.0;
		}
	}

	/* We open the output file. */
	if((out = fopen(filename,"w")) == NULL)
		error(IO);

	/* We store the aptitude of the selected networks. */
	for(i = 0; i < 5; i++){
		aptitude[i] = cNetPopulation.nets[i]->aptitude;
		cNetPopulation.nets[i]->aptitude = 0.0;
	}

	/* We calculate the network generalization. */
	for(i = 0; i < numGeneral; i++) {
		/* We generate the nodule partial outputs from the generaliation data. */
		for(j = 0; j < cNodulePopulation.numNodules; j++)
			generateNoduleOutput(inputData[i], j, i, NULL);

		for(j = 0; j < 5; j++) {
			/* We generate the network outputs from the nodules outputs. */
			generateNetOutput(j, i);

			/* We measure the network aptitude from the generated outputs. */
			measureNetworkAptitude(outputData[i], j);
		}
	}

	/* We normalize the generalization aptitude. */
	for(i = 0; i < 5; i++)
		cNetPopulation.nets[i]->aptitude /= numGeneral;

	/*
	  We pick the network with best generalization from the 5 best by
	  aptitude. Other selection parameter is the number of nodes in the
	  network, the aptitude of the network and the number of connections of the
	  network.
	*/
	max = 0.0;
	idmax = 0;
	for(i = 0; i < 5; i++) {
		if(cNetPopulation.nets[i]->aptitude > max) {
			max = cNetPopulation.nets[i]->aptitude;
			idmax = i;
		} else if(cNetPopulation.nets[i]->aptitude == max) {
			if(aptitude[idmax] < aptitude[i]) {
				max = cNetPopulation.nets[i]->aptitude;
				idmax = i;
			} else if(aptitude[idmax] == aptitude[i]) {
				nodes = 0;
				for(j = 0; j < cNodulePopulation.numSubpops; j++)
					nodes += (cNetPopulation.nets[idmax]->nodules[j]->nodes -
							  cNetPopulation.nets[i]->nodules[j]->nodes);

				if(nodes > 0) {
					max = cNetPopulation.nets[i]->aptitude;
					idmax = i;
				} else if(nodes == 0) {
					connections = 0;
					for(l = 0; l < cNodulePopulation.numSubpops; l++) {
						for(j = 0; j < cNetPopulation.numInputNodes; j++)
							for(k = 0; k < cNetPopulation.nets[idmax]->nodules[l]->nodes; k++)
								if(cNetPopulation.nets[idmax]->nodules[l]->inConn[j][k] == 1)
									connections++;
						for(j = 0; j < cNetPopulation.nets[idmax]->nodules[l]->nodes; j++)
							for(k = 0; k < cNetPopulation.numOutputNodes; k++)
								if(cNetPopulation.nets[idmax]->nodules[l]->outConn[j][k] == 1)
									connections++;
						for(j = 0; j < cNetPopulation.numInputNodes; j++)
							for(k = 0; k < cNetPopulation.nets[i]->nodules[l]->nodes; k++)
								if(cNetPopulation.nets[i]->nodules[l]->inConn[j][k] == 1)
									connections--;
						for(j = 0; j < cNetPopulation.nets[i]->nodules[l]->nodes; j++)
							for(k = 0; k < cNetPopulation.numOutputNodes; k++)
								if(cNetPopulation.nets[i]->nodules[l]->outConn[j][k] == 1)
									connections--;
					}

					if(connections > 0) {
						max = cNetPopulation.nets[i]->aptitude;
						idmax = i;
					}
				}
			}
		}
	}

	/*
	  Write in an output file the aptitude, generalization and number of
	  input, hidden and output nodes of the best network and its number of
	  input and output connections.
	*/
	if(fprintf(out, _("PRINT_NET_APTITUDE"), aptitude[idmax]) == EOF ||
	   fprintf(out, _("PRINT_NET_GENERALIZATION"), cNetPopulation.nets[idmax]->aptitude) == EOF ||
	   fprintf(out, _("PRINT_NUM_NODULES"), cNodulePopulation.numSubpops) == EOF ||
	   fprintf(out, _("PRINT_NUM_INPUT_NODES"), cNetPopulation.numInputNodes) == EOF)
		error(IO);

	nodes = 0;
	for(i = 0; i < cNodulePopulation.numSubpops; i++)
		nodes += cNetPopulation.nets[idmax]->nodules[i]->nodes;

	if(fprintf(out, _("PRINT_NUM_HIDDEN_NODES"), nodes) == EOF ||
	   fprintf(out, _("PRINT_NUM_OUTPUT_NODES"), cNetPopulation.numOutputNodes) == EOF)
		error(IO);

	connections = 0;
	for(i = 0; i < cNodulePopulation.numSubpops; i++)
		for(j = 0; j < cNetPopulation.numInputNodes; j++)
			for(k = 0; k < cNetPopulation.nets[idmax]->nodules[i]->nodes; k++)
				if(cNetPopulation.nets[idmax]->nodules[i]->inConn[j][k] == 1)
					connections++;

	if(fprintf(out, _("PRINT_NUM_INPUT_CONNECTIONS"), connections) == EOF)
		error(IO);

	connections = 0;
	for(i = 0; i < cNodulePopulation.numSubpops; i++)
		for(j = 0; j < cNetPopulation.nets[idmax]->nodules[i]->nodes; j++)
			for(k = 0; k < cNetPopulation.numOutputNodes; k++)
				if(cNetPopulation.nets[idmax]->nodules[i]->outConn[j][k] == 1)
					connections++;

	if(fprintf(out, _("PRINT_NUM_OUTPUT_CONNECTIONS"), connections) == EOF)
		error(IO);

	/* We close the output file. */
	if(fclose(out) == EOF)
		error(IO);
	free(aptitude);
}
