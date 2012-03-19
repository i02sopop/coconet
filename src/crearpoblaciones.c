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
	if(nodulePopulation.n_subpobl == 1) {
		netPopulation.n_redes = numNodules;
		netPopulation.redes = (red **)malloc(numNodules * sizeof(red));
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
	dnet = (red **)malloc(maxNetworks * sizeof(red));
	if(dnet == NULL)
		error(RES_MEM);

	for(i = 0; i < maxNetworks; i++) {
		dnet[i] = (red *)malloc(sizeof(red));
		if(dnet[i] == NULL)
			error(RES_MEM);

		dnet[i]->aptitud = 0;
		dnet[i]->valores_salida = (double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
		dnet[i]->nodulos = (nodule **)malloc(nodulePopulation.n_subpobl * sizeof(nodule));
		if(dnet[i]->valores_salida == NULL || dnet[i]->nodulos == NULL)
			error(RES_MEM);

		/* Select a new network randomly. */
		netsel = random() % netPopulation.n_redes;

		/* Random selection of the node to add. */
		nodsel = random() % numNodules;

		/* Creation of the new network. */
		for(j = 0; j < nodulePopulation.n_subpobl - 1; j++)
			dnet[i]->nodulos[j] = netPopulation.redes[netsel]->nodulos[j];

		dnet[i]->nodulos[nodulePopulation.n_subpobl - 1] = nodulePopulation.nodulos[(nodulePopulation.n_subpobl - 1) * numNodules + nodsel];
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
	for(i = 0; i < nodulePopulation.n_subpobl; i++)
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
	for(i = nodulePopulation.n_nodulos - numNodules; i < nodulePopulation.n_nodulos; i++) {
		nodulePopulation.nodulos[i] = (nodule *)malloc(sizeof(nodule));
		if(nodulePopulation.nodulos[i] == NULL)
			error(RES_MEM);

		/* Nodule id. */
		nodulePopulation.nodulos[i]->id = i;

		/* Number of hidden nodes of the nodule. */
		nodulePopulation.nodulos[i]->nodes = random() % maxNodes;

		/* Initial aptitude. */
		nodulePopulation.nodulos[i]->aptitude = 0;

		if(nodulePopulation.nodulos[i]->nodes == 0)
			nodulePopulation.nodulos[i]->nodes++;

		/* Memory allocation for the creation of connections and weights. */
		/* Connections from input to hidden nodes. */
		nodulePopulation.nodulos[i]->inConn = (int **)malloc(netPopulation.n_nodos_entrada * sizeof(int));

		/* Connections from hidden to output nodes. */
		nodulePopulation.nodulos[i]->outConn = (int **)malloc(maxNodes * sizeof(int));

		/* Weights of connections between input and hidden nodes. */
		nodulePopulation.nodulos[i]->inWeights = (double **)malloc(netPopulation.n_nodos_entrada * sizeof(double));

		/* Weights of connections between hidden and output nodes. */
		nodulePopulation.nodulos[i]->outWeights = (double **)malloc(maxNodes * sizeof(double));
		nodulePopulation.nodulos[i]->partialOutputs = (double **)malloc(n_entrenamiento * sizeof(double));
		nodulePopulation.nodulos[i]->transf = (func *)malloc(maxNodes * sizeof(func));

		if(nodulePopulation.nodulos[i]->inConn == NULL || nodulePopulation.nodulos[i]->outConn == NULL ||
		   nodulePopulation.nodulos[i]->inWeights == NULL || nodulePopulation.nodulos[i]->outWeights == NULL ||
		   nodulePopulation.nodulos[i]->partialOutputs == NULL || nodulePopulation.nodulos[i]->transf == NULL)
			error(RES_MEM);

		/* Creation of connections and weigths. */
		/* Entry connections and weights. */
		for(j = 0; j < netPopulation.n_nodos_entrada; j++) {
			nodulePopulation.nodulos[i]->inConn[j] = (int *)malloc(maxNodes * sizeof(int));
			nodulePopulation.nodulos[i]->inWeights[j] = (double *)malloc(maxNodes * sizeof(double));
			if(nodulePopulation.nodulos[i]->inConn[j] == NULL || nodulePopulation.nodulos[i]->inWeights[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < maxNodes; k++) {
				if(random() % 2 == 1 && k < nodulePopulation.nodulos[i]->nodes) {
					nodulePopulation.nodulos[i]->inConn[j][k] = 1;
					nodulePopulation.nodulos[i]->inWeights[j][k] = doubleRandom() / 2;
				} else {
					nodulePopulation.nodulos[i]->inConn[j][k] = 0;
					nodulePopulation.nodulos[i]->inWeights[j][k] = 0.0;
				}
			}
		}

		/* Output connections and weights. */
		for(j = 0; j < maxNodes; j++) {
			nodulePopulation.nodulos[i]->outConn[j] = (int *)malloc(netPopulation.n_nodos_salida * sizeof(int));
			nodulePopulation.nodulos[i]->outWeights[j] = (double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
			if(nodulePopulation.nodulos[i]->outConn[j] == NULL || nodulePopulation.nodulos[i]->outWeights[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < netPopulation.n_nodos_salida; k++) {
				if(random() % 2 == 1 && j < nodulePopulation.nodulos[i]->nodes) {
					nodulePopulation.nodulos[i]->outConn[j][k] = 1;
					nodulePopulation.nodulos[i]->outWeights[j][k] = doubleRandom() / 2;
				} else {
					nodulePopulation.nodulos[i]->outConn[j][k] = 0;
					nodulePopulation.nodulos[i]->outWeights[j][k] = 0.0;
				}
			}
		}

		/* Partial outputs. */
		for(j = 0; j < n_entrenamiento; j++) {
			nodulePopulation.nodulos[i]->partialOutputs[j] = (double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
			if(nodulePopulation.nodulos[i]->partialOutputs[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < netPopulation.n_nodos_salida; k++)
				nodulePopulation.nodulos[i]->partialOutputs[j][k] = 0.0;
		}

		/* Assign the transfer function to each node. */
		for(j = 0; j < nodulePopulation.nodulos[i]->nodes; j++) {
			if(random() % 2 == 0)
				nodulePopulation.nodulos[i]->transf[j] = (func)&HyperbolicTangent;
			else
				nodulePopulation.nodulos[i]->transf[j] = (func)&Logistic;
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
	nodulePopulation.n_subpobl++;
	nodulePopulation.n_nodulos = nodulePopulation.n_subpobl * numNodules;

	/* Memory allocation for the new subpopulation. */
	nodulePopulation.nodulos = (nodulePopulation.n_subpobl == 1) ? (nodule **)malloc(numNodules * sizeof(nodule)) : (nodule **)realloc(nodulePopulation.nodulos, nodulePopulation.n_nodulos * sizeof(nodule));
	if(nodulePopulation.nodulos == NULL)
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

	for(i = 0; i < numNodules; i++) {
		if((netPopulation.redes[i] = (red *)malloc(sizeof(red))) == NULL)
			error(RES_MEM);

		/* Allocation of memory. */
		netPopulation.redes[i]->nodulos = (nodule **)malloc(sizeof(nodule));
		netPopulation.redes[i]->valores_salida = (double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
		if(netPopulation.redes[i]->nodulos == NULL || netPopulation.redes[i]->valores_salida == NULL)
			error(RES_MEM);

		/* Initialization of variables. */
		netPopulation.redes[i]->aptitud = 0.0;
		netPopulation.redes[i]->nodulos[0] = nodulePopulation.nodulos[i];
		for(j = 0; j < netPopulation.n_nodos_salida; j++)
			netPopulation.redes[i]->valores_salida[j] = 0.0;
	}
}
