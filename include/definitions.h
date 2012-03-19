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

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <macros.h>

/* Parameters for the transfer function. */
typedef struct {
    double logistic_a;
    double logistic_b;
    double htan_a;
    double htan_b;
    double epsilon;
} transf_param;

typedef struct {
	int id; /* Id of the nodule. */
	int nodes; /* Number of nodes of the nodule. */
	double aptitude; /* Nodule aptitude. */
	int **inConn; /* Connections between input and hidden nodes. */
	int **outConn; /* Connections between hidden and output nodes. */
	double **inWeights; /* Weights of the input connections. */
	double **outWeights; /* Weights of the output connections. */
	func *transf; /* Transfer function of each node. */
	double **partialOutputs; /* Partial outputs of the nodule. */
} nodule;

typedef struct {
	nodule **nodulos; /* Nodules of the network. */
	double aptitud; /* Network aptitude. */
	double *valores_salida; /* Output values of the output nodes. */
} red;

typedef struct {
	int n_redes; /* Number of tetworks at the population. */
	int n_nodos_entrada; /* Number of input nodes of each network. */
	int n_nodos_salida; /* Number of output networks of each network. */
	red **redes; /* Networks of the population. */
} pobl_redes;

typedef struct {
	int n_subpobl; /* Number of subpopulations. */
	int n_nodulos; /* Total number of nodules at the population. */
	nodulo **nodulos; /* Nodules of the population. */
}pobl_nodulos;

/* Ponderation for the nodule population. */
typedef struct {
	double sust;
	double dif;
	double best;
} ponderacion;

pobl_redes netPopulation;            /* Network population. */
pobl_nodulos pnodulos;        /* Nodule population. */
int max_nodos;	              /* Max number of nodes in a nodule. */
int num_nodulos;              /* Max number of nodules in a subpopulation. */
int max_redes;                /* Max number of networks in the population. */
transf_param ptransferencia;  /* Parameters of the transfer functions. */
int n_entrenamiento;          /* Number of entries at the training file, */
int n_generalizacion;         /* Number of entries at the generalization file. */
double **entrada;             /* Input data. */
double **salida;              /* Output data. */
ponderacion pond;             /* Parameters ponderation values for the nodule aptitude. */
int redsel;                   /* Number of networks selected to perform some operations. */
double ToSA;                  /* Initial temperatire for the simulated annealing. */
double alphasa;               /* Change factor in the weight mutation for the simulated annealing. */
int iteraciones_sa;           /* Number of iterations of the simulated annealing. */
int nodsel;                   /* Number of nodes selected to perform some operations. */
int delta_min;                /* Min value of delta for the structural mutation. */
int delta_max;                /* Max value of delta for the structural mutation. */
nodule **descendencia;        /* Descendant of the last nodules subpopulation. */
double evolim;                /* Modification limit for the stop criteria. */
func net_transf;              /* Transfer function for the networks. */
double alpharet;              /* Ponderation in the changing of weights during the backpropagation. */

#include <functions.h>

#endif /* DEFINITIONS_H */
