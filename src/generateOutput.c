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
 File: generateOutput.c
 Function: generateNoduleOutput()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Generate the nodule output from an input data.
 Input Parameters:
   in: Array of floats. Input data of each input node.
   numNodule: Integer. Number of nodule to work with.
   numInput: Integer. Number of input data to generate an output data.
   nodOut: Array of floats. Store the partial outputs of each node.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   numNodes: Number of nodes of the nodule.
   outputs: Array of floats. Store the partial outputs of each node.
   inputs: Array of floats. Store the input of each node.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
******************************************************************************/

void generateNoduleOutput(double *in, int numNodule, int numInput, double *nodOut)
{
	int i, j, numNodes;
	double *outputs, *inputs;
 
	/* Variable initialization. */
	numNodes = cNodulePopulation.nodules[numNodule]->nodes;
	outputs = (double *)malloc(numNodes * sizeof(double));
	inputs = (double *)malloc(numNodes * sizeof(double));

	if(outputs == NULL || inputs == NULL)
		error(RES_MEM);

	for(i = 0; i < numNodes; i++) {
		outputs[i] = 0.0;
		inputs[i] = 0.0;
	}

	for(i = 0; i < cNetPopulation.numOutputNodes; i++)
		cNodulePopulation.nodules[numNodule]->partialOutputs[numInput][i] = 0.0;

	/* We propagate the input data over the first layer. */
	for(i = 0; i < cNetPopulation.numInputNodes; i++)
		for(j = 0; j < numNodes; j++)
			if(cNodulePopulation.nodules[numNodule]->inConn[i][j] == 1)
				inputs[j] += (in[i] * cNodulePopulation.nodules[numNodule]->inWeights[i][j]);

	/* We generate the output of the hidden nodes. */
	for(i = 0; i < numNodes; i++)
		if(inputs[i] != 0.0)
			outputs[i] = (*(cNodulePopulation.nodules[numNodule]->transf[i]))(inputs[i]);

	/* We propagate the hidden nodes output to the output layer. */
	for(i = 0; i < numNodes; i++)
		if(outputs[i] != 0)
			for(j = 0; j < cNetPopulation.numOutputNodes; j++)
				if(cNodulePopulation.nodules[numNodule]->outConn[i][j] == 1)
					cNodulePopulation.nodules[numNodule]->partialOutputs[numInput][j] +=
						(outputs[i] * cNodulePopulation.nodules[numNodule]->outWeights[i][j]);

	/* We store the nodule output in nodOut. */
	if(nodOut != NULL) {
		for(i = 0; i < cNetPopulation.numInputNodes; i++)
			nodOut[i] = in[i];
  
		for(i = 0; i < numNodes; i++)
			nodOut[i + cNetPopulation.numInputNodes] = outputs[i];
    
		for(i = 0; i < cNetPopulation.numOutputNodes; i++)
			nodOut[i + cNetPopulation.numInputNodes + numNodes] = (*(netTransf))(cNodulePopulation.nodules[numNodule]->partialOutputs[numInput][i]);
	}

	free(outputs);
	free(inputs);
}

/******************************************************************************
 File: generateOutput.c
 Function: generateNetOutput()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Generate a network output from its nodules outputs.
 Input Parameters:
 	netNumber: Integer. Number of network that is going to generate the output.
 	inputNumber: Integer. Number of input data with generated output.
 Local Variables:
 	i: Integer. Counter.
 	j: Integer. Counter.
 Return Value: None
 Calling Functions: None
*******************************************************************************/

void generateNetOutput(int netNumber, int inputNumber)
{
	int i, j;

	/* We generate the network output from the partial outputs and the transfer
	   function. */
	for(i = 0; i < cNetPopulation.numOutputNodes; i++) {
		cNetPopulation.nets[netNumber]->outValues[i] = 0.0;
		for(j = 0; j < cNodulePopulation.numSubpops;j ++)
			cNetPopulation.nets[netNumber]->outValues[i] +=
				(*(netTransf))(cNetPopulation.nets[netNumber]->nodules[j]->partialOutputs[inputNumber][i]);
	}
}
