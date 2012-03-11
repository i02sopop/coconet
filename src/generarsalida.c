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
 File: generarSalida.c
 Function: generarSalidaNodulo()
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

void generarSalidaNodulo(double *in, int numNodule, int numInput, double *nodOut)
{
	int i, j, numNodes;
	double *outputs, *inputs;
 
	/* Variable initialization. */
	numNodes = pnodulos.nodulos[numNodule]->n_nodos;
	outputs = (double *)malloc(numNodes * sizeof(double));
	inputs = (double *)malloc(numNodes * sizeof(double));

	if(outputs == NULL || inputs == NULL)
		error(RES_MEM);

	for(i = 0; i < numNodes; i++) {
		outputs[i] = 0.0;
		inputs[i] = 0.0;
	}

	for(i = 0; i < predes.n_nodos_salida; i++)
		pnodulos.nodulos[numNodule]->salidas_parciales[numInput][i] = 0.0;

	/* We propagate the input data over the first layer. */
	for(i = 0; i < predes.n_nodos_entrada; i++)
		for(j = 0; j < numNodes; j++)
			if(pnodulos.nodulos[numNodule]->conexiones_entrada[i][j] == 1)
				inputs[j] += (in[i] * pnodulos.nodulos[numNodule]->pesos_entrada[i][j]);

	/* We generate the output of the hidden nodes. */
	for(i = 0; i < numNodes; i++)
		if(inputs[i] != 0.0)
			outputs[i] = (*(pnodulos.nodulos[numNodule]->transf[i]))(inputs[i]);

	/* We propagate the hidden nodes output to the output layer. */
	for(i = 0; i < numNodes; i++)
		if(outputs[i] != 0)
			for(j = 0; j < predes.n_nodos_salida; j++)
				if(pnodulos.nodulos[numNodule]->conexiones_salida[i][j] == 1)
					pnodulos.nodulos[numNodule]->salidas_parciales[numInput][j] +=
						(outputs[i] * pnodulos.nodulos[numNodule]->pesos_salida[i][j]);

	/* We store the nodule output in nodOut. */
	if(nodOut != NULL) {
		for(i = 0; i < predes.n_nodos_entrada; i++)
			nodOut[i] = in[i];
  
		for(i = 0; i < pnodulos.nodulos[numNodule]->n_nodos; i++)
			nodOut[i + predes.n_nodos_entrada] = outputs[i];
    
		for(i = 0; i < predes.n_nodos_salida; i++)
			nodOut[i + predes.n_nodos_entrada + numNodes] = (*(net_transf))(pnodulos.nodulos[numNodule]->salidas_parciales[numInput][i]);
	}

	free(outputs);
	free(inputs);
}

/******************************************************************************
 File: generarSalida.c
 Function: generarSalidaRed()
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

void generarSalidaRed(int netNumber, int inputNumber)
{
	int i, j;

	/* We generate the network output from the partial outputs and the transfer
	   function. */
	for(i = 0; i < predes.n_nodos_salida; i++) {
		predes.redes[netNumber]->valores_salida[i] = 0.0;
		for(j = 0; j < pnodulos.n_subpobl;j ++)
			predes.redes[netNumber]->valores_salida[i] +=
				(*(net_transf))(predes.redes[netNumber]->nodulos[j]->salidas_parciales[inputNumber][i]);
	}
}
