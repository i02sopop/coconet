/********************************************************************************
 * Copyright 2005 Pablo Alvarez de Sotomayor Posadillo			                *
 *									                                            *
 * This file is part of Coconet.						                        *
 *									                                            *
 * Coconet is free software; you can redistribute it and/or modify	            *
 * it under the terms of the GNU General Public License as published by	        *
 * the Free Software Foundation; either version 2 of the License, or	        *
 * (at your option) any later version.					                        *
 *									                                            *
 * Coconet is distributed in the hope that it will be useful,		            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		        *
 * GNU General Public License for more details.				                    *
 *									                                            *
 * You should have received a copy of the GNU General Public License	        *
 * along with Coconet; if not, write to the Free Software		                *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA    *
 ********************************************************************************/

#include <definiciones.h>

/*******************************************************************************
 * File: crearPoblaciones.c						                               *
 *									                                           *
 * Function: anadirNoduloRedes()						                       *
 *									                                           *
 * Author: Pablo Alvarez de Sotomayor Posadillo				                   *
 *									                                           *
 * Functionality: Add a new nodule subpopulation to the network population.    *
 *									                                           *
 * Input parameters: None       					                           *
 *									                                           *
 * Internal parameters: None						                           *
 *									                                           *
 * Output parameters: None      						                       *
 *									                                           *
 * Functions that calls:                 					                   *
 *	crearRedes(): Create a new network population from the previous nodes      *
 *                population.                                                  *
 *	crearDescendencia(): Create a new network population from the nodule       *
 *                       subpopulation and the previous network population.    *
 *	error(): Function to print an error message.                               *
 *******************************************************************************/

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

/*******************************************************************************
 * File: crearPoblaciones.c						                               *
 *					                                     				       *
 * Function: crearDescendencia()						                       *
 *									                                           *
 * Author: Pablo Álvarez de Sotomayor Posadillo         				       *
 *									                                           *
 * Functionality: Create a new network population from a new nodule population *
 *                and the previous network population.                         *
 *									                                           *
 * Input parameters: None       					                           *
 * 									                                           *
 * Internal parameters:							                               *
 * 	i: Integer. Counter.						                               *
 * 	j: Integer. Counter.						                               *
 * 	redsel: Integer. Selection of a network of the present network.            *
 * 	nodsel: Integer. Selection of a nodule from the new subpopulation.         *
 * 	nueva: Array of networks. New network population.                          *
 * 									                                           *
 * Output parameters: None      						                       *
 * 									                                           *
 * Functions that calls:                 					                   *
 *	liberarRed(): Function to free a given network.			                   *
 *	error(): Function to print an error message.                               *
 *******************************************************************************/

void crearDescendencia()
{
	int i, j, redsel, nodsel;
	red **nueva; /* New network population. Descend of the actual population. */

	/* Creation of the new network. */
	if((nueva = (red **)malloc(max_redes * sizeof(red))) == NULL)
		error(RES_MEM);
	for(i = 0; i < max_redes; i++) {
		if((nueva[i] = (red *)malloc(sizeof(red))) == NULL)
			error(RES_MEM);

		nueva[i]->aptitud = 0;
		if((nueva[i]->valores_salida = (double *)malloc(predes.n_nodos_salida * sizeof(double))) == NULL)
			error(RES_MEM);

		if((nueva[i]->nodulos = (nodulo **)malloc(pnodulos.n_subpobl * sizeof(nodulo))) == NULL)
			error(RES_MEM);

		/* Select a new network randomly. */
		redsel = random() % predes.n_redes;

		/* Random selection of the node to add. */
		nodsel = random() % num_nodulos;

		/* Creation of the new network. */
		for(j = 0; j < pnodulos.n_subpobl - 1; j++)
			nueva[i]->nodulos[j] = predes.redes[redsel]->nodulos[j];
		nueva[i]->nodulos[pnodulos.n_subpobl - 1] = pnodulos.nodulos[(pnodulos.n_subpobl - 1) * num_nodulos + nodsel];
	}

	/* We fix the new network. */
	for(i = 0; i < predes.n_redes; i++)
		liberarRed(predes.redes[i]);

	free(predes.redes);
	predes.redes = nueva;
	nueva = NULL;
	free(nueva);
}

/*******************************************************************************
 * File: crearPoblaciones.c						                               *
 *									                                           *
 * Function: liberarRed()							                           *
 *	                                    								       *
 * Author: Pablo Álvarez de Sotomayor Posadillo	            			       *
 *									                                           *
 * Functionality: Function to free a given network.		                       *
 *									                                           *
 * Input parameters:            					                           *
 * 	borrar: Red. Network to delete.				                               *
 * 									                                           *
 * Internal parameters:							                               *
 * 	i: Integer. Counter.						                               *
 * 									                                           *
 * Output parameters: None      						                       *
 * 									                                           *
 * Functions that calls: None              					                   *
 *******************************************************************************/

void liberarRed(red *borrar)
{
	int i;

	/* We delete all the network. */
	for(i = 0; i < pnodulos.n_subpobl; i++)
		borrar->nodulos[i] = NULL;

	free(borrar->nodulos);
	free(borrar->valores_salida);
	free(borrar);
}

