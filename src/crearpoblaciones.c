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
		netPopulation.n_redes = num_nodulos;
		netPopulation.redes = (red **)malloc(num_nodulos * sizeof(red));
		if(netPopulation.redes == NULL)
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
	dnet = (red **)malloc(max_redes * sizeof(red));
	if(dnet == NULL)
		error(RES_MEM);

	for(i = 0; i < max_redes; i++) {
		dnet[i] = (red *)malloc(sizeof(red));
		if(dnet[i] == NULL)
			error(RES_MEM);

		dnet[i]->aptitud = 0;
		dnet[i]->valores_salida = (double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
		dnet[i]->nodulos = (nodulo **)malloc(pnodulos.n_subpobl * sizeof(nodulo));
		if(dnet[i]->valores_salida == NULL || dnet[i]->nodulos == NULL)
			error(RES_MEM);

		/* Select a new network randomly. */
		netsel = random() % netPopulation.n_redes;

		/* Random selection of the node to add. */
		nodsel = random() % num_nodulos;

		/* Creation of the new network. */
		for(j = 0; j < pnodulos.n_subpobl - 1; j++)
			dnet[i]->nodulos[j] = netPopulation.redes[netsel]->nodulos[j];

		dnet[i]->nodulos[pnodulos.n_subpobl - 1] = pnodulos.nodulos[(pnodulos.n_subpobl - 1) * num_nodulos + nodsel];
	}

	/* We fix the new network. */
	for(i = 0; i < netPopulation.n_redes; i++)
		liberarRed(netPopulation.redes[i]);

	free(netPopulation.redes);
	netPopulation.redes = dnet;
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
   doubleRandom(): Returns a random float of double precision.
 *****************************************************************************/

void crearNodulos()
{
	int i, j, k;

	/* We create the nodule subpopulation. */
	for(i = pnodulos.n_nodulos - num_nodulos; i < pnodulos.n_nodulos; i++) {
		pnodulos.nodulos[i] = (nodulo *)malloc(sizeof(nodulo));
		if(pnodulos.nodulos[i] == NULL)
			error(RES_MEM);

		/* Nodule id. */
		pnodulos.nodulos[i]->id = i;

		/* Number of hidden nodes of the nodule. */
		pnodulos.nodulos[i]->nodes = random() % max_nodos;

		/* Initial aptitude. */
		pnodulos.nodulos[i]->aptitude = 0;

		if(pnodulos.nodulos[i]->nodes == 0)
			pnodulos.nodulos[i]->nodes++;

		/* Memory allocation for the creation of connections and weights. */
		/* Connections from input to hidden nodes. */
		pnodulos.nodulos[i]->inConn = (int **)malloc(netPopulation.n_nodos_entrada * sizeof(int));

		/* Connections from hidden to output nodes. */
		pnodulos.nodulos[i]->outConn = (int **)malloc(max_nodos * sizeof(int));

		/* Weights of connections between input and hidden nodes. */
		pnodulos.nodulos[i]->inWeights = (double **)malloc(netPopulation.n_nodos_entrada * sizeof(double));

		/* Weights of connections between hidden and output nodes. */
		pnodulos.nodulos[i]->outWeights = (double **)malloc(max_nodos * sizeof(double));
		pnodulos.nodulos[i]->partialOutputs = (double **)malloc(n_entrenamiento * sizeof(double));
		pnodulos.nodulos[i]->transf = (func *)malloc(max_nodos * sizeof(func));

		if(pnodulos.nodulos[i]->inConn == NULL || pnodulos.nodulos[i]->outConn == NULL ||
		   pnodulos.nodulos[i]->inWeights == NULL || pnodulos.nodulos[i]->outWeights == NULL ||
		   pnodulos.nodulos[i]->partialOutputs == NULL || pnodulos.nodulos[i]->transf == NULL)
			error(RES_MEM);

		/* Creation of connections and weigths. */
		/* Entry connections and weights. */
		for(j = 0; j < netPopulation.n_nodos_entrada; j++) {
			pnodulos.nodulos[i]->inConn[j] = (int *)malloc(max_nodos * sizeof(int));
			pnodulos.nodulos[i]->inWeights[j] = (double *)malloc(max_nodos * sizeof(double));
			if(pnodulos.nodulos[i]->inConn[j] == NULL || pnodulos.nodulos[i]->inWeights[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < max_nodos; k++) {
				if(random() % 2 == 1 && k < pnodulos.nodulos[i]->nodes) {
					pnodulos.nodulos[i]->inConn[j][k] = 1;
					pnodulos.nodulos[i]->inWeights[j][k] = doubleRandom() / 2;
				} else {
					pnodulos.nodulos[i]->inConn[j][k] = 0;
					pnodulos.nodulos[i]->inWeights[j][k] = 0.0;
				}
			}
		}

		/* Output connections and weights. */
		for(j = 0; j < max_nodos; j++) {
			pnodulos.nodulos[i]->outConn[j] = (int *)malloc(netPopulation.n_nodos_salida * sizeof(int));
			pnodulos.nodulos[i]->outWeights[j] = (double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
			if(pnodulos.nodulos[i]->outConn[j] == NULL || pnodulos.nodulos[i]->outWeights[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < netPopulation.n_nodos_salida; k++) {
				if(random() % 2 == 1 && j < pnodulos.nodulos[i]->nodes) {
					pnodulos.nodulos[i]->outConn[j][k] = 1;
					pnodulos.nodulos[i]->outWeights[j][k] = doubleRandom() / 2;
				} else {
					pnodulos.nodulos[i]->outConn[j][k] = 0;
					pnodulos.nodulos[i]->outWeights[j][k] = 0.0;
				}
			}
		}

		/* Partial outputs. */
		for(j = 0; j < n_entrenamiento; j++) {
			pnodulos.nodulos[i]->partialOutputs[j] = (double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
			if(pnodulos.nodulos[i]->partialOutputs[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < netPopulation.n_nodos_salida; k++)
				pnodulos.nodulos[i]->partialOutputs[j][k] = 0.0;
		}

		/* Assign the transfer function to each node. */
		for(j = 0; j < pnodulos.nodulos[i]->nodes; j++) {
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
	pnodulos.n_nodulos = pnodulos.n_subpobl * num_nodulos;

	/* Memory allocation for the new subpopulation. */
	pnodulos.nodulos = (pnodulos.n_subpobl == 1) ? (nodule **)malloc(num_nodulos * sizeof(nodulo)) : (nodule **)realloc(pnodulos.nodulos, pnodulos.n_nodulos * sizeof(nodulo));
	if(pnodulos.nodulos == NULL)
		error(RES_MEM);

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
		if((netPopulation.redes[i] = (red *)malloc(sizeof(red))) == NULL)
			error(RES_MEM);

		/* Allocation of memory. */
		netPopulation.redes[i]->nodulos = (nodule **)malloc(sizeof(nodulo));
		netPopulation.redes[i]->valores_salida = (double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
		if(netPopulation.redes[i]->nodulos == NULL || netPopulation.redes[i]->valores_salida == NULL)
			error(RES_MEM);

		/* Initialization of variables. */
		netPopulation.redes[i]->aptitud = 0.0;
		netPopulation.redes[i]->nodulos[0] = pnodulos.nodulos[i];
		for(j = 0; j < netPopulation.n_nodos_salida; j++)
			netPopulation.redes[i]->valores_salida[j] = 0.0;
	}
}
