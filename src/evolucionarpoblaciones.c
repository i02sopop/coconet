/******************************************************************************
 Copyright (c) 2004-2011 coconet project (see AUTHORS)

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
   numNodulo: Integer. Number of the nodule to work with.
   n_patrones: Integer. Number of input patterns.
   iteraciones: Integer. Number of iterations of the algorithm.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   iter: Integer. Counter.
   patron: Integer. Pattern number selected to run the backpropagation.
   numNodos: Integer. Number of nodes of the nodule.
   einicial: Float array. Store the initial error of each nodule.
   F_W: Float array. Store the change to apply over the nodule weights.
   out: Float array. Store the output of each node of the nodule.
   pesos: Float array. Store the connection weights of the nodule.
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

void retropropagacion(int numNodulo,int n_patrones,int iteraciones)
{
	int i, j, iter, patron, numNodos;
	double *einicial, **F_W, *out,* *pesos;
	func *transf;

	/* We initializate the local variables. */
	numNodos = pnodulos.nodulos[numNodulo]->n_nodos + predes.n_nodos_entrada +
		predes.n_nodos_salida;

	transf = (func *)malloc((predes.n_nodos_salida + pnodulos.nodulos[numNodulo]->n_nodos) * sizeof(func));
	out = (double *)malloc(numNodos * sizeof(double));
	F_W = (double **)malloc(numNodos * sizeof(double));
	pesos = (double **)malloc(numNodos * sizeof(double));
	einicial = (double *)malloc(predes.n_nodos_salida * sizeof(double));

	if(transf == NULL || out == NULL || F_W == NULL || pesos == NULL ||
	   einicial == NULL)
		error(RES_MEM);

	for(i = 0; i < numNodos; i++) {
		F_W[i] = (double *)malloc(numNodos * sizeof(double));
		pesos[i] = (double *)malloc(numNodos * sizeof(double))
		if(F_W[i] == NULL || pesos[i] == NULL)
			error(RES_MEM);

		for(j = 0; j < numNodos; j++) {
			F_W[i][j] = 0.0;
			pesos[i][j] = 0.0;
		}
	}

	for(i = 0; i < pnodulos.nodulos[numNodulo]->n_nodos; i++)
		transf[i] = pnodulos.nodulos[numNodulo]->transf[i];

	for(;i < pnodulos.nodulos[numNodulo]->n_nodos + predes.n_nodos_salida; i++)
		transf[i] = net_transf;

	/* Backpropagation. */
	for(iter = 0; iter < iteraciones; iter++) {
		/* We pick a pattern. */
		patron = random() % n_patrones;
		generarSalidaNodulo(entrada[patron], numNodulo, patron, out);

		/* We assign the weight to the weights matrix*/
		for(j = 0; j < predes.n_nodos_salida; j++)
			einicial[j] = out[j + predes.n_nodos_entrada + pnodulos.nodulos[numNodulo]->n_nodos] -
				salida[patron][j];

		for(i = 0; i < pnodulos.nodulos[numNodulo]->n_nodos; i++)
			for(j = 0; j < predes.n_nodos_entrada; j++)
				pesos[i + predes.n_nodos_entrada][j] =
					pnodulos.nodulos[numNodulo]->pesos_entrada[j][i];

		for(i = 0; i < predes.n_nodos_salida; i++)
			for(j = 0; j < pnodulos.nodulos[numNodulo]->n_nodos; j++)
				pesos[i + predes.n_nodos_entrada + pnodulos.nodulos[numNodulo]->n_nodos][j + predes.n_nodos_entrada] =
					pnodulos.nodulos[numNodulo]->pesos_salida[j][i];

		/* Obtain the weight change. */
		cambioPesos(einicial, pesos, out, F_W, pnodulos.nodulos[numNodulo]->n_nodos,
					transf);

		/* We update the weigths. */
		for(i = 0; i < pnodulos.nodulos[numNodulo]->n_nodos; i++)
			for(j = 0; j < predes.n_nodos_entrada; j++)
				pnodulos.nodulos[numNodulo]->pesos_entrada[j][i] -= alpharet *
					F_W[i + predes.n_nodos_entrada][j];

		for(i = 0; i < predes.n_nodos_salida; i++)
			for(j = 0; j < pnodulos.nodulos[numNodulo]->n_nodos; j++)
				pnodulos.nodulos[numNodulo]->pesos_salida[j][i] -= alpharet *
					F_W[i + predes.n_nodos_entrada + pnodulos.nodulos[numNodulo]->n_nodos][j + predes.n_nodos_entrada];
	}

	/* Free memory. */
	for(i = 0; i < numNodos; i++) {
		free(pesos[i]);
		free(F_W[i]);
	}

	free(pesos);
	free(F_W);
	free(einicial);
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
   nodos: Integer. Number of nodes at the nodule.
   einicial: Array of floats. Store the initial error of the nodule.
   F_W: Matrix of floats. Store the change to apply in each nodule weight.
   out: Array of floats. Store the output of each node of the nodule.
   pesos: Matrix of floats. Store all the connection weights of the nodule.
   transf: Array of functions. Store the transfer functions of each node of the
           nodule.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   N: Integer. Number of input and hidden nodes of the nodule.
   enodo: Array of floats. Output error of a nodule.
   ered: Array of floats. Network error in a given node.
 Return Value: None
 Calling functions:
   error(): Function to print an error message.
