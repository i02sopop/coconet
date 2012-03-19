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
 File: ordenar.c
 Funtion: ordenarNodulos()
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

void ordenarNodulos()
{
	int i, j;
	nodule *aux;

	/* We order the nodules and copy them to the new population. */
	for(i = numNodules * (nodulePopulation.n_subpobl - 1); i < nodulePopulation.n_nodulos; i++) {
		for(j = i; j < nodulePopulation.n_nodulos; j++) {
			if(nodulePopulation.nodulos[i]->aptitude < nodulePopulation.nodulos[j]->aptitude) {
				aux = nodulePopulation.nodulos[i];
				nodulePopulation.nodulos[i] = nodulePopulation.nodulos[j];
				nodulePopulation.nodulos[j] = aux;
			} /* end if */

			nodulePopulation.nodulos[i]->id = i;
		} /* end for */
	} /* end for */
}

/******************************************************************************
 File: ordenar.c
 Function: ordenarRedes()
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

void ordenarRedes()
{
	int i,j;
	red *aux;

	/* We order the networks from higher to lower aptitude. */
	for(i = 0; i < netPopulation.n_redes; i++) {
		for(j = i; j < netPopulation.n_redes; j++) {
			if(netPopulation.redes[i]->aptitud < netPopulation.redes[j]->aptitud) {
				aux = netPopulation.redes[i];
				netPopulation.redes[i] = netPopulation.redes[j];
				netPopulation.redes[j] = aux;
			} /* end if */
		} /* end for */
	} /* end for */
}
