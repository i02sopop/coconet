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
#include <time.h>

/******************************************************************************
 File: ficheros.c
 Function: cargarFichero()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Load the input data from training and configuration files.
 Input Parameters:
   config: String. Name of the configuration file.
   training: String. Name of the training file.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   configFile: Pointer to the configuration file.
   trainingFile: Pointer to the training file.
 Return Value: None
 Calling Functions:
   error(): Function to print an error message.
******************************************************************************/

void cargarFichero(char *config, char *training)
{
	int i, j;
	FILE *configFile, *trainingFile;

	pnodulos.n_subpobl = 0;

	/* We open the configuration and training files. */
	configFile = fopen(config, "r");
	trainingFile = fopen(training, "r");
	if(configFile == NULL || trainingFile == NULL)
		error(IO);

	if(fscanf(trainingFile, "$ %d\n", &n_entrenamiento) == EOF ||
	   fscanf(trainingFile, "$ %d\n", &predes.n_nodos_entrada) == EOF ||
	   fscanf(trainingFile,"$ %d\n",&predes.n_nodos_salida) == EOF)
		error(IO);

	/* Loading the training data. */
	entrada = (double **)malloc(n_entrenamiento * sizeof(double));
	salida = (double **)malloc(n_entrenamiento * sizeof(double));
	if(entrada == NULL || salida == NULL)
		error(RES_MEM);

	for(i = 0; i < n_entrenamiento; i++) {
		entrada[i] = (double *)malloc(predes.n_nodos_entrada * sizeof(double));
		salida[i] = (double *)malloc(predes.n_nodos_salida * sizeof(double));
		if(entrada[i] == NULL || salida[i] == NULL)
			error(RES_MEM);

		for(j = 0; j < predes.n_nodos_entrada; j++)
			if(fscanf(trainingFile, "%lf", &entrada[i][j]) == EOF)
				error(IO);

		for(j = 0; j < predes.n_nodos_salida; j++)
			if(fscanf(trainingFile, "%lf", &salida[i][j]) == EOF)
				error(IO);
	}

	if(fscanf(configFile, "Redes: %d\n", &max_redes) == EOF ||
	   fscanf(configFile, "Nodos: %d\n", &max_nodos) == EOF ||
	   fscanf(configFile, "Nodulos: %d\n", &num_nodulos) == EOF)
		error(IO);

	/* Parameters of the transfer functions. */
	if(fscanf(configFile, "Htan a: %lf\n", &ptransferencia.htan_a) == EOF ||
	   fscanf(configFile, "Htan b: %lf\n", &ptransferencia.htan_b) == EOF ||
	   fscanf(configFile, "Logistic a: %lf\n", &ptransferencia.logistic_a) == EOF ||
	   fscanf(configFile, "Logistic b: %lf\n", &ptransferencia.logistic_b) == EOF ||
	   fscanf(configFile, "Epsilon: %lf\n", &ptransferencia.epsilon) == EOF)
		error(IO);

	/* Ponderation for nodule aptitude. */
	if(fscanf(configFile, "Sustitucion: %lf\n", &pond.sust) == EOF ||
	   fscanf(configFile, "Diferencia: %lf\n", &pond.dif) == EOF ||
	   fscanf(configFile, "Mejores: %lf\n", &pond.best) == EOF)
		error(IO);

	if(fscanf(configFile, "Redes Seleccionadas: %d\n", &redsel) == EOF ||
	   fscanf(configFile, "Nodulos Seleccionados: %d\n", &nodsel) == EOF)
		error(IO);

	if(fscanf(configFile, "Iteraciones SA: %d\n", &iteraciones_sa) == EOF ||
	   fscanf(configFile, "To SA: %lf\n", &ToSA) == EOF ||
	   fscanf(configFile, "alpha SA: %lf\n", &alphasa) == EOF)
		error(IO);

	if(fscanf(configFile, "Delta Minimo: %d\n", &delta_min) == EOF ||
	   fscanf(configFile, "Delta Maximo: %d\n", &delta_max) == EOF)
		error(IO);

	if(fscanf(configFile, "Limite Evolucion: %lf\n", &evolim) == EOF ||
	   fscanf(configFile, "Alpha Retropropagacion: %lf\n", &alpharet) == EOF)
		error(IO);

	if(fclose(configFile) == EOF || fclose(trainingFile) == EOF)
		error(IO);

	srandom(time(NULL));
	if(random() % 2 == 0)
		net_transf=(func)&HyperbolicTangent;
	else
		net_transf = (func)&Logistic;

	/* The max number of networks is as the max number of nodules per subpopulation. */
	if(max_redes < num_nodulos)
		max_redes = num_nodulos;

	if(nodsel > num_nodulos)
		nodsel = num_nodulos;
}

