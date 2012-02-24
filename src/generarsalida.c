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

#include <definiciones.h>

/******************************************************************************
 File: generarSalida.c
 Function: generarSalidaNodulo()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Generate the nodule output from an input data.
 Input Parameters:
   in: Array of floats. Input data of each input node.
   numNodulo: Integer. Number of nodule to work with.
   numEntrada: Integer. Number of input data to generate an output data.
   nod_out: Array of floats. Store the partial outputs of each node.
 Local Variables:
   i: Integer. Counter
   j: Integer. Counter
   salidas: Array of floats. Store the partial outputs of each node.
   activacion: Array of floats. Store the input of each node.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
******************************************************************************/

void generarSalidaNodulo(double *in, int numNodulo, int numEntrada, double *nod_out)
{
	int i, j, num_nodos;;
	double *salidas, *activacion;
 
	/* Variable initialization. */
	num_nodos = pnodulos.nodulos[numNodulo]->n_nodos;
	salidas = (double *)malloc(num_nodos * sizeof(double));
	activacion = (double *)malloc(num_nodos * sizeof(double));

	if(salidas == NULL || activacion == NULL)
		error(RES_MEM);

	for(i = 0; i < num_nodos; i++) {
		salidas[i] = 0.0;
		activacion[i] = 0.0;
	}

	for(i = 0; i < predes.n_nodos_salida; i++)
		pnodulos.nodulos[numNodulo]->salidas_parciales[numEntrada][i] = 0.0;


	/* We propagate the input data over the first layer. */
	for(i = 0; i < predes.n_nodos_entrada; i++)
		for(j = 0; j < num_nodos; j++)
			if(pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j] == 1)
				activacion[j] += 
					(in[i] * pnodulos.nodulos[numNodulo]->pesos_entrada[i][j]);

	/* We generate the output of the hidden nodes. */
	for(i = 0; i < num_nodos; i++)
		if(activacion[i] != 0.0)
			salidas[i] = (*(pnodulos.nodulos[numNodulo]->transf[i]))(activacion[i]);

	/* We propagate the hidden nodes output to the output layer. */
	for(i = 0; i < num_nodos; i++)
		if(salidas[i] != 0)
			for(j = 0; j < predes.n_nodos_salida; j++)
				if(pnodulos.nodulos[numNodulo]->conexiones_salida[i][j] == 1)
					pnodulos.nodulos[numNodulo]->salidas_parciales[numEntrada][j] +=
						(salidas[i] *
						 pnodulos.nodulos[numNodulo]->pesos_salida[i][j]);

	/* We store the nodule output in nod_out. */
	if(nod_out != NULL) {
		for(i = 0; i < predes.n_nodos_entrada; i++)
			nod_out[i] = in[i];
  
		for(i = 0; i < pnodulos.nodulos[numNodulo]->n_nodos; i++)
			nod_out[i + predes.n_nodos_entrada] = salidas[i];
    
		for(i = 0; i < predes.n_nodos_salida; i++)
			nod_out[i + predes.n_nodos_entrada + num_nodos] = (*(net_transf))(pnodulos.nodulos[numNodulo]->salidas_parciales[numEntrada][i]);
	}

	free(salidas);
	free(activacion);
}

/******************************************************************************
 File: generarSalida.c
 Function: generarSalidaRed()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Generate a network output from its nodules outputs.
 Input Parameters:
 	numRed: Integer. Number of network that is going to generate the output.
 	numEntrada: Integer. Number of input data with generated output.
 Local Variables:
 	i: Integer. Counter.
 	j: Integer. Counter.
 Return Value: None
 Calling Functions: None
*******************************************************************************/

void generarSalidaRed(int numRed, int numEntrada)
{
	int i, j;

	/* We generate the network output from the partial outputs and the transfer
	   function. */
	for(i = 0; i < predes.n_nodos_salida; i++) {
		predes.redes[numRed]->valores_salida[i] = 0.0;
		for(j = 0; j < pnodulos.n_subpobl;j ++)
			predes.redes[numRed]->valores_salida[i] +=
				(*(net_transf))(predes.redes[numRed]->nodulos[j]->salidas_parciales[numEntrada][i]);
	}
}
