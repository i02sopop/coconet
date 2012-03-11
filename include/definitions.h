/********************************************************************************
* Copyright (c) 2004-2011 coconet project (see AUTHORS)			        *
*									        *
* This file is part of Coconet.						        *
*									        *
* Coconet is free software: you can redistribute it and/or modify it under the  *
* terms of the GNU General Public License as published by the Free Software     *
* Foundation, either version 3 of the License, or (at your option) any later    *
* version.                                                                      *
*									        *
* Coconet is distributed in the hope that it will be useful, but WITHOUT ANY    *
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR *
* A PARTICULAR PURPOSE.  See the GNU General Public License for more details.   *
*									        *
* You should have received a copy of the GNU General Public License along with  *
* coconet. If not, see <http://www.gnu.org/licenses/>.                          *
********************************************************************************/

#ifndef DEFINICIONES_H
#define DEFINICIONES_H

#include <macros.h>
#include <stdio.h>
#include <stdlib.h>

/*Parámetros de las funciones de transferencia*/
typedef struct{
    double logistic_a;
    double logistic_b;
    double htan_a;
    double htan_b;
    double epsilon;
}transf_param;

typedef struct{
	int id; /*Identificador del nódulo*/
	int n_nodos; /*número de nodos del nódulo*/
	double aptitud; /*última aptitud del nódulo*/
	int **conexiones_entrada; /*conexiones entre los nodos de entrada y los ocultos*/
	int **conexiones_salida; /*Conexiones entre los nodos ocultos y los de salida*/
	double **pesos_entrada;/*pesos de las conexiones de entrada*/
	double **pesos_salida;/*pesos de las conexiones de salida*/
	func *transf; /*función de transferencia de cada nodo*/
	double **salidas_parciales; /*Salidas parciales del nodulo*/
}nodulo;

typedef struct{
	nodulo **nodulos; /*nódulos que tiene la red*/
	double aptitud;/*aptitud de la red*/
	double *valores_salida;/*valores de salida de los nodos de salida*/
}red;

typedef struct{
	int n_redes;/*Número de redes de la población*/
	int n_nodos_entrada; /*Número de nodos de entrada de cada red*/
	int n_nodos_salida;/*Número de nodos de salida de cada red*/
	red **redes;/*redes de la población*/
}pobl_redes;

typedef struct{
	int n_subpobl;/*número de subpoblaciones*/
	int n_nodulos;/*número total de nódulos en la población*/
	nodulo **nodulos;/*nódulos de la población de nódulos*/
}pobl_nodulos;

/*Ponderación para la aptitud de los nodulos*/
typedef struct{
	double sust;
	double dif;
	double best;
	}ponderacion;

pobl_redes predes;            /* Población de redes. */
pobl_nodulos pnodulos;        /* Población de nódulos. */
int max_nodos;	              /* Número máximo de nodos en un nódulo. */
int num_nodulos;              /* Número de nódulos en cada subpoblación. */
int max_redes;                /* Número máximo de redes en la pobación. */
transf_param ptransferencia;  /* Parámetros de las funciones de transferencia. */
int n_entrenamiento;          /* Número de entradas en el fichero de entrenamiento, */
int n_generalizacion;         /* Número de entradas en el fichero de generalización. */
double **entrada;             /* Datos de entrada. */
double **salida;              /* Datos de salida. */
ponderacion pond;             /* Valores de ponderación de los parámetros de la aptitud
				 de los nódulos. */
int redsel;                   /* Numero de redes seleccionadas para algunas 
				 operaciones. */
double ToSA;                  /* Temperatura inicial para el enfriamiento simulado. */
double alphasa;               /* Factor de modificacion en la mutacion de pesos con el
				 enfriamiento simulado. */
int iteraciones_sa;           /* Número de iteraciones del algoritmo enfriamiento
				 simulado. */
int nodsel;                   /* Número de nodulos seleccionados para algunas
				 operaciones. */
int delta_min;                /* Valor minimo de la variable delta de la mutacion
				 estructural*/
int delta_max;                /* Valor maximo de la variable delta de la mutacion
				 estructural. */
nodulo **descendencia;        /* Descendencia de la ultima subpoblacion de nodulos. */
double evolim;                /* Límite de modificación para los criterios de paro. */
func net_transf;              /* Función de transferencia de las redes. */
double alpharet;              /* Variable de ponderación en la modificación de pesos de
				 la retropropagación. */

#include <funciones.h>

#endif /*DEFINICIONES_H*/