/******************************************************************************
 File: ficheros.c
 Function: leerGeneralizacion()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Read the input data to measure the generalization.
 Input Parameters:
   filename: String. Name of the generalization file.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   file: Pointer to file. File that contains the input data.
 Return Value: None
 Calling Functions:
   error(): Function to print an error message.
******************************************************************************/

void leerGeneralizacion(char *filename)
{
	int i, j;
	FILE *file;

	/* We open the generalization file. */
	if((file = fopen(filename, "r")) == NULL)
		error(IO);

	/* We load the generalization data. */
	if(fscanf(file, "$ %d\n", &n_generalizacion) == EOF ||
	   fscanf(file, "$ %d\n", &predes.n_nodos_entrada) == EOF ||
	   fscanf(file, "$ %d\n", &predes.n_nodos_salida) == EOF)
		error(IO);

	for(i = 0; i < n_entrenamiento; i++) {
		free(entrada[i]);
		free(salida[i]);
	}
	free(entrada);
	free(salida);

	entrada = (double **)malloc(n_generalizacion * sizeof(double));
	salida = (double **)malloc(n_generalizacion * sizeof(double));
	if(entrada == NULL || salida == NULL)
		error(RES_MEM);

	for(i = 0; i < n_generalizacion; i++) {
		entrada[i] = (double *)malloc(predes.n_nodos_entrada * sizeof(double));
		salida[i] = (double *)malloc(predes.n_nodos_salida * sizeof(double));
		if(entrada[i] == NULL || salida[i] == NULL)
			error(RES_MEM);

		for(j = 0; j < predes.n_nodos_entrada; j++)
			if(fscanf(file, "%lf", &entrada[i][j]) == EOF)
				error(IO);

		for(j = 0; j < predes.n_nodos_salida; j++)
			if(fscanf(file, "%lf", &salida[i][j]) == EOF)
				error(IO);
	}

	/* We close the file. */
	if(fclose(file) == EOF)
		error(IO);
}

/******************************************************************************
 File: ficheros.c
 Function: exportarMejorRed()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Export the network who fits best to the problem to an output
              file.
 Input Parameters:
   filename: String. Name of the output file.
 Local Variables:
   i: Integer. Counter
   j: Integer. Counter.
   k: Integer. Counter
   l: Integer. Counter.
   nodes: Integer. Store the number of nodes at the network.
   max: Float. Maximum generalization.
   idmax: Integer. Network id with maximum generalization.
   connections: Integer. Number of connections of the network.
   aptitud: Float. Store the aptitude to measure the generalization.
   out: Pointer to file. Output file
 Return Value: None
 Calling Functions:
   generarSalidaRed(): Generate the network output from the nodules output.
   generarSalidaNodulo(): Generate the nodule output from an input parameter.
   medirAptitudRed(): Measure the net aptitude from the generated outputs.
   error(): Function to print an error message.
******************************************************************************/

