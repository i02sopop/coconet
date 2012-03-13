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
 File: mutacionEstructural.c
 Function: copiarDescendencia()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Copy the actual nodule population into a new descendant
              population.
 Input Parameters: None
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   k: Integer. Counter.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
******************************************************************************/

void copiarDescendencia()
{
	int i, j, k, noduleOrig, initialNodule;

	/* Initialization of variables. */
	initialNodule = num_nodulos * (pnodulos.n_subpobl - 1);
	descendencia = (nodulo **)malloc(num_nodulos * sizeof(nodulo));
	if(descendencia == NULL)
		error(RES_MEM);

	/* We copy the nodules to the new population. */
	for(i = 0; i < num_nodulos; i++) {
		descendencia[i] = (nodulo *)malloc(sizeof(nodulo));
		if(descendencia[i] == NULL)
			error(RES_MEM);

		/* We copy the nodule from the old to the new population. */
		noduleOrig = initialNodule + i;
		descendencia[i]->id = pnodulos.nodulos[noduleOrig]->id;
		descendencia[i]->n_nodos = pnodulos.nodulos[noduleOrig]->n_nodos;
		descendencia[i]->aptitud = pnodulos.nodulos[noduleOrig]->aptitud;

		descendencia[i]->conexiones_salida = (int **)malloc(max_nodos * sizeof(int));
		descendencia[i]->pesos_salida = (double **)malloc(max_nodos * sizeof(double));
		descendencia[i]->transf = (func *)malloc(max_nodos * sizeof(func));
		if(descendencia[i]->conexiones_salida == NULL ||
		   descendencia[i]->pesos_salida == NULL ||
		   descendencia[i]->transf == NULL)
			error(RES_MEM);

		for(j = 0; j < max_nodos; j++) {
			descendencia[i]->conexiones_salida[j] =
				(int *)malloc(netPopulation.n_nodos_salida * sizeof(int));
			descendencia[i]->pesos_salida[j] =
				(double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
			if(descendencia[i]->conexiones_salida[j] == NULL ||
				descendencia[i]->pesos_salida[j] == NULL)
				error(RES_MEM);

			descendencia[i]->transf[j] = pnodulos.nodulos[noduleOrig]->transf[j];

			for(k = 0; k < netPopulation.n_nodos_salida; k++) {
				descendencia[i]->conexiones_salida[j][k] =
					pnodulos.nodulos[noduleOrig]->conexiones_salida[j][k];
				descendencia[i]->pesos_salida[j][k] =
					pnodulos.nodulos[noduleOrig]->pesos_salida[j][k];
			} /* end for */
		} /* end for */

		descendencia[i]->pesos_entrada =
			(double **)malloc(netPopulation.n_nodos_entrada * sizeof(double));
		descendencia[i]->conexiones_entrada =
			(int **)malloc(netPopulation.n_nodos_entrada * sizeof(int));
		if(descendencia[i]->pesos_entrada == NULL ||
			descendencia[i]->conexiones_entrada == NULL)
			error(RES_MEM);

		for(j = 0; j < netPopulation.n_nodos_entrada; j++) {
			descendencia[i]->conexiones_entrada[j] =
				(int *)malloc(max_nodos * sizeof(int));
			descendencia[i]->pesos_entrada[j] =
				(double *)malloc(max_nodos * sizeof(double))
			if(descendencia[i]->conexiones_entrada[j] == NULL ||
				descendencia[i]->pesos_entrada[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < max_nodos; k++) {
				descendencia[i]->conexiones_entrada[j][k] =
					pnodulos.nodulos[noduleOrig]->conexiones_entrada[j][k];
				descendencia[i]->pesos_entrada[j][k] =
					pnodulos.nodulos[noduleOrig]->pesos_entrada[j][k];
			} /* end for */
		} /* end for */

		descendencia[i]->salidas_parciales =
			(double **)malloc(n_entrenamiento * sizeof(double));
		if(descendencia[i]->salidas_parciales == NULL)
			error(RES_MEM);

		for(j = 0; j < n_entrenamiento; j++) {
			descendencia[i]->salidas_parciales[j] =
				(double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
			if(descendencia[i]->salidas_parciales[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < netPopulation.n_nodos_salida; k++)
				descendencia[i]->salidas_parciales[j][k] =
					pnodulos.nodulos[noduleOrig]->salidas_parciales[j][k];
		} /* end for */
	} /* end for */
}

/******************************************************************************
 File: mutacionEstructural.c
 Function: mutarNodulos()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Make an estructural mutation in a given nodule.
 Input Parameters:
   nodule: Integer. Number of nodule to mutate.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   num: Integer. Number of mutation elements.
 Return Value: None
 Calling Functions: 
   anadirNodo(): Add a node to the nodule.
   borrarNodo(): Delete a node from the nodule.
   anadirConexion(): Add a connection to the nodule.
   borrarConexion(): Delete a connection from the nodule.
   medirCambioNodulo(): Calculate the nodules aptitude variation after the
                        mutation.
   doubleRandom(): Returns a random float of double precision.
******************************************************************************/

void mutarNodulos(int nodule)
{
	int i, j, num;

	/* We made the estructural mutation. */
	/* Deletion of nodes. */
	num = (int)(delta_min + doubleRandom() * (1 - pnodulos.nodulos[nodule]->aptitud)
				* (delta_min - delta_max));
	if(num < 0)
		num = 0;
	else if(pnodulos.nodulos[numNodulo]->n_nodos < num)
		num = pnodulos.nodulos[nodule]->n_nodos;
	else if(num > 0)
		borrarNodo(nodule, num);

	/* Add nodes. */
	num = (int)(delta_min + doubleRandom() * (1 - pnodulos.nodulos[nodule]->aptitud)
				* (delta_min - delta_max));
	if(num < 0)
		num = 0;
	else if((pnodulos.nodulos[numNodulo]->n_nodos + num) > max_nodos)
		num = max_nodos - pnodulos.nodulos[nodule]->n_nodos;
	else if(num > 0)
		anadirNodo(nodule, num); /* We add a new node,*/

	/* Delete connections. */
	num = (int)(delta_min + doubleRandom() * (1 - pnodulos.nodulos[nodule]->aptitud)
				* (delta_min - delta_max));
	if(num < 0)
		num = 0;

	for(i = 0; i <num && pnodulos.nodulos[nodule]->n_nodos > 0; i++) {
		j = random() % (netPopulation.n_nodos_entrada + netPopulation.n_nodos_salida);
		borrarConexion(nodule, j);
	}

	/* Add connections. */
	num = (int)(delta_min + doubleRandom() * (1 - pnodulos.nodulos[nodule]->aptitud)
				* (delta_min - delta_max));
	if(num < 0)
		num = 0;

	for(i = 0; i < num && pnodulos.nodulos[nodule]->n_nodos > 0; i++)
		anadirConexion(nodule);

	/*
	  We make the calculations over the modified nodule, both his partial
	  outputs and his aptitude.
	*/
	medirCambioNodulo(nodule);
}

/******************************************************************************
 File: mutacionEstructural.c
 Function: anadirConexion()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Add a connection to a given nodule.
 Input Parameters:
   nodule: Integer. Number of nodule to modify.
 Local Variables:
   i. Integer. Counter.
   j: Integer. Counter.
   sel: Integer. Type of connection to add (input, internal or outpunt).
   origin: Vector if integers. Possible origin nodes for the connection.
   destination: Vector of integers. Possible destination nodes for the connection.
   num: Integer. Counter.
   pos: Integer. Position to add the connection.
 Return Value: None
 Calling Functions
   error(): Function to show an error message depending on an error number.
   doubleRandom(): Returns a random float of double precision.
******************************************************************************/

void anadirConexion(int nodule)
{
	int i, j, sel, *origin, *destination, num, pos;

	num = 0;
	sel = random() % 2;
	origin = (int *)malloc(sizeof(int));
	destination = (int *)malloc(sizeof(int));
	if(origin == NULL || destination == NULL)
		error(RES_MEM);

	if(sel == 0) {
		/* We add an input connection. */
		for(i = 0; i < netPopulation.n_nodos_entrada; i++)
			for(j = 0; j < pnodulos.nodulos[nodule]->n_nodos; j++)
				if(pnodulos.nodulos[nodule]->conexiones_entrada[i][j] == 0) {
					origin = (int *)realloc(origin, (num + 1) * sizeof(int));
					destination = (int *)realloc(destination, (num + 1) * sizeof(int));
					origin[num] = i;
					destination[num] = j;
					num++;
				} /* end if */

		if(num != 0){
			pos = random() % num;
			pnodulos.nodulos[nodule]->conexiones_entrada[origin[pos]][destination[pos]] = 1;
			pnodulos.nodulos[nodule]->pesos_entrada[origin[pos]][destination[pos]] =
				doubleRandom() / 2;
			free(origin);
			free(destination);
		} /* end if */
	} /* end if */

	if((sel == 1 && num == 0) || num == 0) {
		/* We add an output connection. */
		for(i = 0; i < pnodulos.nodulos[nodule]->n_nodos; i++)
			for(j = 0; j < netPopulation.n_nodos_salida; j++)
				if(pnodulos.nodulos[nodule]->conexiones_salida[i][j] == 0) {
					origin = (int *)realloc(origin, (num + 1) * sizeof(int));
					destination = (int *)realloc(destination, (num + 1) * sizeof(int));
					origin[num] = i;
					destination[num] = j;
					num++;
				} /* end if */

		if(num != 0) {
			pos = random() % num;
			pnodulos.nodulos[nodule]->conexiones_salida[origin[pos]][destination[pos]] = 1;
			pnodulos.nodulos[nodule]->pesos_salida[origin[pos]][destination[pos]] =
				doubleRandom() / 2;
			free(origin);
			free(destination);
		} /* end if */
	} /* end if */

	if(num == 0) {
		free(origin);
		free(destination);
	} /* end if */
}

/********************************************************************************
 File: mutacionEstructural.c
 Function: anadirNodo()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Add some nodes to a nodule.
 Input Parameters:
   nodule: Integer. Number of nodule to work with.
   nodes: Integer. Number of nodes to add.
 Local Variables:
   i: Integer. Counter
 Return Value: None
 Calling Functions: None
*******************************************************************************/

void anadirNodo(int nodule, int nodes)
{
	int i;

	for(i = 0; i < nodes; i++) {
		/* We assign the transfer function to the node. */
		if((random() % 2) == 0)
			pnodulos.nodulos[nodule]->transf[pnodulos.nodulos[nodule]->n_nodos + i]
				= (func)&HyperbolicTangent;
		else
			pnodulos.nodulos[nodule]->transf[pnodulos.nodulos[nodule]->n_nodos + i]
				= (func)&Logistic;
	}

	/* We update the number of nodes. */
	pnodulos.nodulos[nodule]->n_nodos += nodes;
}

/*******************************************************************************
 File: mutacionEstructural.c
 Function: borrarConexion()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Delete a connection of a given node.
 Input Parameters:
   nodule: Integer. Number of nodule to work with.
   node: Integer. Node to delete the connection from.
 Local Variables:
   i: Integer. Counter
   sel: Integer. Connection to delete.
 Return Value: None
 Calling Functions: None
*******************************************************************************/

void borrarConexion(int nodule, int node)
{
	int i, sel;

	if(node < netPopulation.n_nodos_entrada){
		/* We delete an input connection. */
		for(i = 0; i < pnodulos.nodulos[nodule]->n_nodos &&
				pnodulos.nodulos[nodule]->conexiones_entrada[node][i] != 1; i++);
		if(i < pnodulos.nodulos[nodule]->n_nodos){
			sel = random() % pnodulos.nodulos[nodule]->n_nodos;
			for(; pnodulos.nodulos[nodule]->conexiones_entrada[node][sel] != 1;
				sel = random() % pnodulos.nodulos[nodule]->n_nodos);
			pnodulos.nodulos[nodule]->conexiones_entrada[node][sel] = 0;
			pnodulos.nodulos[nodule]->pesos_entrada[node][sel] = 0;
		} /* end if */
	} else {
		/* We delete an output connection. */
		node -= netPopulation.n_nodos_entrada;
		for(i = 0; i < pnodulos.nodulos[nodule]->n_nodos &&
				pnodulos.nodulos[nodule]->conexiones_salida[i][node] != 1; i++);
		if(i < pnodulos.nodulos[nodule]->n_nodos) {
			sel = random() % pnodulos.nodulos[nodule]->n_nodos;
			for(; pnodulos.nodulos[nodule]->conexiones_salida[sel][node] != 1;
				sel = random() % pnodulos.nodulos[nodule]->n_nodos);
			pnodulos.nodulos[nodule]->conexiones_salida[sel][node] = 0;
			pnodulos.nodulos[nodule]->pesos_salida[sel][node] = 0;
		} /* end if */
	} /* end if */
}

/********************************************************************************
 File: mutacionEstructural.c
 Function: borrarNodo()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Delete a given number of nodes from a nodule.
 Input Parameters:
   nodule: Integer. Number of nodule to work with.
   nodes. Integer. Number of nodes to delete.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   k: Integer. Counter.
   node: Integer. Node to delete.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
*******************************************************************************/

void borrarNodo(int nodule, int nodes)
{
	int i, j, k, node;
  
	for(k = 0; k < nodes; k++) {
		node = random() % pnodulos.nodulos[nodule]->n_nodos;

		/* All the nodes after the node to delete have to go back a position. */
		for(i = node + 1; i < pnodulos.nodulos[nodule]->n_nodos; i++) {
			pnodulos.nodulos[nodule]->transf[i - 1] =
				pnodulos.nodulos[nodule]->transf[i];
			for(j = 0; j < netPopulation.n_nodos_salida; j++) {
				pnodulos.nodulos[nodule]->conexiones_salida[i - 1][j] =
					pnodulos.nodulos[nodule]->conexiones_salida[i][j];
				pnodulos.nodulos[nodule]->pesos_salida[i - 1][j] =
					pnodulos.nodulos[nodule]->pesos_salida[i][j];
				pnodulos.nodulos[nodule]->conexiones_salida[i][j] = 0;
				pnodulos.nodulos[nodule]->pesos_salida[i][j] = 0;
			} /* end for */
		} /* end for */

		for(j = 0; j < netPopulation.n_nodos_salida; j++) {
			pnodulos.nodulos[nodule]->conexiones_salida[i - 1][j] = 0;
			pnodulos.nodulos[nodule]->pesos_salida[i - 1][j] = 0;
		} /* end for */

		for(i = 0; i < netPopulation.n_nodos_entrada; i++) {
			for(j = node + 1; j < pnodulos.nodulos[nodule]->n_nodos; j++) {
				pnodulos.nodulos[nodule]->conexiones_entrada[i][j - 1] =
					pnodulos.nodulos[nodule]->conexiones_entrada[i][j];
				pnodulos.nodulos[nodule]->pesos_entrada[i][j - 1] =
					pnodulos.nodulos[nodule]->pesos_entrada[i][j];
				pnodulos.nodulos[nodule]->conexiones_entrada[i][j] = 0;
				pnodulos.nodulos[nodule]->pesos_entrada[i][j] = 0;
			} /* end for */

			pnodulos.nodulos[nodule]->conexiones_entrada[i][j-1] = 0;
			pnodulos.nodulos[nodule]->pesos_entrada[i][j-1] = 0;
		} /* end for */
  
		pnodulos.nodulos[nodule]->n_nodos--;
	} /* end for */
}

/********************************************************************************
 File: mutacionEstructural.c
 Function: sustituirNodulos()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Substitude the worst nodules of a population with the bset nodules
              of its descendant population.
 Input Parameters: None
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   id: Vector of Integers. Store the nodules id in order to sort them by
       aptitude.
   order: Vector of floats. Store the nodules aptitude in order to sort them.
 Return Value: None
 Calling Functions
   liberarNodulo(): Free the memory ocuppied by a given nodule.
   ordenarNodulos(): Sort the nodules of the population.
   error(): Function to show an error message depending on an error number.
*******************************************************************************/

void sustituirNodulos()
{
	int i, j, *id;
	double *order;

	/* We order the nodules by aptitude. */
	id = (int *)malloc(num_nodulos * sizeof(int));
	order = (double *)malloc(num_nodulos * sizeof(double));
	if(id == NULL || order == NULL)
		error(RES_MEM);

	for(i = 0; i < num_nodulos; i++) {
		id[i] = ((pnodulos.n_subpobl - 1) * num_nodulos) + i;
		order[i] =
			pnodulos.nodulos[(pnodulos.n_subpobl - 1) * num_nodulos + i]->aptitud;
	}

	for(i = 0; i < num_nodulos; i++)
		for(j = i; j < num_nodulos; j++)
			if(order[i] < order[j]) {
				id[i] = ((pnodulos.n_subpobl - 1) * num_nodulos) + j;
				id[j] = ((pnodulos.n_subpobl-1) * num_nodulos) + i;
				order[i] = pnodulos.nodulos[((pnodulos.n_subpobl - 1) * num_nodulos) + j]->aptitud;
				order[j] = pnodulos.nodulos[((pnodulos.n_subpobl - 1) * num_nodulos) + i]->aptitud;
			}

	/*
	  We substitude the best nodules of the descendant subpopulation by the worst
	  nodules of the original subpopulation. 
	*/
	for(i = num_nodulos; i > nodsel; i--){
		for(j = 0; j < netPopulation.n_redes; j++)
			if(netPopulation.redes[j]->nodulos[pnodulos.n_subpobl - 1] ==
			   pnodulos.nodulos[id[i - 1]])
				netPopulation.redes[j]->nodulos[pnodulos.n_subpobl - 1] =
					descendencia[i - nodsel - 1];
		liberarNodulo(pnodulos.nodulos[id[i - 1]]);
		free(pnodulos.nodulos[id[i - 1]]);
		pnodulos.nodulos[id[i - 1]] = descendencia[i - nodsel - 1];
		descendencia[i - nodsel - 1] = NULL;
	}

	for(i = num_nodulos-nodsel; i < num_nodulos; i++)
		liberarNodulo(descendencia[i]);
	free(descendencia);
	descendencia = NULL;
	orderarNodulos();
	free(id);
	free(order);
}
