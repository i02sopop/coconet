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

/*****************************************************************************
 File: evolucionarPoblaciones.c
 Function: evolucionarPoblaciones()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Evolve the networks and nodules populations.
 Input Parameters: None
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   k: Integer. Counter.
   initialNumNodules: Integer. Initial number of nodules of the subpopulation.
   networkAptitude: Float. Store the average aptitude of the networks.
 Return Value: None
 Calling Functions:
   retropropagacion(): Run the backpropagation algorithm over a given nodule.
   generarSalidaNodulo(): Generate the outout of a nodule with an input data
                          given.
   generarSalidaRed(): Generate the output of a network combining the nodule
                       outputs.
   medirAptitudRed(): Measure the network aptitude.
   medirAptitudNodulos(): Measure the nodule aptitude.
   normalizarAptitudNodulos(): Normalize the nodule aptitude.
   ordenarRedes(): Sort the networks by aptitude.
   enfriamientoSimulado(): Run the simulated annealing over a nodule.
   ordenarNodulos(): Sort the nodules by aptitude.
   copiarDescendencia(): Make a copy of the nodules subpopulation.
   mutarNodulos(): Make an structural mutation of a nodule.
   sustituirNodulos(): Substitution of the worst nodules by the mutation of
                       the best nodules in the subpopulation.
*****************************************************************************/

void evolucionarPoblaciones()
{
	int i, j, k, initialNumNodules;
	double networkAptitude;

	/* Local variables initialization. */
	networkAptitude = 0.0;
	for(i = 0; i < predes.n_redes; i++)
		predes.redes[i]->aptitud=0;

	/* We evolve the networks until its average aptitude doesn't get enhaced
	   enough. */
	for(i = 0; medirCambio(&networkAptitude, i) == false; i++) {
		initialNumNodules = num_nodulos * (pnodulos.n_subpobl - 1);
		if(i == 0) {
			/* If the subpopulation is new we run the backpropagation to reduce
			   its error. */
			fprintf(stderr, "Doing the backpropagation to a new nodule "
					"subpopulation. \n");
			for(j = 0; j < num_nodulos; j++)
				retropropagacion(initialNumNodules + j, n_entrenamiento, 5000);

			/* We train by first time the networks to have an initial value of
			   the nodules and networks aptitude. */
			fprintf(stderr,"We train by first time the networks.\n");
			for(j = 0; j < n_entrenamiento; j++) {
				/* We generate the nodules output. */
				for(k = pnodulos.n_nodulos - num_nodulos; k < pnodulos.n_nodulos ;k++)
					generarSalidaNodulo(entrada[j], k, j, NULL);

				/* We measure the networks aptitude. */
				for(k = 0; k < predes.n_redes; k++) {
					generarSalidaRed(k, j);
					medirAptitudRed(salida[j], k);
				}
			}

			/* We normalize the network aptitude. */
			for(j = 0; j < predes.n_redes; j++)
				predes.redes[j]->aptitud /= n_entrenamiento;

			/* We measure and normalize the nodules aptitude. */
			for(j = 0; j < pnodulos.n_nodulos; j++)
				medirAptitudNodulos(j);
			normalizarAptitudNodulos();
		}

		/* We sort the networks by aptitude. */
		fprintf(stderr, "Sorting networks.\n");
		ordenarRedes();

		/* We train the networks by simulate annealing. */
		fprintf(stderr,"Training the networks by simulate annealing.\n");
		for(j = initialNumNodules; j < pnodulos.n_nodulos; j++)
			enfriamientoSimulado(j);

		/* We normalize the nodules aptitude. */
		fprintf(stderr, "Normalization of the nodules aptitude.\n");
		normalizarAptitudNodulos();

		/* We sort the nodules by aptitude. */
		fprintf(stderr,"Sorting nodules.\n");
		ordenarNodulos();

		/* We create a new subpopulation descendant of the actual one. */
		fprintf(stderr,"Creating new subpopulation.\n");
		copiarDescendencia();

		/* We make the structural mutation of the nodules. */
		fprintf(stderr,"Nodules mutation.\n");
		for(j = initialNumNodules; j < pnodulos.n_nodulos; j++)
			mutarNodulos(j);

		/* We substitude the worst nodules by the best ones in the descendant
		   population. */
		fprintf(stderr,"Susbstitution of the worst nodules by the bes ones.\n");
		sustituirNodulos();

		fprintf(stderr,"End of the evolve.\n");
	}
}

