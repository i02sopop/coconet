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
 File: crearPoblaciones.c
 Function: anadirNoduloRedes()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Add a new nodule subpopulation to the network population.
 Input Parameters: None
 Local Variables: None
 Return Value: None
 Calling Functions:
   crearRedes(): Create a new network population from the previous nodes
                 population.
   crearDescendencia(): Create a new network population from the nodule
                        subpopulation and the previous network population.
   error(): Function to show an error message depending on an error number.
*****************************************************************************/

void anadirNodulosRedes()
{
	/* If it's the first nodes population we create a network per node. */
	if(pnodulos.n_subpobl == 1) {
		predes.n_redes = num_nodulos;
		if((predes.redes = (red **)malloc(num_nodulos * sizeof(red))) == NULL)
			error(RES_MEM);
		crearRedes();
	} else {
		/* We create the new population adding a new node to each network. */
		crearDescendencia();
	}
}

/******************************************************************************
 File: crearPoblaciones.c
 Function: crearDescendencia()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Create a new network population from a new nodule population
			  and the previous network population.
 Input parameters: None
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   netsel: Integer. Selection of a network of the present network.
   nodsel: Integer. Selection of a nodule from the new subpopulation.
   dnet: Array of networks. New descendant network population.
 Return Value: None
 Calling Functions:
   liberarRed(): Function to free a given network.
   error(): Function to show an error message depending on an error number.
*****************************************************************************/

void crearDescendencia()
{
	int i, j, netsel, nodsel;
	red **dnet; /* New network population. Descend of the actual population. */

	/* Creation of the new network. */
	if((dnet = (red **)malloc(max_redes * sizeof(red))) == NULL)
		error(RES_MEM);

	for(i = 0; i < max_redes; i++) {
		if((dnet[i] = (red *)malloc(sizeof(red))) == NULL)
			error(RES_MEM);

		dnet[i]->aptitud = 0;
		if((dnet[i]->valores_salida = (double *)malloc(predes.n_nodos_salida * sizeof(double))) == NULL)
			error(RES_MEM);

		if((dnet[i]->nodulos = (nodulo **)malloc(pnodulos.n_subpobl * sizeof(nodulo))) == NULL)
			error(RES_MEM);

		/* Select a new network randomly. */
		netsel = random() % predes.n_redes;

		/* Random selection of the node to add. */
		nodsel = random() % num_nodulos;

		/* Creation of the new network. */
		for(j = 0; j < pnodulos.n_subpobl - 1; j++)
			dnet[i]->nodulos[j] = predes.redes[netsel]->nodulos[j];

		dnet[i]->nodulos[pnodulos.n_subpobl - 1] = pnodulos.nodulos[(pnodulos.n_subpobl - 1) * num_nodulos + nodsel];
	}

	/* We fix the new network. */
	for(i = 0; i < predes.n_redes; i++)
		liberarRed(predes.redes[i]);

	free(predes.redes);
	predes.redes = dnet;
	dnet = NULL;
}

/******************************************************************************
 File: crearPoblaciones.c
 Function: liberarRed()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Function to free a given network.
 Input Parameters:
   network: Red. Network to delete.
 Local Variables:
   i: Integer. Counter.
 Return Value: None
 Calling Functions: None
*****************************************************************************/

void liberarRed(red *network)
{
	int i;

	/* We delete all the network. */
	for(i = 0; i < pnodulos.n_subpobl; i++)
		network->nodulos[i] = NULL;

	free(network->nodulos);
	free(network->valores_salida);
	free(network);
}

/******************************************************************************
 File: crearPoblaciones.c
 Function: crearNodulos()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Create a new nodule subpopulation.
 Input OParameters: None
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   k: Integer. Counter.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
 *****************************************************************************/