void exportarMejorRed(char *filename)
{
	int i, j, k, l, nodes, idmax, connections;
	double *aptitud, max;
	FILE *out;

	if((aptitud = (double *)malloc(5 * sizeof(double))) == NULL)
		error(RES_MEM);

	for(i = 0; i < pnodulos.n_nodulos; i++) {
		for(j = 0; j < n_entrenamiento; j++)
			free(pnodulos.nodulos[i]->salidas_parciales[j]);
		free(pnodulos.nodulos[i]->salidas_parciales);

		pnodulos.nodulos[i]->salidas_parciales = (double **)malloc(n_generalizacion * sizeof(double));
		if(pnodulos.nodulos[i]->salidas_parciales == NULL)
			error(RES_MEM);

		for(j = 0; j < n_generalizacion; j++){
			pnodulos.nodulos[i]->salidas_parciales[j] = (double *)malloc(predes.n_nodos_salida * sizeof(double));
			if(pnodulos.nodulos[i]->salidas_parciales[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < predes.n_nodos_salida; k++)
				pnodulos.nodulos[i]->salidas_parciales[j][k] = 0.0;
		}
	}

	/* We open the output file. */
	if((out = fopen(filename,"w")) == NULL)
		error(IO);

	/* We store the aptitude of the selected networks. */
	for(i = 0; i < 5; i++){
		aptitud[i] = predes.redes[i]->aptitud;
		predes.redes[i]->aptitud = 0.0;
	}

	/* We calculate the network generalization. */
	for(i = 0; i < n_generalizacion; i++) {
		/* We generate the nodule partial outputs from the generaliation data. */
		for(j = 0; j < pnodulos.n_nodulos; j++)
			generarSalidaNodulo(entrada[i], j, i, NULL);

		for(j = 0; j < 5; j++) {
			/* We generate the network outputs from the nodules outputs. */
			generarSalidaRed(j, i);

			/* We measure the network aptitude from the generated outputs. */
			medirAptitudRed(salida[i], j);
		}
	}

	/* We normalize the generalization aptitude. */
	for(i = 0; i < 5; i++)
		predes.redes[i]->aptitud /= n_generalizacion;

	/*
	  We pick the network with best generalization from the 5 best by
	  aptitude. Other selection parameter is the number of nodes in the
	  network, the aptitude of the network and the number of connections of the
	  network.
	*/
	max = 0.0;
	idmax = 0;
	for(i = 0; i < 5; i++) {
		if(predes.redes[i]->aptitud > max) {
			max = predes.redes[i]->aptitud;
			idmax = i;
		} else if(predes.redes[i]->aptitud == max) {
			if(aptitud[idmax] < aptitud[i]) {
				max = predes.redes[i]->aptitud;
				idmax = i;
			} else if(aptitud[idmax] == aptitud[i]) {
				nodes = 0;
				for(j = 0; j < pnodulos.n_subpobl; j++)
					nodes += (predes.redes[idmax]->nodulos[j]->n_nodos -
							  predes.redes[i]->nodulos[j]->n_nodos);

				if(nodes > 0) {
					max = predes.redes[i]->aptitud;
					idmax = i;
				} else if(nodes == 0) {
					connections = 0;
					for(l = 0; l < pnodulos.n_subpobl; l++) {
						for(j = 0; j < predes.n_nodos_entrada; j++)
							for(k = 0; k < predes.redes[idmax]->nodulos[l]->n_nodos; k++)
								if(predes.redes[idmax]->nodulos[l]->conexiones_entrada[j][k] == 1)
									connections++;
						for(j = 0; j < predes.redes[idmax]->nodulos[l]->n_nodos; j++)
							for(k = 0; k < predes.n_nodos_salida; k++)
								if(predes.redes[idmax]->nodulos[l]->conexiones_salida[j][k] == 1)
									connections++;
						for(j = 0; j < predes.n_nodos_entrada; j++)
							for(k = 0; k < predes.redes[i]->nodulos[l]->n_nodos; k++)
								if(predes.redes[i]->nodulos[l]->conexiones_entrada[j][k] == 1)
									connections--;
						for(j = 0; j < predes.redes[i]->nodulos[l]->n_nodos; j++)
							for(k = 0; k < predes.n_nodos_salida; k++)
								if(predes.redes[i]->nodulos[l]->conexiones_salida[j][k] == 1)
									connections--;
					}

					if(connections > 0) {
						max = predes.redes[i]->aptitud;
						idmax = i;
					}
				}
			}
		}
	}

	/*
	  Write in an output file the aptitude, generalization and number of
	  input, hidden and output nodes of the best network and its number of
	  input and output connections.
	*/
	if(fprintf(out, "Aptitude: %lf\n", aptitud[idmax]) == EOF ||
	   fprintf(out, "Generalization: %lf\n", predes.redes[idmax]->aptitud) == EOF ||
	   fprintf(out, "Number of Nodules: %d\n", pnodulos.n_subpobl) == EOF ||
	   fprintf(out, "Number of Input Nodes: %d\n", predes.n_nodos_entrada) == EOF)
		error(IO);

	nodes = 0;
	for(i = 0; i < pnodulos.n_subpobl; i++)
		nodes += predes.redes[idmax]->nodulos[i]->n_nodos;

	if(fprintf(out, "Number of Hidden Nodes: %d\n", nodes) == EOF ||
	   fprintf(out, "Number of Output Nodes: %d\n", predes.n_nodos_salida) == EOF)
		error(IO);

	connections = 0;
	for(i = 0; i < pnodulos.n_subpobl; i++)
		for(j = 0; j < predes.n_nodos_entrada; j++)
			for(k = 0; k < predes.redes[idmax]->nodulos[i]->n_nodos; k++)
				if(predes.redes[idmax]->nodulos[i]->conexiones_entrada[j][k] == 1)
					connections++;

	if(fprintf(out, "Number of Input Connections: %d\n", connections) == EOF)
		error(IO);

	connections = 0;
	for(i = 0; i < pnodulos.n_subpobl; i++)
		for(j = 0; j < predes.redes[idmax]->nodulos[i]->n_nodos; j++)
			for(k = 0; k < predes.n_nodos_salida; k++)
				if(predes.redes[idmax]->nodulos[i]->conexiones_salida[j][k] == 1)
					connections++;

	if(fprintf(out, "Number of Output Connections: %d\n", connections) == EOF)
		error(IO);

	/* We close the output file. */
	if(fclose(out) == EOF)
		error(IO);
	free(aptitud);
}
