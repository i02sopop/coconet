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

/* General headers. */
#include <stdio.h>
#include <stdlib.h>

/* Gettext headers. */
#include <config.h>
#include <locale.h>
#include <gettext.h>

/* Local headers. */
#include <macros.h>

/* Parameters for the transfer function. */
typedef struct {
	double logistic_a;
	double logistic_b;
	double htan_a;
	double htan_b;
	double epsilon;
} transfParam;

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
	nodule **nodules; /* Nodules of the network. */
	double aptitude; /* Network aptitude. */
	double *outValues; /* Output values of the output nodes. */
} network;

typedef struct {
	int numNets; /* Number of networks at the population. */
	int numInputNodes; /* Number of input nodes of each network. */
	int numOutputNodes; /* Number of output networks of each network. */
	network **nets; /* Networks of the population. */
} netPopulation;

typedef struct {
	int numSubpops; /* Number of subpopulations. */
	int numNodules; /* Total number of nodules at the population. */
	nodule **nodules; /* Nodules of the population. */
} nodulePopulation;

/* Ponderation for the nodule population. */
typedef struct {
	double sust;
	double dif;
	double best;
} adjustment;

netPopulation cNetPopulation;       /* Network population. */
nodulePopulation cNodulePopulation; /* Nodule population. */
int numNodules;                     /* Max number of nodules in a subpopulation. */
int maxNodes;	                    /* Max number of nodes in a nodule. */
int maxNetworks;                    /* Max number of networks in the population. */
transfParam pTransfer;              /* Parameters of the transfer functions. */
int numTrain;                       /* Number of entries at the training file, */
int numGeneral;                     /* Number of entries at the generalization file. */
double **inputData;                 /* Input data. */
double **outputData;                /* Output data. */
adjustment adj;                     /* Parameters ponderation values for the nodule aptitude. */
int selNets;                        /* Number of networks selected to perform some operations. */
double ToSA;                        /* Initial temperatire for the simulated annealing. */
double alphasa;                     /* Change factor in the weight mutation for the simulated annealing. */
int numSAIterantions;               /* Number of iterations of the simulated annealing. */
int nodSel;                         /* Number of nodes selected to perform some operations. */
int delta_min;                      /* Min value of delta for the structural mutation. */
int delta_max;                      /* Max value of delta for the structural mutation. */
nodule **descend;                   /* Descendant of the last nodules subpopulation. */
double evolim;                      /* Modification limit for the stop criteria. */
func netTransf;                     /* Transfer function for the networks. */
double alpharet;                    /* Ponderation in the changing of weights during the backpropagation. */

#include <functions.h>

#endif /* DEFINITIONS_H */
