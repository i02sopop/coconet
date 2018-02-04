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

/******************************************************************************
 * Generate the nodule output from an input data.                             *
 * @param double *in: Input data of each input node.                          *
 * @param int numNodule: Number of nodule to work with.                       *
 * @param int numInput: Number of input data to generate an output data.      *
 * @param double *nodOut: Store the partial outputs of each node.             *
 * @return void                                                               *
 *****************************************************************************/
void
generateNoduleOutput(double *in, int numNodule, int numInput, double *nodOut) {
	int i, j, numNodes;
	double *outputs, *inputs;

	/* Variable initialization. */
	numNodes = cNodulePopulation.nodules[numNodule]->nodes;
	outputs = (double *)xmalloc(numNodes * sizeof(double));
	inputs = (double *)xmalloc(numNodes * sizeof(double));
	for (i = 0; i < numNodes; i++)
		inputs[i] = outputs[i] = 0.0;

	for (i = 0; i < cNetPopulation.numOutputNodes; i++)
		cNodulePopulation.nodules[numNodule]->partialOutputs[numInput][i] = 0.0;

	/* Propagate the input data over the first layer. */
	for (i = 0; i < cNetPopulation.numInputNodes; i++)
		for (j = 0; j < numNodes; j++)
			if (cNodulePopulation.nodules[numNodule]->inConn[i][j] == 1)
				inputs[j] += (in[i] * cNodulePopulation.nodules[numNodule]->inWeights[i][j]);

	/* Generate the output of the hidden nodes. */
	for (i = 0; i < numNodes; i++)
		if (inputs[i] != 0.0)
			outputs[i] = (*(cNodulePopulation.nodules[numNodule]->transf[i]))(inputs[i]);

	/* Propagate the hidden nodes output to the output layer. */
	for (i = 0; i < numNodes; i++)
		if (outputs[i] != 0)
			for (j = 0; j < cNetPopulation.numOutputNodes; j++)
				if (cNodulePopulation.nodules[numNodule]->outConn[i][j] == 1)
					cNodulePopulation.nodules[numNodule]->partialOutputs[numInput][j] +=
						(outputs[i] * cNodulePopulation.nodules[numNodule]->outWeights[i][j]);

	/* Store the nodule output in nodOut. */
	if (nodOut != NULL) {
		for (i = 0; i < cNetPopulation.numInputNodes; i++)
			nodOut[i] = in[i];

		for (i = 0; i < numNodes; i++)
			nodOut[i + cNetPopulation.numInputNodes] = outputs[i];

		for (i = 0; i < cNetPopulation.numOutputNodes; i++)
			nodOut[i + cNetPopulation.numInputNodes + numNodes] =
				(*(netTransf))(cNodulePopulation.nodules[numNodule]->partialOutputs[numInput][i]);
	}

	free(outputs);
	free(inputs);
}

/*********************************************************************************
 * Generate a network output from its nodules outputs.                           *
 * @param int netNumber: Number of network that is going to generate the output. *
 * @param int inputNumber: Number of input data with generated output.           *
 * @return void                                                                  *
 ********************************************************************************/
void
generateNetOutput(int netNumber, int inputNumber) {
	int i, j;

	/* Generate the network output from the partial outputs and the transfer
	 * function. */
	for (i = 0; i < cNetPopulation.numOutputNodes; i++) {
		cNetPopulation.nets[netNumber]->outValues[i] = 0.0;
		for (j = 0; j < cNodulePopulation.numSubpops;j ++)
			cNetPopulation.nets[netNumber]->outValues[i] +=
				(*(netTransf))(cNetPopulation.nets[netNumber]->nodules[j]->partialOutputs[inputNumber][i]);
	}
}
