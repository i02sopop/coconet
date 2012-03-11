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

/*Par�metros de las funciones de transferencia*/
typedef struct{
    double logistic_a;
    double logistic_b;
    double htan_a;
    double htan_b;
    double epsilon;
}transf_param;

typedef struct{
	int id; /*Identificador del n�dulo*/
	int n_nodos; /*n�mero de nodos del n�dulo*/
	double aptitud; /*�ltima aptitud del n�dulo*/
	int **conexiones_entrada; /*conexiones entre los nodos de entrada y los ocultos*/
	int **conexiones_salida; /*Conexiones entre los nodos ocultos y los de salida*/
	double **pesos_entrada;/*pesos de las conexiones de entrada*/
	double **pesos_salida;/*pesos de las conexiones de salida*/
	func *transf; /*funci�n de transferencia de cada nodo*/
	double **salidas_parciales; /*Salidas parciales del nodulo*/
}nodulo;

typedef struct{
	nodulo **nodulos; /*n�dulos que tiene la red*/
	double aptitud;/*aptitud de la red*/
	double *valores_salida;/*valores de salida de los nodos de salida*/
}red;

typedef struct{
	int n_redes;/*N�mero de redes de la poblaci�n*/
	int n_nodos_entrada; /*N�mero de nodos de entrada de cada red*/
	int n_nodos_salida;/*N�mero de nodos de salida de cada red*/
	red **redes;/*redes de la poblaci�n*/
}pobl_redes;

typedef struct{
	int n_subpobl;/*n�mero de subpoblaciones*/
	int n_nodulos;/*n�mero total de n�dulos en la poblaci�n*/
	nodulo **nodulos;/*n�dulos de la poblaci�n de n�dulos*/
}pobl_nodulos;

/*Ponderaci�n para la aptitud de los nodulos*/
typedef struct{
	double sust;
	double dif;
	double best;
	}ponderacion;

pobl_redes predes;            /* Poblaci�n de redes. */
pobl_nodulos pnodulos;        /* Poblaci�n de n�dulos. */
int max_nodos;	              /* N�mero m�ximo de nodos en un n�dulo. */
int num_nodulos;              /* N�mero de n�dulos en cada subpoblaci�n. */
int max_redes;                /* N�mero m�ximo de redes en la pobaci�n. */
transf_param ptransferencia;  /* Par�metros de las funciones de transferencia. */
int n_entrenamiento;          /* N�mero de entradas en el fichero de entrenamiento, */
int n_generalizacion;         /* N�mero de entradas en el fichero de generalizaci�n. */
double **entrada;             /* Datos de entrada. */
double **salida;              /* Datos de salida. */
ponderacion pond;             /* Valores de ponderaci�n de los par�metros de la aptitud
				 de los n�dulos. */
int redsel;                   /* Numero de redes seleccionadas para algunas 
				 operaciones. */
double ToSA;                  /* Temperatura inicial para el enfriamiento simulado. */
double alphasa;               /* Factor de modificacion en la mutacion de pesos con el
				 enfriamiento simulado. */
int iteraciones_sa;           /* N�mero de iteraciones del algoritmo enfriamiento
				 simulado. */
int nodsel;                   /* N�mero de nodulos seleccionados para algunas
				 operaciones. */
int delta_min;                /* Valor minimo de la variable delta de la mutacion
				 estructural*/
int delta_max;                /* Valor maximo de la variable delta de la mutacion
				 estructural. */
nodulo **descendencia;        /* Descendencia de la ultima subpoblacion de nodulos. */
double evolim;                /* L�mite de modificaci�n para los criterios de paro. */
func net_transf;              /* Funci�n de transferencia de las redes. */
double alpharet;              /* Variable de ponderaci�n en la modificaci�n de pesos de
				 la retropropagaci�n. */

#include <funciones.h>

#endif /*DEFINICIONES_H*/
