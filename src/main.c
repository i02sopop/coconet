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

#include <string.h>
#include <definitions.h>

/******************************************************************************
 File: main.c
 Function: main
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Manage the program flow.
 Input Parameters:
	argc: Integer. Number of arguments of the program.
	argv: Vector of strings. Arguments passed to the program.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   netAptitude: Float. Store the average flair of the networks.
 Return Value: 0 with no errors and -1 otherwise.
 Calling Functions:
   error(): Function to show an error message depending on an error number.
   cargarFichero(): Function to load the data from config and training files.
   escalarDatosEntrada(): Scale the data from the input nodes.
   escalarDatosSalida(): Scale the data from the output nodes.
   medirCambio(): Measure the average flair change in the network population.
   crearPoblacionNodulos(): Create a new subpopulation of nodes.
   anadirNoduloRedes(): Add a new node subpopulation to the networks
                        population.
   evolucionarPoblaciones(): Evolve the networks and nodes populations.
   ordenarRedes(): Order the networks by aptitude.
   leerGeneralizacion(): Read the data to measure the network generalization.
   exportarMejorRed(): Export the best network in the generalization process.
   ajustarMatrices(): Adjust the size of the matrix used in the new nodes
				      subpopulation.
******************************************************************************/

int main(int argc, char **argv)
{
	int i;
	double netAptitude;

	/* Checking the number of arguments. */
	if(argc < 4 || (argc > 4 && (argc != 6 || strcmp(argv[4], "-o") != 0)))
		error(COMMAND);

	/* We load the config and training files. */
	cargarFichero(argv + 1, argv + 2);
	if(nodsel > numNodules)
		nodsel = numNodules;

	/* Initialization of the average flair of the networks. */
	netAptitude = 0.0;

	/* Scaling of input and output data from the training file. */
	fprintf(stderr, "Scaling the input data.\n");
	escalarDatosEntrada(n_entrenamiento);
	fprintf(stderr, "Scaling the output data.\n");
	if(net_transf == (func)&Logistic)
		escalarDatosSalida(salida,
						   n_entrenamiento,
						   0.0 + ptransferencia.epsilon,
						   ptransferencia.logistic_a - ptransferencia.epsilon);
	else
		escalarDatosSalida(salida,
						   n_entrenamiento,
						   ptransferencia.epsilon - ptransferencia.htan_a,
						   ptransferencia.htan_a - ptransferencia.epsilon);

	/* We evolve the networks and nodes ppulations. */
	for(i = 0; medirCambio(&netAptitude, i) == false; i++) {
		/* We create a new population of nodes. */
		fprintf(stderr, "Especie %d\n",i+1);
		crearPoblacionNodulos();

		/*
		  We add the new population of nodes to the previous population of
		  networks.
		*/
		fprintf(stderr,"Add nodes to networks.\n");
		anadirNodulosRedes();

		/* We evolve the populations of networks and nodes. */
		fprintf(stderr, "Evolve popularions.\n");
		evolucionarPoblaciones();

		/* We adjust the size of the last node's subpopulation matrix. */
		fprintf(stderr, "Ajust matrix\n");
		ajustarMatrices();
	}

	/* We order the networks by its flair. */
	fprintf(stderr, "Ordering networks.\n");
	ordenarRedes();

	/* We read the data of the generalization file. */
	fprintf(stderr, "Read the generalization data.\n");
	leerGeneralizacion(argv[3]);

	/* We scale the input and output data from the generalization file. */
	fprintf(stderr, "Scaling input data.\n");
	escalarDatosEntrada(n_generalizacion);
	fprintf(stderr,"Scaling output data.\n");
	if(net_transf == (func)&Logistic)
		escalarDatosSalida(salida,
						   n_generalizacion,
						   0.0 + ptransferencia.epsilon,
						   ptransferencia.logistic_a - ptransferencia.epsilon);
	else
		escalarDatosSalida(salida,
						   n_generalizacion,
						   -ptransferencia.
						   htan_a + ptransferencia.epsilon,
						   ptransferencia.htan_a - ptransferencia.epsilon);

	/* We export the best network found. */
	fprintf(stderr,"Exporting the best network.\n");
	if(argc == 6)
		exportarMejorRed(argv[5]);
	else
		exportarMejorRed("output.txt");
	fprintf(stderr,"End\n");

	return 0;
}
