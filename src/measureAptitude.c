/*********************************************************************************
 * Copyright (c) 2004-2015 coconet project (see AUTHORS)                         *
 *                                                                               *
 * This file is part of Coconet.                                                 *
 *                                                                               *
 * Coconet is free software: you can redistribute it and/or modify it under the  *
 * terms of the GNU General Public License as published by the Free Software     *
 * Foundation, either version 3 of the License, or (at your option) any later    *
 * version.                                                                      *
 *                                                                               *
 * Coconet is distributed in the hope that it will be useful, but WITHOUT ANY    *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR *
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.   *
 *                                                                               *
 * You should have received a copy of the GNU General Public License along with  *
 * coconet. If not, see <http://www.gnu.org/licenses/>.                          *
 ********************************************************************************/

#include <definitions.h>

/*********************************************************************************
 * Measure the network aptitude from its outputs.                                *
 * @param double* output: Output of the network.                                 *
 * @param int netNumber: Network number to work with.                            *
 * @returns void                                                                 *
 ********************************************************************************/
void
measureNetworkAptitude(double *output, int netNumber) {
	int i;
	/* Output selected by the network and its corresponding value. */
	double max_output[2];
	/* Valid output according to the input data and its corresponding value. */
	double max_file[2];

	/* Variable initialization. */
	max_output[0] = cNetPopulation.nets[netNumber]->outValues[0];
	max_file[0] = output[0];
	max_output[1] = max_file[1] = 0;

	/* We calculate the output selected. */
	for (i = 0; i < cNetPopulation.numOutputNodes; i++) {
		if (max_file[0] < output[i]) {
			max_file[0] = output[i];
			max_file[1] = i;
		}

		if (max_output[0] < cNetPopulation.nets[netNumber]->outValues[i]) {
			max_output[0] = cNetPopulation.nets[netNumber]->outValues[i];
			max_output[1] = i;
		}
	}

	/* We check if the selected output is the correct one. */
	if(max_file[1] == max_output[1])
		cNetPopulation.nets[netNumber]->aptitude++;
}

/*********************************************************************************
 * Measure the nodule aptitude from several parameters.                          *
 * @param int nodule: Nodule number to measure.                                  *
 * @returns void                                                                 *
 ********************************************************************************/
void
measureNoduleAptitude(int nodule) {
	int i;
	double subst; /* Partial aptitude by substitution. */
	double diff; /* Partial aptitude by difference. */
	/* Partial aptitude by the best networks in which the nodule takes part. */
	double bestNets;
	network **population = NULL; /* Copy of the network population. */

	/* Variable initialization. */
	population = (network **)malloc(cNetPopulation.numNets * sizeof(network));
	if (population == NULL)
		error(RES_MEM);

	/* We make a copy of the network population. */
	copyNetwork(cNetPopulation.nets, population);

	/* We measure the partial aptitude of the nodule by substitution. */
	subst = replace(nodule, population);
	for (i = 0; i < cNetPopulation.numNets; i++)
		freeNetwork(cNetPopulation.nets[i]);

	free(cNetPopulation.nets);
	if ((cNetPopulation.nets = (network **)malloc(cNetPopulation.numNets * sizeof(network))) == NULL)
		error(RES_MEM);

	/* We restore the network population. */
	copyNetwork(population, cNetPopulation.nets);

	/* We measure the nodule partial aptitude by difference. */
	diff = differ(nodule, population);

	/* We restore the network population. */
	for (i = 0; i < cNetPopulation.numNets; i++)
		freeNetwork(cNetPopulation.nets[i]);

	free(cNetPopulation.nets);
	cNetPopulation.nets = population;
	population = NULL;
	bestNets = best(nodule);

	/* We measure the partial aptitude of the nodule. */
	/* We calculate the final aptitude by ponderating the aptitudes calculated
	   previously. */
	cNodulePopulation.nodules[nodule]->aptitude = adj.sust * subst + adj.dif * diff +
		adj.best * bestNets;
}

/*********************************************************************************
 * Make a copy of the network population.                                        *
 * @param network** origin: Population of origin.                                *
 * @param network** destination: Population of destination.                      *
 * @returns void                                                                 *
 ********************************************************************************/
void
copyNetwork(network **origin, network **destination) {
	int i, j;

	/* We make a copy of the network population. */
	for (i = 0; i < cNetPopulation.numNets; i++) {
		destination[i] = (network *)malloc(sizeof(network));
		if (destination[i] == NULL)
			error(RES_MEM);

		destination[i]->nodules = (nodule **)malloc(cNodulePopulation.numSubpops * sizeof(nodule));
		destination[i]->outValues = (double *)malloc(cNetPopulation.numOutputNodes * sizeof(double));
		if (destination[i]->nodules == NULL || destination[i]->outValues == NULL)
			error(RES_MEM);

		destination[i]->aptitude = origin[i]->aptitude;
		for (j = 0; j < cNodulePopulation.numSubpops; j++)
			destination[i]->nodules[j] = origin[i]->nodules[j];
		for (j = 0; j < cNetPopulation.numOutputNodes; j++)
			destination[i]->outValues[j] = origin[i]->outValues[j];
	}
}