******************************************************************************/

void cambioPesos(double *einicial, double **pesos, double *out, double **F_W,
				 int nodos, func *transf)
{
	int i, j, N;
	double *enodo, *ered;

	/* Variable initialization. */
	N = predes.n_nodos_entrada + nodos;
	enodo = (double *)malloc((N + predes.n_nodos_salida) * sizeof(double));
	ered = (double *)malloc((N + predes.n_nodos_salida) * sizeof(double));
	if(enodo == NULL || ered == NULL)
		error(RES_MEM);

	for(i = 0; i < N; i++)
		enodo[i] = 0.0;

	for(i = 0; i < predes.n_nodos_salida; i++)
		enodo[N + i] = einicial[i];

	/* We get the updates. */
	for(i = N + predes.n_nodos_salida - 1; i >= predes.n_nodos_entrada; i--) {
		for(j = i + 1; j < N + predes.n_nodos_salida; j++)
			enodo[i] += pesos[j][i] * ered[j];

		if(transf[i - predes.n_nodos_entrada] == (func)&Logistic)
			ered[i] = enodo[i] * ptransferencia.logistic_b * out[i] *
				(1.0 - out[i] / ptransferencia.logistic_a);
		else if(transf[i - predes.n_nodos_entrada] == (func)&HyperbolicTangent)
			ered[i] = enodo[i] * (ptransferencia.htan_b / ptransferencia.htan_a) *
				(ptransferencia.htan_a - out[i]) *
				(ptransferencia.htan_a + out[i]);

		for(j = 0; j < i; j++)
			F_W[i][j] = ered[i] *out[j];
	}
}

/******************************************************************************
 File: evolucionarPoblaciones.c
 Function: enfriamientoSimulado()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Run the simulated annealing over a given nodule.
 Input Parameters:
	numNodulo: Integer. Number of nodule to run over.
 Local Variables:
	i: Integer. Counter.
	j: Integer. Counter.
	k: Integer. Counter.
	pasos: Integer. Number of iterations of the annealing.
	T: Float. Temperature of the simulated annealing.
	pesos: Array of floats. Old weights.
	apt_antigua: Float. Old nodule aptitude.
 Return Value: None
 Calling functions:
	pasoAleatorio(): Make a random step in the connection weights.
	medirCambioNodulo(): Measure the aptitude change at the nodule.
******************************************************************************/

