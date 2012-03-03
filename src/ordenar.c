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

/******************************************************************************
 File: ordenar.c
 Funtion: ordenarNodulos()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Order the nodules by aptitude.
 Input Parameters: None
 Local Variables
   i: Integer. Counter.
   j: Integer. Counter.
   aux: Nodulo. Auxiliar variable to change the order of the nodules.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void ordenarNodulos()
{
	int i, j;
	nodulo *aux;

	/* We order the nodules and copy them to the new population. */
	for(i = num_nodulos * (pnodulos.n_subpobl - 1); i < pnodulos.n_nodulos; i++) {
		for(j = i; j < pnodulos.n_nodulos; j++) {
			if(pnodulos.nodulos[i]->aptitud < pnodulos.nodulos[j]->aptitud) {
				aux = pnodulos.nodulos[i];
				pnodulos.nodulos[i] = pnodulos.nodulos[j];
				pnodulos.nodulos[j] = aux;
			} /* end if */

			pnodulos.nodulos[i]->id = i;
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
	for(i = 0; i < predes.n_redes; i++) {
		for(j = i; j < predes.n_redes; j++) {
			if(predes.redes[i]->aptitud < predes.redes[j]->aptitud) {
				aux = predes.redes[i];
				predes.redes[i] = predes.redes[j];
				predes.redes[j] = aux;
			} /* end if */
		} /* end for */
	} /* end for */
}