/*********************************************************************************
 * Measure the partial output of a nodule by difference.                         *
 * @param int nodule: Number of nodule to work with.                             *
 * @param network** population: Copy of the network population.                  *
 * @returns Double. Partial output of the nodule.                                *
 ********************************************************************************/
double
differ(int nodule, network **population) {
	int i, j, k, l;
	int netNumber = 0; /* Number of network in which the nodule takes part. */
	/* Network ids in which the nodule takes part. */
	int *netIds = (int *)malloc(maxNetworks * sizeof(int));
	double sum = 0.0;
  
	if (netIds == NULL)
		error(RES_MEM);

	/* We store the networks in which the nodule takes part. */
	for (i = 0; i < cNetPopulation.numNets; i++)
		if (cNetPopulation.nets[i]->nodules[cNodulePopulation.numSubpops - 1] == cNodulePopulation.nodules[nodule]) {
			cNetPopulation.nets[i]->aptitude = 0;
			netIds[netNumber] = i;
			netNumber++;
		}

	for (i = 0; i < netNumber; i++) {
		for (j = 0; j < numTrain; j++) {
			/* We generate the network outputs without the partial outputs of
			   the nodule that we are measuring. */
			for (k = 0; k < cNetPopulation.numOutputNodes; k++) {
				cNetPopulation.nets[netIds[i]]->outValues[k] = 0.0;
				for (l = 0; l < cNodulePopulation.numSubpops - 1; l++)
					cNetPopulation.nets[netIds[i]]->outValues[k] += (*(netTransf))(cNetPopulation.nets[netIds[i]]->nodules[l]->partialOutputs[j][k]);
			}

			/* We measure the networks aptitude. */
			if (cNodulePopulation.numSubpops > 1)
				measureNetworkAptitude(outputData[j], netIds[i]);
			else
				cNetPopulation.nets[netIds[i]]->aptitude = 0.0;
		}

		cNetPopulation.nets[netIds[i]]->aptitude /= numTrain;

		/* We add the difference between the network aptitude with and without the nodule. */
		sum += cNetPopulation.nets[netIds[i]]->aptitude - population[netIds[i]]->aptitude;
	}

	/* We make the average of the aptitudes calculated previously. */
	if (netNumber > 0)
		sum /= netNumber;
	free(netIds);

	return sum;
}

/*********************************************************************************
 * Measure the aptitude of a given nodule by the best networks it takes part of. *
 * @param int nodule: Nodule number to measure the aptitude.                     *
 * @return Double. Partial aptitude of the node.                                 *
 ********************************************************************************/
double
best(int nodule) {
	int i;
	int netNumber = 0; /* Number of networks the nodule take part of. */
	double sum = 0.0;
  
	for (i = 0; i < cNetPopulation.numNets && netNumber < selNets; i++) {
		if (cNetPopulation.nets[i]->nodules[cNodulePopulation.numSubpops - 1] == cNodulePopulation.nodules[nodule]) {
			netNumber++;
			sum += cNetPopulation.nets[i]->aptitude;
		}
	}

	/* We make the average of the best networks in which the nodule takes part of. */
	sum /= netNumber;

	return sum;
}

/*********************************************************************************
 * Measure the nodule partial aptitude by substitution.                          *
 * @param int nodule: Number of nodule to work with.                             *
 * @param network** population: Copy of the network population.                  *
 * @return Double. Partial aptitude of the nodule.                               *
 ********************************************************************************/
double
replace(int nodule, network **population) {
	int i, j;
	int netNumber = 0; /* Number of networks the nodule takes part of. */
	int selected = selNets; /* Number of networks selected. */
	double sum = 0;
  
	for (i = 0; i < selected && i < cNetPopulation.numNets; i++) {
		if (cNetPopulation.nets[i]->nodules[cNodulePopulation.numSubpops - 1] != cNodulePopulation.nodules[nodule]) {
			/* We change the nodules of its subpopulation by the nodule to measure. */
			cNetPopulation.nets[i]->nodules[cNodulePopulation.numSubpops - 1] = cNodulePopulation.nodules[nodule];

			/* We initializate the networks aptitude to make a new measure. */
			cNetPopulation.nets[i]->aptitude = 0;

			/* We train the modified networks. */
			for (j = 0; j < numTrain; j++) {
				generateNetOutput(i, j);
				measureNetworkAptitude(outputData[j], i);
			}

			/* We normalize the networks aptitude depending on the number of
			   entries of the training file. */
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

/*********************************************************************************
 * Normalize the nodule aptitude by setting the minimal aptitude to 0.           *
 * @returns void                                                                 *
 *********************************************************************************/
void
normalizeNoduleAptitude() {
	int i;
	double min;
  
	/* We calculate the minimal value of the nodules aptitude. */
	min = 0;
	for (i = 0; i < cNodulePopulation.numNodules; i++)
		if (min > cNodulePopulation.nodules[i]->aptitude)
			min = cNodulePopulation.nodules[i]->aptitude;

	/* We set the minimal nodule aptitude to 0. */
	if (min < 0)
		for (i = 0; i < cNodulePopulation.numNodules; i++)
			cNodulePopulation.nodules[i]->aptitude -= min;
}
