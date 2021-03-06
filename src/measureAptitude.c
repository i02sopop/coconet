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
 File: measureAptitude.c
 Function: measureNetworkAptitude()
 Author: Pablo �lvarez de Sotomayor Posadillo
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

void measureNetworkAptitude(double *output, int netNumber)
{
	int i;
	double max_output[2], max_file[2];

	/* Variable initialization. */
	max_output[0] = cNetPopulation.nets[netNumber]->outValues[0];
	max_file[0] = output[0];
	max_output[1] = max_file[1] = 0;

	/* We calculate the output selected. */
	for(i = 0; i < cNetPopulation.numOutputNodes; i++) {
		if(max_file[0] < output[i]) {
			max_file[0] = output[i];
			max_file[1] = i;
		} if(max_output[0] < cNetPopulation.nets[netNumber]->outValues[i]) {
			max_output[0] = cNetPopulation.nets[netNumber]->outValues[i];
			max_output[1] = i;
		}
	}

	/* We check if the selected output is the correct one. */
	if(max_file[1] == max_output[1])
		cNetPopulation.nets[netNumber]->aptitude++;
}

/******************************************************************************
 File: measureAptitude.c
 Function: measureNoduleAptitude()
 Author: Pablo �lvarez de Sotomayor Posadillo
 Description: Measure the nodule aptitude from several parameters.
 Input Parameters:
   nodule: Integer. Nodule number to measure.
 Local Variables:
   i: Integer. Counter.
   sust: Float. Partial aptitude by substitution.
   dif: Float. Partial aptitude by difference.
   bestNets: Float. Partial aptitude by the best networks in which the nodule
             takes part.
   population: Vector of networks. Copy of the network population.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
   copyNetwork(): Make a copy of the network population.
   freeNetwork(): Free the memory of a network.
   replace(): Measure the aptitude by substitution.
   differ(): Measure the aptitude by difference.
   best(): Measure the aptitude by the best networks in which the nodule
              takes part.
******************************************************************************/

void measureNoduleAptitude(int nodule)
{
	int i;
	double sust, dif, bestNets;
	network **population;
  
	population = NULL;
	if((population = (network **)malloc(cNetPopulation.numNets * sizeof(network))) == NULL)
		error(RES_MEM);

	/* We make a copy of the network population. */
	copyNetwork(cNetPopulation.nets, population);

	/* We measure the partial aptitude of the nodule by substitution. */
	sust = replace(nodule, population);
	for(i = 0; i < cNetPopulation.numNets; i++)
		freeNetwork(cNetPopulation.nets[i]);

	free(cNetPopulation.nets);
	if((cNetPopulation.nets = (network **)malloc(cNetPopulation.numNets * sizeof(network))) == NULL)
		error(RES_MEM);

	/* We restore the network population. */
	copyNetwork(population, cNetPopulation.nets);

	/* We measure the nodule partial aptitude by difference. */
	dif = differ(nodule, population);

	/* We restore the network population. */
	for(i = 0; i < cNetPopulation.numNets; i++)
		freeNetwork(cNetPopulation.nets[i]);

	free(cNetPopulation.nets);
	cNetPopulation.nets = population;
	population = NULL;
	bestNets = best(nodule);

	/* We measure the partial aptitude of the nodule. */
	/* We calculate the final aptitude by ponderating the aptitudes calculated
	   previously. */
	cNodulePopulation.nodules[nodule]->aptitude = adj.sust * sust + adj.dif * dif +
		adj.best * bestNets;
}

/******************************************************************************
 File: measureAptitude.c
 Function: copyNetwork()
 Author: Pablo �lvarez de Sotomayor Posadillo
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

void copyNetwork(network **origin, network **destination)
{
	int i, j;

	/* We make a copy of the network population. */
	for(i = 0; i < cNetPopulation.numNets; i++) {
		destination[i] = (network *)malloc(sizeof(network));
		if(destination[i] == NULL)
			error(RES_MEM);

		destination[i]->nodules = (nodule **)malloc(cNodulePopulation.numSubpops * sizeof(nodule));
		destination[i]->outValues = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
		if(destination[i]->nodules == NULL || destination[i]->outValues == NULL)
			error(RES_MEM);

		destination[i]->aptitude = origin[i]->aptitude;
		for(j = 0; j < cNodulePopulation.numSubpops; j++)
			destination[i]->nodules[j] = origin[i]->nodules[j];
		for(j = 0; j < cNetPopulation.numOutputNodes; j++)
			destination[i]->outValues[j] = origin[i]->outValues[j];
	}
}

/*******************************************************************************
 File: measureAptitude.c
 Function: differ()
 Author: Pablo �lvarez de Sotomayor Posadillo
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
   measureNetworkAptitude(): Function to measure the aptitude of a network.
   error(): Function to show an error message depending on an error number.
******************************************************************************/

