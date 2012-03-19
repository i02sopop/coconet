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
 File: medirAptitud.c
 Function: medirAptitudRed()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Measure the network aptitude from its outputs.
 Input Parameters:
   output: Float vector. Output of the network.
   netNumber: Integer. Network number to work with.
 Local Variables:
   i: Integer. Counter.
   max_output: Float vector. Output selected by the network and its
               corresponding value.
   max_file: Float vector. Valid output according to the input data and its
             corresponding value.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void medirAptitudRed(double *output, int netNumber)
{
	int i;
	double max_output[2], max_file[2];

	/* Variable initialization. */
	max_output[0] = netPopulation.redes[netNumber]->valores_salida[0];
	max_file[0] = output[0];
	max_output[1] = max_file[1] = 0;

	/* We calculate the output selected. */
	for(i = 0; i < netPopulation.n_nodos_salida; i++) {
		if(max_file[0] < output[i]) {
			max_file[0] = output[i];
			max_file[1] = i;
		} if(max_output[0] < netPopulation.redes[netNumber]->valores_salida[i]) {
			max_output[0] = netPopulation.redes[netNumber]->valores_salida[i];
			max_output[1] = i;
		}
	}

	/* We check if the selected output is the correct one. */
	if(max_file[1] == max_output[1])
		netPopulation.redes[netNumber]->aptitud++;
}

/******************************************************************************
 File: medirAptitud.c
 Function: medirAptitudNodulos()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Measure the nodule aptitude from several parameters.
 Input Parameters:
   nodule: Integer. Nodule number to measure.
 Local Variables:
   i: Integer. Counter.
   sust: Float. Partial aptitude by substitution.
   dif: Float. Partial aptitude by difference.
   best: Float. Partial aptitude by the best networks in which the nodule takes
         part.
   population: Vector of networks. Copy of the network population.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
   copiarRed(): Make a copy of the network population.
   liberarRed(): Free the memory of a network.
   sustitucion(): Measure the aptitude by substitution.
   diferencia(): Measure the aptitude by difference.
   mejores(): Measure the aptitude by the best networks in which the nodule
              takes part.
******************************************************************************/

void medirAptitudNodulos(int nodule)
{
	int i;
	double sust, dif, best;
	red **population;
  
	population = NULL;
	if((population = (red **)malloc(netPopulation.n_redes * sizeof(red))) == NULL)
		error(RES_MEM);

	/* We make a copy of the network population. */
	populationrRed(netPopulation.redes, population);

	/* We measure the partial aptitude of the nodule by substitution. */
	sust = sustitucion(nodule, population);
	for(i = 0; i < netPopulation.n_redes; i++)
		liberarRed(netPopulation.redes[i]);

	free(netPopulation.redes);
	if((netPopulation.redes = (red **)malloc(netPopulation.n_redes * sizeof(red))) == NULL)
		error(RES_MEM);

	/* We restore the network population. */
	populationrRed(population, netPopulation.redes);

	/* We measure the nodule partial aptitude by difference. */
	dif = diferencia(nodule, population);

	/* We restore the network population. */
	for(i = 0; i < netPopulation.n_redes; i++)
		liberarRed(netPopulation.redes[i]);

	free(netPopulation.redes);
	netPopulation.redes = population;
	population = NULL;
	best = mejores(nodule);

	/* We measure the partial aptitude of the nodule. */
	/* We calculate the final aptitude by ponderating the aptitudes calculated
	   previously. */
	nodulePopulation.nodulos[nodule]->aptitude = pond.sust * sust + pond.dif * dif +
		pond.best * best;
}

/******************************************************************************
 File: medirAptitud.c
 Function: copiarRed()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Make a copy of the network population.
 Input Parameters:
   origin: Vector of networks. Population of origin.
   destination: Vector of networks. Population of destination.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
******************************************************************************/

void copiarRed(red **origin, red **destination)
{
	int i, j;

	/* We make a copy of the network population. */
	for(i = 0; i < netPopulation.n_redes; i++) {
		destination[i] = (red *)malloc(sizeof(red));
		if(destination[i] == NULL)
			error(RES_MEM);

		destination[i]->nodulos = (nodule **)malloc(nodulePopulation.n_subpobl * sizeof(nodule));
		destination[i]->valores_salida = (double *)malloc(netPopulation.n_nodos_salida * sizeof(double));
		if(destination[i]->nodulos == NULL || destination[i]->valores_salida == NULL)
			error(RES_MEM);

		destination[i]->aptitud = origin[i]->aptitud;
		for(j = 0; j < nodulePopulation.n_subpobl; j++)
			destination[i]->nodulos[j] = origin[i]->nodulos[j];
		for(j = 0; j < netPopulation.n_nodos_salida; j++)
			destination[i]->valores_salida[j] = origin[i]->valores_salida[j];
	}
}

/*******************************************************************************
 File: medirAptitud.c
 Function: diferencia()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Measure the partial output of a nodule by difference.
 Input Parameters:
 	nodule: Integer. Number of nodule to work with.
 	population: Vector of networks. Copy of the network population.
 Local Variables:
 	i: Integer. Counter.
 	j: Integer. Counter.
 	k: Integer. Counter.
 	l: Integer. Counter.
 	netNumber: Integer. Number of network in which the nodule takes part.
 	netIds: Vector of integers. Network ids in which the nodule takes part.
 Return Value: Float. Partial output of the nodule.
 Calling Functions:
   medirAptitudRed(): Function to measure the aptitude of a network.
   error(): Function to show an error message depending on an error number.
******************************************************************************/

