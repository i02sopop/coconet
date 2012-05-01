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
 File: sort.c
 Funtion: sortNodules()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Order the nodules by aptitude.
 Input Parameters: None
 Local Variables
   i: Integer. Counter.
   j: Integer. Counter.
   aux: Nodule. Auxiliar variable to change the order of the nodules.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void sortNodules()
{
	int i, j;
	nodule *aux;

	/* We order the nodules and copy them to the new population. */
	for(i = numNodules * (cNodulePopulation.numSubpops - 1); i < cNodulePopulation.numNodules; i++) {
		for(j = i; j < cNodulePopulation.numNodules; j++) {
			if(cNodulePopulation.nodules[i]->aptitude < cNodulePopulation.nodules[j]->aptitude) {
				aux = cNodulePopulation.nodules[i];
				cNodulePopulation.nodules[i] = cNodulePopulation.nodules[j];
				cNodulePopulation.nodules[j] = aux;
			} /* end if */

			cNodulePopulation.nodules[i]->id = i;
		} /* end for */
	} /* end for */
}

/******************************************************************************
 File: sort.c
 Function: sortNetworks()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Order the networks by aptitude.
 Input Parameters: None
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   aux: Red. Auxiliar variable to change the order of the networks.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void sortNetworks()
{
	int i,j;
	network *aux;

	/* We order the networks from higher to lower aptitude. */
	for(i = 0; i < cNetPopulation.numNets; i++) {
		for(j = i; j < cNetPopulation.numNets; j++) {
			if(cNetPopulation.nets[i]->aptitude < cNetPopulation.nets[j]->aptitude) {
				aux = cNetPopulation.nets[i];
				cNetPopulation.nets[i] = cNetPopulation.nets[j];
				cNetPopulation.nets[j] = aux;
			} /* end if */
		} /* end for */
	} /* end for */
}