double differ(int nodule, network **population)
{
	int i, j, k, l, netNumber, *netIds;
	double sum;
  
	netNumber = 0;
	sum = 0.0;
	if((netIds = (int *)malloc(maxNetworks * sizeof(int))) == NULL)
		error(RES_MEM);

	/* We store the networks in which the nodule takes part. */
	for(i = 0; i < cNetPopulation.numNets; i++)
		if(cNetPopulation.nets[i]->nodules[cNodulePopulation.numSubpops - 1] == cNodulePopulation.nodules[nodule]) {
			cNetPopulation.nets[i]->aptitude = 0;
			netIds[netNumber] = i;
			netNumber++;
		}

	for(i = 0; i < netNumber; i++) {
		for(j = 0; j < numTrain; j++) {
			/* We generate the network outputs without the partial outputs of the nodule that we are measuring. */
			for(k = 0; k < cNetPopulation.numOutputNodes; k++) {
				cNetPopulation.nets[netIds[i]]->outValues[k] = 0.0;
				for(l = 0; l < cNodulePopulation.numSubpops - 1; l++)
					cNetPopulation.nets[netIds[i]]->outValues[k] += (*(netTransf))(cNetPopulation.nets[netIds[i]]->nodules[l]->partialOutputs[j][k]);
			}

			/* We measure the networks aptitude. */
			if(cNodulePopulation.numSubpops > 1)
				measureNetworkAptitude(outputData[j], netIds[i]);
			else
				cNetPopulation.nets[netIds[i]]->aptitude = 0.0;
		}

		cNetPopulation.nets[netIds[i]]->aptitude /= numTrain;

		/* We add the difference between the network aptitude with and without the nodule. */
		sum += cNetPopulation.nets[netIds[i]]->aptitude - population[netIds[i]]->aptitude;
	}

	/* We make the average of the aptitudes calculated previously. */
	if(netNumber > 0)
		sum /= netNumber;
	free(netIds);

	return sum;
}

/******************************************************************************
 File: measureAptitude.c
 Function: best()
 Author: Pablo �lvarez de Sotomayor Posadillo
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

double best(int nodule)
{
	int i, netNumber;
	double sum;

	sum = 0;
	netNumber = 0;
  
	for(i = 0; i < cNetPopulation.numNets && netNumber < selNets; i++) {
		if(cNetPopulation.nets[i]->nodules[cNodulePopulation.numSubpops - 1] == cNodulePopulation.nodules[nodule]) {
			netNumber++;
			sum += cNetPopulation.nets[i]->aptitude;
		}
	}

	/* We make the average of the best networks in which the nodule takes part of. */
	sum /= netNumber;

	return sum;
}

/******************************************************************************
 File: measureAptitude.c
 Function: replace()
 Author: Pablo �lvarez de Sotomayor Posadillo
 Description: Mide la aptitud parcial del n�dulo por sustitci�n.
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
   generateNetOutput(): Generate the network output from an input pattern.
   meditAptitudRed(): Measure the network aptitude from the generated output.
******************************************************************************/

double replace(int nodule, network **population)
{
	int i, j, netNumber, selected;
	double sum;
  
	netNumber = 0;
	sum = 0;
	selected = selNets;
  
	for(i = 0; i < selected && i < cNetPopulation.numNets; i++) {
		if(cNetPopulation.nets[i]->nodules[cNodulePopulation.numSubpops - 1] != cNodulePopulation.nodules[nodule]) {
			/* We change the nodules of its subpopulation by the nodule to measure. */
			cNetPopulation.nets[i]->nodules[cNodulePopulation.numSubpops - 1] = cNodulePopulation.nodules[nodule];

			/* We initializate the networks aptitude to make a new measure. */
			cNetPopulation.nets[i]->aptitude = 0;

			/* We train the modified networks. */
			for(j = 0; j < numTrain; j++) {
				generateNetOutput(i, j);
				measureNetworkAptitude(outputData[j], i);
			}

			/* We normalize the networks aptitude depending on the number of entries of the training file. */
			cNetPopulation.nets[i]->aptitude /= numTrain;
			sum += cNetPopulation.nets[i]->aptitude - population[i]->aptitude;
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
 File: measureAptitude.c
 Function: normalizeNoduleAptitude()
 Author: Pablo �lvarez de Sotomayor Posadillo
 Description: Normalize the nodule aptitude by setting the minimal aptitude to
              0.
 Input Parameters: None
 Local Variables:
   i: Integer. Counter.
   min: Float. Min aptitude.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void normalizeNoduleAptitude()
{
	int i;
	double min;
  
	/* We calculate the minimal value of the nodules aptitude. */
	min = 0;
	for(i = 0; i < cNodulePopulation.numNodules; i++)
		if(min > cNodulePopulation.nodules[i]->aptitude)
			min = cNodulePopulation.nodules[i]->aptitude;

	/* We set the minimal nodule aptitude to 0. */
	if(min < 0)
		for(i = 0; i < cNodulePopulation.numNodules; i++)
			cNodulePopulation.nodules[i]->aptitude -= min;
}