double diferencia(int nodule, red **population)
{
	int i, j, k, l, netNumber, *netIds;
	double sum;
  
	netNumber = 0;
	sum = 0.0;
	if((netIds = (int *)malloc(max_netIds * sizeof(int))) == NULL)
		error(RES_MEM);

	/* We store the networks in which the nodule takes part. */
	for(i = 0; i < pnetIds.n_netIds; i++)
		if(pnetIds.netIds[i]->nodulos[nodulePopulation.n_subpobl - 1] == nodulePopulation.nodulos[nodule]) {
			pnetIds.netIds[i]->aptitud = 0;
			netIds[netNumber] = i;
			netNumber++;
		}

	for(i = 0; i < netNumber; i++) {
		for(j = 0; j < n_entrenamiento; j++) {
			/* We generate the network outputs without the partial outputs of the nodule that we are measuring. */
			for(k = 0; k < pnetIds.n_nodos_salida; k++) {
				pnetIds.netIds[netIds[i]]->valores_salida[k] = 0.0;
				for(l = 0; l < nodulePopulation.n_subpobl - 1; l++)
					pnetIds.netIds[netIds[i]]->valores_salida[k] += (*(net_transf))(pnetIds.netIds[netIds[i]]->nodulos[l]->partialOutputs[j][k]);
			}

			/* We measure the networks aptitude. */
			if(nodulePopulation.n_subpobl > 1)
				medirAptitudRed(salida[j], netIds[i]);
			else
				pnetIds.netIds[netIds[i]]->aptitud = 0.0;
		}

		pnetIds.netIds[netIds[i]]->aptitud /= n_entrenamiento;

		/* We add the difference between the network aptitude with and without the nodule. */
		sum += pnetIds.netIds[netIds[i]]->aptitud - population[netIds[i]]->aptitud;
	}

	/* We make the average of the aptitudes calculated previously. */
	if(netNumber > 0)
		sum /= netNumber;
	free(netIds);

	return sum;
}

/******************************************************************************
 File: medirAptitud.c
 Function: mejores()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Measure the aptitude of a given nodule by the best networks it
              takes part of.
 Input Parameters:
 	nodule: Integer. Nodule number to measure the aptitude.
 Local Variables:
 	i: Integer. Counter.
 	netNumber: Integer. Number of networks the nodule take part of.
 Return Value: Float. Partial aptitude of the node.
 Calling Functions: None
******************************************************************************/

double mejores(int nodule)
{
	int i, netNumber;
	double sum;

	sum = 0;
	netNumber = 0;
  
	for(i = 0; i < netPopulation.n_redes && netNumber < redsel; i++) {
		if(netPopulation.redes[i]->nodulos[nodulePopulation.n_subpobl - 1] == nodulePopulation.nodulos[nodule]) {
			netNumber++;
			sum += netPopulation.redes[i]->aptitud;
		}
	}

	/* We make the average of the best networks in which the nodule takes part of. */
	if(sum > 0)
		sum /= netNumber;

	return sum;
}

/******************************************************************************
 File: medirAptitud.c
 Function: sustitucion()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Mide la aptitud parcial del nódulo por sustitción.
 Input Parameters:
   nodule: Integer. Number of nodule to work with.
   population: Vector of networks. Copy of the network population.
 Local Variables:
   i: Integer. Counter
   j: Integer. Counter.
   netNumber: Integer. Number of networks the nodule takes part of.
   selected: Integer. Number of networks selected.
 Return Value: Float. Partial aptitude of the nodule.
 Calling Functions:
   generarSalidaRed(): Generate the network output from an input pattern.
   meditAptitudRed(): Measure the network aptitude from the generated output.
******************************************************************************/

double sustitucion(int nodule, red **population)
{
	int i, j, netNumber, selected;
	double sum;
  
	netNumber = 0;
	sum = 0;
	selected = redsel;
  
	for(i = 0; i < selected && i < netPopulation.n_redes; i++) {
		if(netPopulation.redes[i]->nodulos[nodulePopulation.n_subpobl-1] != nodulePopulation.nodulos[nodule]) {
			/* We change the nodules of its subpopulation by the nodule to measure. */
			netPopulation.redes[i]->nodulos[nodulePopulation.n_subpobl-1] = nodulePopulation.nodulos[nodule];

			/* We initializate the networks aptitude to make a new measure. */
			netPopulation.redes[i]->aptitud = 0;

			/* We train the modified networks. */
			for(j = 0; j < n_entrenamiento; j++) {
				generarSalidaRed(i, j);
				medirAptitudRed(salida[j], i);
			}

			/* We normalize the networks aptitude depending on the number of entries of the training file. */
			netPopulation.redes[i]->aptitud /= n_entrenamiento;
			sum += netPopulation.redes[i]->aptitud - population[i]->aptitud;
		} else {
			/* We add the network number to the vector. */
			netNumber++;
			selected++;
		}
	}

	/* We return the average difference of the networks aptitude that have been modified. */
	sum /= (selected - netNumber);
  
	return sum;
}

/******************************************************************************
 File: medirAptitud.c
 Function: normalizarAptitudNodulos()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Normalize the nodule aptitude by setting the minimal aptitude to
              0.
 Input Parameters: None
 Local Variables:
   i: Integer. Counter.
   min: Float. Min aptitude.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void normalizarAptitudNodulos()
{
	int i;
	double min;
  
	/* We calculate the minimal value of the nodules aptitude. */
	min = 0;
	for(i = 0; i < nodulePopulation.n_nodulos; i++)
		if(min > nodulePopulation.nodulos[i]->aptitude)
			min = nodulePopulation.nodulos[i]->aptitude;

	/* We set the minimal nodule aptitude to 0. */
	if(min < 0)
		for(i = 0; i < nodulePopulation.n_nodulos; i++)
			nodulePopulation.nodulos[i]->aptitude -= min;
}