/******************************************************************************
 File: evolucionarPoblaciones.c
 Function: retropropagacion()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Run the backpropagation algorithm to a given nodule.
 Input Parameters:
   nodule: Integer. Number of the nodule to work with.
   numPatterns: Integer. Number of input patterns.
   iter: Integer. Number of iterations of the algorithm.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   k: Integer. Counter.
   pattern: Integer. Pattern number selected to run the backpropagation.
   numNodes: Integer. Number of nodes of the nodule.
   initialError: Float array. Store the initial error of each nodule.
   F_W: Float array. Store the change to apply over the nodule weights.
   out: Float array. Store the output of each node of the nodule.
   weights: Float array. Store the connection weights of the nodule.
   transf: Functions array. Store the transfer functions of each node of the
           nodule.
 Return Value: None
 Calling Functions:
   generarSalidaNodulo(): Generate the output of a nodule given an entry
                          pattern.
   cambioPesos(): Function to calculate the change value of each weight to
                  reduce the output error.
   error(): Function to print an error message.
******************************************************************************/

void retropropagacion(int nodule, int numPatterns, int iter)
{
	int i, j, k, pattern, numNodes;
	double *initialError, **F_W, *out,* *weights;
	func *transf;

	/* We initializate the local variables. */
	numNodes = pnodulos.nodulos[nodule]->n_nodos + predes.n_nodos_entrada +
		predes.n_nodos_salida;

	transf = (func *)malloc((predes.n_nodos_salida + pnodulos.nodulos[nodule]->n_nodos) * sizeof(func));
	out = (double *)malloc(numNodes * sizeof(double));
	F_W = (double **)malloc(numNodes * sizeof(double));
	weights = (double **)malloc(numNodes * sizeof(double));
	initialError = (double *)malloc(predes.n_nodos_salida * sizeof(double));

	if(transf == NULL || out == NULL || F_W == NULL || weights == NULL ||
	   initialError == NULL)
		error(RES_MEM);

	for(i = 0; i < numNodes; i++) {
		F_W[i] = (double *)malloc(numNodes * sizeof(double));
		weights[i] = (double *)malloc(numNodes * sizeof(double))
		if(F_W[i] == NULL || weights[i] == NULL)
			error(RES_MEM);

		for(j = 0; j < numNodes; j++) {
			F_W[i][j] = 0.0;
			weights[i][j] = 0.0;
		}
	}

	for(i = 0; i < pnodulos.nodulos[nodule]->n_nodos; i++)
		transf[i] = pnodulos.nodulos[nodule]->transf[i];

	for(;i < pnodulos.nodulos[nodule]->n_nodos + predes.n_nodos_salida; i++)
		transf[i] = net_transf;

	/* Backpropagation. */
	for(k = 0; k < iter; k++) {
		/* We pick a pattern. */
		pattern = random() % numPatterns;
		generarSalidaNodulo(entrada[pattern], nodule, pattern, out);

		/* We assign the weight to the weights matrix*/
		for(j = 0; j < predes.n_nodos_salida; j++)
			initialError[j] = out[j + predes.n_nodos_entrada + pnodulos.nodulos[nodule]->n_nodos] -
				salida[pattern][j];

		for(i = 0; i < pnodulos.nodulos[nodule]->n_nodos; i++)
			for(j = 0; j < predes.n_nodos_entrada; j++)
				weights[i + predes.n_nodos_entrada][j] =
					pnodulos.nodulos[nodule]->pesos_entrada[j][i];

		for(i = 0; i < predes.n_nodos_salida; i++)
			for(j = 0; j < pnodulos.nodulos[nodule]->n_nodos; j++)
				weights[i + predes.n_nodos_entrada + pnodulos.nodulos[nodule]->n_nodos][j + predes.n_nodos_entrada] =
					pnodulos.nodulos[nodule]->pesos_salida[j][i];

		/* Obtain the weight change. */
		cambioPesos(initialError, weight, out, F_W, pnodulos.nodulos[nodule]->n_nodos, transf);

		/* We update the weigths. */
		for(i = 0; i < pnodulos.nodulos[nodule]->n_nodos; i++)
			for(j = 0; j < predes.n_nodos_entrada; j++)
				pnodulos.nodulos[nodule]->pesos_entrada[j][i] -= alpharet *
					F_W[i + predes.n_nodos_entrada][j];

		for(i = 0; i < predes.n_nodos_salida; i++)
			for(j = 0; j < pnodulos.nodulos[nodule]->n_nodos; j++)
				pnodulos.nodulos[nodule]->pesos_salida[j][i] -= alpharet *
					F_W[i + predes.n_nodos_entrada + pnodulos.nodulos[nodule]->n_nodos][j + predes.n_nodos_entrada];
	}

	/* Free memory. */
	for(i = 0; i < numNodes; i++) {
		free(weights[i]);
		free(F_W[i]);
	}

	free(weights);
	free(F_W);
	free(initialError);
	free(out);
	free(transf);
}