void crearNodulos()
{
	int i, j, k;

	/* We create the nodule subpopulation. */
	for(i = pnodulos.n_nodulos - num_nodulos; i < pnodulos.n_nodulos; i++) {
		if((pnodulos.nodulos[i] = (nodulo *)malloc(sizeof(nodulo))) == NULL)
			error(RES_MEM);

		/* Nodule id. */
		pnodulos.nodulos[i]->id = i;
		/* Number of hidden nodes of the nodule. */
		pnodulos.nodulos[i]->n_nodos = random() % max_nodos;
		/* Initial aptitude. */
		pnodulos.nodulos[i]->aptitud = 0;

		if(pnodulos.nodulos[i]->n_nodos == 0)
			pnodulos.nodulos[i]->n_nodos++;

		/* Memory allocation for the creation of connections and weights. */
		/* Connections from input to hidden nodes. */
		if((pnodulos.nodulos[i]->conexiones_entrada = (int **)malloc(predes.n_nodos_entrada * sizeof(int))) == NULL)
			error(RES_MEM);

		/* Connections from hidden to output nodes. */
		if((pnodulos.nodulos[i]->conexiones_salida = (int **)malloc(max_nodos * sizeof(int))) == NULL)
			error(RES_MEM);

		/* Weights of connections between input and hidden nodes. */
		if((pnodulos.nodulos[i]->pesos_entrada = (double **)malloc(predes.n_nodos_entrada * sizeof(double))) == NULL)
			error(RES_MEM);

		/* Weights of connections between hidden and output nodes. */
		if((pnodulos.nodulos[i]->pesos_salida = (double **)malloc(max_nodos * sizeof(double))) == NULL)
			error(RES_MEM);

		if((pnodulos.nodulos[i]->salidas_parciales = (double **)malloc(n_entrenamiento * sizeof(double))) == NULL)
			error(RES_MEM);

		for(j = 0; j < n_entrenamiento; j++) {
			if((pnodulos.nodulos[i]->salidas_parciales[j] = (double *)malloc(predes.n_nodos_salida * sizeof(double))) == NULL)
				error(RES_MEM);

			for(k = 0; k < predes.n_nodos_salida; k++)
				pnodulos.nodulos[i]->salidas_parciales[j][k] = 0.0;
		}

		/* Creation of connections and weigths. */
		/* Entry connections and weights. */
		for(j = 0; j < predes.n_nodos_entrada; j++) {
			if((pnodulos.nodulos[i]->conexiones_entrada[j] = (int *)malloc(max_nodos * sizeof(int))) == NULL)
				error(RES_MEM);

			if((pnodulos.nodulos[i]->pesos_entrada[j] = (double *)malloc(max_nodos * sizeof(double))) == NULL)
				error(RES_MEM);

			for(k = 0; k < max_nodos; k++) {
				if(random() % 2 == 1 && k < pnodulos.nodulos[i]->n_nodos) {
					pnodulos.nodulos[i]->conexiones_entrada[j][k] = 1;
					pnodulos.nodulos[i]->pesos_entrada[j][k] = aleatorio() / 2;
				} else {
					pnodulos.nodulos[i]->conexiones_entrada[j][k] = 0;
					pnodulos.nodulos[i]->pesos_entrada[j][k] = 0.0;
				}
			}
		}

		/* Output connections and weights. */
		for(j = 0; j < max_nodos; j++) {
			if((pnodulos.nodulos[i]->conexiones_salida[j] = (int *)malloc(predes.n_nodos_salida * sizeof(int))) == NULL)
				error(RES_MEM);

			if((pnodulos.nodulos[i]->pesos_salida[j] = (double *)malloc(predes.n_nodos_salida * sizeof(double))) == NULL)
				error(RES_MEM);

			for(k = 0; k < predes.n_nodos_salida; k++) {
				if(random() % 2 == 1 && j < pnodulos.nodulos[i]->n_nodos) {
					pnodulos.nodulos[i]->conexiones_salida[j][k] = 1;
					pnodulos.nodulos[i]->pesos_salida[j][k] = aleatorio() / 2;
				} else {
					pnodulos.nodulos[i]->conexiones_salida[j][k] = 0;
					pnodulos.nodulos[i]->pesos_salida[j][k] = 0.0;
				}
			}
		}

		/* Assign the transfer function to each node. */
		if((pnodulos.nodulos[i]->transf = (func *)malloc(max_nodos * sizeof(func))) == NULL)
			error(RES_MEM);

		for(j = 0; j < pnodulos.nodulos[i]->n_nodos; j++) {
			if(random() % 2 == 0)
				pnodulos.nodulos[i]->transf[j] = (func)&HyperbolicTangent;
			else
				pnodulos.nodulos[i]->transf[j] = (func)&Logistic;
		}
	}
}

/******************************************************************************
 File: crearPoblaciones.c
 Function: crearPoblacionNodulos()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Create or expand the nodule population.
 Input Parameters: None
 Local Variables: None
 Return Value: None
 Callimg Functions:
   crearNodulos(): Function to create a new nodule subpopulation.
   error(): Function to show an error message depending on an error number.
******************************************************************************/

void crearPoblacionNodulos()
{
	/*
	  We update the number of subpopulations and nodules of the nodule
	  population.
	*/
	pnodulos.n_subpobl++;
	pnodulos.n_nodulos = pnodulos.n_subpobl*num_nodulos;

	/* Memory allocation for the new subpopulation. */
	if(pnodulos.n_subpobl == 1) {
		if((pnodulos.nodulos = (nodulo **)malloc(num_nodulos * sizeof(nodulo))) == NULL)
			error(RES_MEM);
	} else {
		if((pnodulos.nodulos = (nodulo **)realloc(pnodulos.nodulos, pnodulos.n_nodulos * sizeof(nodulo))) == NULL)
			error(RES_MEM);
	}

	/* We create the nodules of the new subpopulation. */
	crearNodulos();
}

/******************************************************************************
 File: crearPoblaciones.c
 Function: crearRedes()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Create a new network population.
 Input Parameters: None
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
 *****************************************************************************/

void crearRedes()
{
	int i, j;

	for(i = 0; i < num_nodulos; i++) {
		if((predes.redes[i] = (red *)malloc(sizeof(red))) == NULL)
			error(RES_MEM);

		/* Allocation of memory and initialization of variables. */
		predes.redes[i]->aptitud = 0.0;
		if((predes.redes[i]->nodulos = (nodulo **)malloc(sizeof(nodulo))) == NULL)
			error(RES_MEM);

		/* We assign to the net its first nodule. */
		predes.redes[i]->nodulos[0] = pnodulos.nodulos[i];
		if((predes.redes[i]->valores_salida = (double *)malloc(predes.n_nodos_salida * sizeof(double))) == NULL)
			error(RES_MEM);

		for(j = 0; j < predes.n_nodos_salida; j++)
			predes.redes[i]->valores_salida[j] = 0.0;
	}
}
