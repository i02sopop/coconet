/*********************************************************************************
 * Copyright (c) 2004-2016 coconet project (see AUTHORS)                         *
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
 * Measure the change in the average aptitude of the network population.         *
 * @param float networkAptitude: Previous average aptitude of the network        *
 *                               population.                                     *
 * @param int iter:  Number of evolutions of the network population.             *
 * @return True if the difference between average aptitudes is smaller than the  *
 *         evolution limit.                                                      *
 ********************************************************************************/
bool
measureChange(double *networkAptitude, int iter) {
	int i, j;
	double tmpAptitude = 0; /* New average aptitude of the network population. */
	double diff; /* Difference between the two average aptitudes. */

	/* Measure the networks to check its aptitude. */
	if (iter > 0) {
		fprintf(stderr, _("TRAINING"));

		/* First train the networks to have an initial value of the networks
		 * and nodules aptitude. */
		for (i = 0; i < numTrain; i++) {
			/* Generate the nodule output. */
			for (j = cNodulePopulation.numNodules - numNodules; j < cNodulePopulation.numNodules; j++)
				generateNoduleOutput(inputData[i], j, i, NULL);

			/* Measure the network aptitude. */
			for (j = 0; j < cNetPopulation.numNets; j++) {
				generateNetOutput(j, i);
				measureNetworkAptitude(outputData[i], j);
			}
		}

		/* Normalize the networks aptitude. */
		for (i = 0; i < cNetPopulation.numNets; i++)
			cNetPopulation.nets[i]->aptitude /= numTrain;

		/* Measure the nodules aptitude. */
		for (i = 0; i < cNodulePopulation.numNodules; i++)
			measureNoduleAptitude(i);

		/* Normalize the nodules aptitude. */
		normalizeNoduleAptitude();
		tmpAptitude = 0.0;

		/* Measure the average aptitude of all the networks. */
		for (i = 0; i < cNetPopulation.numNets; i++)
			tmpAptitude += cNetPopulation.nets[i]->aptitude;
		tmpAptitude /= cNetPopulation.numNets;
	}

	/* If it's the first iteration or the difference between the previous ant the
	 * current average aptitude exceed a limit the evolution continues. */
	if (iter == 0) {
		*networkAptitude = 0.0;
		return false;
	} else {
		diff = tmpAptitude - *networkAptitude;
		*networkAptitude = tmpAptitude;
		return (diff <= evolim);
	}
}

/*********************************************************************************
 * Measure the aptitude change suffer by a given nodule.                         *
 * @param int nodule: Number of nodule to work with.                             *
 * @returns void                                                                 *
 ********************************************************************************/
void
measureNoduleChange(int nodule) {
	int i, j;

	for (i = 0; i < cNetPopulation.numNets; i++) {
		/* Train the networks of the modified nodule. */
		if (cNetPopulation.nets[i]->nodules[cNodulePopulation.numSubpops - 1] == cNodulePopulation.nodules[nodule]) {
			cNetPopulation.nets[i]->aptitude = 0;

			/* Calculate the networks aptitude. */
			for (j = 0; j < numTrain; j++) {
				generateNoduleOutput(inputData[j], nodule, j, NULL);
				generateNetOutput(i, j);
				measureNetworkAptitude(outputData[j], i);
			}

			/* Normalize the networks aptitude. */
			cNetPopulation.nets[i]->aptitude /= numTrain;
		}
	}

	/* Sort the networks by aptitude. */
	sortNetworks();

	/* Measure the new nodule aptitude. */
	measureNoduleAptitude(nodule);
	normalizeNoduleAptitude();
}