/******************************************************************************
 File: evolucionarPoblaciones.c
 Function: cambioPesos()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Calculate the differential value to apply to each weight to
              reduce the error of a nodule.
 Input Parameters:
   nodes: Integer. Number of nodes at the nodule.
   initialError: Array of floats. Store the initial error of the nodule.
   F_W: Matrix of floats. Store the change to apply in each nodule weight.
   out: Array of floats. Store the output of each node of the nodule.
   weights: Matrix of floats. Store all the connection weights of the nodule.
   transf: Array of functions. Store the transfer functions of each node of the
           nodule.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   k: Integer. Number of input and hidden nodes of the nodule.
   nodeError: Array of floats. Output error of a nodule.
   netError: Array of floats. Network error in a given node.
 Return Value: None
 Calling functions:
   error(): Function to print an error message.
******************************************************************************/

void cambioPesos(double *initialError, double **weights, double *out, double **F_W,
				 int nodes, func *transf)
{
	int i, j, numNodes;
	double *nodeError, *netError;

	/* Variable initialization. */
	numNodes = predes.n_nodos_entrada + nodes;
	nodeError = (double *)malloc((numNodes + predes.n_nodos_salida) * sizeof(double));
	netError = (double *)malloc((numNodes + predes.n_nodos_salida) * sizeof(double));
	if(nodeError == NULL || netError == NULL)
		error(RES_MEM);

	for(i = 0; i < numNodes; i++)
		nodeError[i] = 0.0;

	for(i = 0; i < predes.n_nodos_salida; i++)
		nodeError[numNodes + i] = initialError[i];

	/* We get the updates. */
	for(i = numNodes + predes.n_nodos_salida - 1; i >= predes.n_nodos_entrada; i--) {
		for(j = i + 1; j < numNodes + predes.n_nodos_salida; j++)
			nodeError[i] += weights[j][i] * netError[j];

		if(transf[i - predes.n_nodos_entrada] == (func)&Logistic)
			netError[i] = nodeError[i] * ptransferencia.logistic_b * out[i] *
				(1.0 - out[i] / ptransferencia.logistic_a);
		else if(transf[i - predes.n_nodos_entrada] == (func)&HyperbolicTangent)
			netError[i] = nodeError[i] * (ptransferencia.htan_b / ptransferencia.htan_a) *
				(ptransferencia.htan_a - out[i]) *
				(ptransferencia.htan_a + out[i]);

		for(j = 0; j < i; j++)
			F_W[i][j] = netError[i] *out[j];
	}
}

/******************************************************************************
 File: evolucionarPoblaciones.c
 Function: enfriamientoSimulado()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Run the simulated annealing over a given nodule.
 Input Parameters:
   nodNumber: Integer. Number of nodule to run over.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   k: Integer. Counter.
   steps: Integer. Number of iterations of the annealing.
   T: Float. Temperature of the simulated annealing.
   weights: Array of floats. Old weights.
   oldAptitude: Float. Old nodule aptitude.
 Return Value: None
 Calling functions:
   pasoAleatorio(): Make a random step in the connection weights.
   medirCambioNodulo(): Measure the aptitude change at the nodule.
******************************************************************************/