/*******************************************************************************
 * File: crearPoblaciones.c						                               *
 *									                                           *
 * Function: crearNodulos()	                        					       *
 *									                                           *
 * Author: Pablo Álvarez de Sotomayor Posadillo				                   *
 *									                                           *
 * Functionality: Create a new nodule subpopulation.                           *
 *									                                           *
 * Input parameters: None       					                           *
 * 									                                           *
 * Internal parameters:							                               *
 * 	i: Integer. Counter.						                               *
 * 	j: Integer. Counter.						                               *
 * 	k: Integer. Counter.				                        		       *
 * 	                                    								       *
 * Output parameters: None      						                       *
 *                                  									       *
 * Functions that calls:                					                   *
 *	error(): Function to print an error message.                               *
 *******************************************************************************/

void crearNodulos()
{
	int i, j, k;
	
	/* We create the nodule subpopulation. */
	for(i = pnodulos.n_nodulos - num_nodulos; i < pnodulos.n_nodulos; i++) {
		if((pnodulos.nodulos[i] = (nodulo *)malloc(sizeof(nodulo))) == NULL)
			error(RES_MEM);
    
		pnodulos.nodulos[i]->id = i;   /* Nodule id. */
		pnodulos.nodulos[i]->n_nodos = random() % max_nodos;     /* Number of hidden nodes of the nodule. */
		if(pnodulos.nodulos[i]->n_nodos == 0)
			pnodulos.nodulos[i]->n_nodos++;
		pnodulos.nodulos[i]->aptitud = 0; /* Initial aptitude. */
  
		/* Reserva de memoria para la creación de conexiones y pesos. */
		/* Conexiones nodos de entrada->nodos ocultos. */
		if((pnodulos.nodulos[i]->conexiones_entrada = (int **)malloc(predes.n_nodos_entrada * sizeof(int))) == NULL)
			error(RES_MEM);

		/* Conexiones nodos ocultos->nodos de salida. */
		if((pnodulos.nodulos[i]->conexiones_salida = (int **)malloc(max_nodos * sizeof(int))) == NULL)
			error(RES_MEM);

		/* Pesos de conexiones nodos de entrada->nodos ocultos. */
		if((pnodulos.nodulos[i]->pesos_entrada = (double **)malloc(predes.n_nodos_entrada * sizeof(double))) == NULL)
			error(RES_MEM);

		/* Pesos de conexiones nodos ocultos->nodos de salida. */
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
  
		/* Creación de conexiones junto con sus pesos. */
		/* Conexiones y pesos de entrada. */
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

		/* Connections and output weights. */
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
 
		/*Asignación de la función de transferencia a cada nodo*/
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

/*******************************************************************************
 * File: crearPoblaciones.c                     						       *
 *									                                           *
 * Function: crearPoblacionNodulos()					                       *
 *									                                           *
 * Author: Pablo Álvarez de Sotomayor Posadillo				                   *
 *									                                           *
 * Functionality: Create or expand the nodule population.                      *
 *									                                           *
 * Input parameters: None       					                           *
 * 									                                           *
 * Internal parameters:	None					                               *
 * 	                                    								       *
 * Output parameters: None      						                       *
 * 									                                           *
 * Functions that calls:                					                   *
 *	crearNodulos(): Function to create a new nodule subpopulation.             *
 *	error(): Function to print an error message.                               *
 *******************************************************************************/

void crearPoblacionNodulos()
{
	/* Se actualizan el número de subpoblaciones y el número de nódulos de la población de nódulos. */
	pnodulos.n_subpobl++;
	pnodulos.n_nodulos = pnodulos.n_subpobl*num_nodulos;

	/* Se reserva memoria para la nueva subpoblación. */
	if(pnodulos.n_subpobl == 1) {
		if((pnodulos.nodulos = (nodulo **)malloc(num_nodulos * sizeof(nodulo))) == NULL)
			error(RES_MEM);
	} else {
		if((pnodulos.nodulos = (nodulo **)realloc(pnodulos.nodulos, pnodulos.n_nodulos * sizeof(nodulo))) == NULL)
			error(RES_MEM);
	}

	/* Se crean los nódulos de la nueva subpoblación. */
	crearNodulos();
}

/*******************************************************************************
 * File: crearPoblaciones.c						                               *
 *									                                           *
 * Function: crearRedes()							                           *
 *									                                           *
 * Author: Pablo Alvarez de Sotomayor Posadillo				                   *
 *									                                           *
 * Functionality: Create a new network population.                             *
 *									                                           *
 * Input parameters: None       					                           *
 * 									                                           *
 * Internal parameters:	    					                               *
 * 	i: Integer. Counter.						                               *
 * 	j: Integer. Counter.						                               *
 *									                                           *
 * Output parameters: None      						                       *
 *									                                           *
 * Functions that calls:                					                   *
 *	error(): Function to print an error message.                               *
 *******************************************************************************/

void crearRedes()
{
	int i, j;
  
	for(i = 0; i < num_nodulos; i++) {
		if((predes.redes[i] = (red *)malloc(sizeof(red))) == NULL)
			error(RES_MEM);

		/* Se reserva memoria y se inicializan las variables */
		predes.redes[i]->aptitud = 0.0;
		if((predes.redes[i]->nodulos = (nodulo **)malloc(sizeof(nodulo))) == NULL)
			error(RES_MEM);

		/* Se le asigna la red su primer nódulo. */
		predes.redes[i]->nodulos[0] = pnodulos.nodulos[i];
		if((predes.redes[i]->valores_salida = (double *)malloc(predes.n_nodos_salida * sizeof(double))) == NULL)
			error(RES_MEM);

		for(j = 0; j < predes.n_nodos_salida; j++)
			predes.redes[i]->valores_salida[j] = 0.0;
	}
}