void enfriamientoSimulado(int numNodulo)
{
	int i, j, k, pasos;
	double T, *pesos, apt_antigua;

	/* Variable initialization. */
	T = ToSA;
	pesos = NULL;
	apt_antigua = pnodulos.nodulos[numNodulo]->aptitud;

	/* Simulated annealing. */
	for(pasos = 0; pasos < iteraciones_sa; pasos++) {
		/* We keep the old weights. */
		k = 0;

		/* Input weights. */
		for(i = 0; i < predes.n_nodos_entrada; i++) {
			for(j = 0; j < pnodulos.nodulos[numNodulo]->n_nodos; j++) {
				if(pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j]) {
					pesos = (double *)realloc(pesos, (k + 1) * sizeof(double));
					pesos[k] = pnodulos.nodulos[numNodulo]->pesos_entrada[i][j];
					k++;
				}
			}
		}

		/* Output weights. */
		for(i = 0; i < pnodulos.nodulos[numNodulo]->n_nodos; i++) {
			for(j = 0; j < predes.n_nodos_salida; j++) {
				if(pnodulos.nodulos[numNodulo]->conexiones_salida[i][j]) {
					pesos = (double *)realloc(pesos, (k + 1) * sizeof(double));
					pesos[k] = pnodulos.nodulos[numNodulo]->pesos_salida[i][j];
					k++;
				}
			}
		}

		/* We make a randon step. */
		pasoAleatorio(numNodulo);

		/* We calculate the aptitude of the new nodule. */
		medirCambioNodulo(numNodulo);

		/* If the aptitude is worst we reject the change. */
		if((apt_antigua > pnodulos.nodulos[numNodulo]->aptitud) &&
		   aleatorio() < (apt_antigua - pnodulos.nodulos[numNodulo]->aptitud) * T) {
			/* We restore the old weights. */
			k = 0;

			/* Input weights. */
			for(i = 0; i < predes.n_nodos_entrada; i++) {
				for(j = 0; j < pnodulos.nodulos[numNodulo]->n_nodos; j++) {
					if(pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j]) {
						pnodulos.nodulos[numNodulo]->pesos_entrada[i][j] = pesos[k];
						k++;
					}
				}
			}

			/* Output weights. */
			for(i = 0; i < pnodulos.nodulos[numNodulo]->n_nodos; i++) {
				for(j = 0; j < predes.n_nodos_salida; j++) {
					if(pnodulos.nodulos[numNodulo]->conexiones_salida[i][j]) {
						pnodulos.nodulos[numNodulo]->pesos_salida[i][j] = pesos[k];
						k++;
					}
				}
			}

			/* We recalculate the aptitude of the nodule. */
			medirCambioNodulo(numNodulo);
		} else
			apt_antigua = pnodulos.nodulos[numNodulo]->aptitud;

		/* We update the aptitude temperature. */
		T = alphasa * T;
	}

	free(pesos);
}

/******************************************************************************
 File: evolucionarPoblaciones.c
 Function: pasoAleatorio()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Make a randon step in each nodule weight.
 Input Parameters:
	numNodulo: Integer. Number of the nodule to work with.
 Local Variables:
	i: Integer. Counter.
	j: Integer. Counter.
 Return Value: None
 Calling Functions:
	Normal(): Generate a randon value following a normal distribution.
******************************************************************************/

void pasoAleatorio(int numNodulo)
{
	int i, j;

	/* Randon step in input connection weights. */
	for(i = 0; i < predes.n_nodos_entrada; i++)
		for(j = 0; j < pnodulos.nodulos[numNodulo]->n_nodos; j++)
			if(pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j])
				pnodulos.nodulos[numNodulo]->pesos_entrada[i][j] +=
					Normal(0.0, 1.0);

	/* Random step in output connection weights.  */
	for(i = 0; i < pnodulos.nodulos[numNodulo]->n_nodos; i++)
		for(j = 0; j < predes.n_nodos_salida; j++)
			if(pnodulos.nodulos[numNodulo]->conexiones_salida[i][j] == 1)
				pnodulos.nodulos[numNodulo]->pesos_salida[i][j] +=
					Normal(0.0, 1.0);
}