void enfriamientoSimulado(int nodNumber)
{
	int i, j, k, steps;
	double T, *weights, oldAptitude;

	/* Variable initialization. */
	T = ToSA;
	weights = NULL;
	oldAptitude = pnodulos.nodulos[nodNumber]->aptitud;

	/* Simulated annealing. */
	for(steps = 0; steps < iteraciones_sa; steps++) {
		/* We keep the old weights. */
		k = 0;

		/* Input weights. */
		for(i = 0; i < predes.n_nodos_entrada; i++)
			for(j = 0; j < pnodulos.nodulos[nodNumber]->n_nodos; j++)
				if(pnodulos.nodulos[nodNumber]->conexiones_entrada[i][j]) {
					weigths = (double *)realloc(weigths, (k + 1) * sizeof(double));
					weigths[k] = pnodulos.nodulos[nodNumber]->pesos_entrada[i][j];
					k++;
				}

		/* Output weights. */
		for(i = 0; i < pnodulos.nodulos[nodNumber]->n_nodos; i++)
			for(j = 0; j < predes.n_nodos_salida; j++)
				if(pnodulos.nodulos[nodNumber]->conexiones_salida[i][j]) {
					weigths = (double *)realloc(weigths, (k + 1) * sizeof(double));
					weigths[k] = pnodulos.nodulos[nodNumber]->pesos_salida[i][j];
					k++;
				}

		/* We make a randon step. */
		pasoAleatorio(nodNumber);

		/* We calculate the aptitude of the new nodule. */
		medirCambioNodulo(nodNumber);

		/* If the aptitude is worst we reject the change. */
		if((oldAptitude > pnodulos.nodulos[nodNumber]->aptitud) &&
		   aleatorio() < (oldAptitude - pnodulos.nodulos[nodNumber]->aptitud) * T) {
			/* We restore the old weights. */
			k = 0;

			/* Input weights. */
			for(i = 0; i < predes.n_nodos_entrada; i++)
				for(j = 0; j < pnodulos.nodulos[nodNumber]->n_nodos; j++)
					if(pnodulos.nodulos[nodNumber]->conexiones_entrada[i][j]) {
						pnodulos.nodulos[nodNumber]->pesos_entrada[i][j] = weigths[k];
						k++;
					}

			/* Output weights. */
			for(i = 0; i < pnodulos.nodulos[nodNumber]->n_nodos; i++)
				for(j = 0; j < predes.n_nodos_salida; j++)
					if(pnodulos.nodulos[nodNumber]->conexiones_salida[i][j]) {
						pnodulos.nodulos[nodNumber]->pesos_salida[i][j] = weigths[k];
						k++;
					}

			/* We recalculate the aptitude of the nodule. */
			medirCambioNodulo(nodNumber);
		} else
			oldAptitude = pnodulos.nodulos[nodNumber]->aptitud;

		/* We update the aptitude temperature. */
		T = alphasa * T;
	}

	free(weigths);
}

/******************************************************************************
 File: evolucionarPoblaciones.c
 Function: pasoAleatorio()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Make a randon step in each nodule weight.
 Input Parameters:
   nodNumber: Integer. Number of the nodule to work with.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
 Return Value: None
 Calling Functions:
   Normal(): Generate a randon value following a normal distribution.
******************************************************************************/

void pasoAleatorio(int nodNumber)
{
	int i, j;

	/* Randon step in input connection weights. */
	for(i = 0; i < predes.n_nodos_entrada; i++)
		for(j = 0; j < pnodulos.nodulos[nodNumber]->n_nodos; j++)
			if(pnodulos.nodulos[nodNumber]->conexiones_entrada[i][j])
				pnodulos.nodulos[nodNumber]->pesos_entrada[i][j] +=
					Normal(0.0, 1.0);

	/* Random step in output connection weights.  */
	for(i = 0; i < pnodulos.nodulos[nodNumber]->n_nodos; i++)
		for(j = 0; j < predes.n_nodos_salida; j++)
			if(pnodulos.nodulos[nodNumber]->conexiones_salida[i][j] == 1)
				pnodulos.nodulos[nodNumber]->pesos_salida[i][j] +=
					Normal(0.0, 1.0);
}
