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
 File: medirCambio.c
 Function: medirCambio()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Measure the change in the average aptitude of the network
              population.
 Input Parameters:
   aptitudRedes: Float. Previois average aptitude of the network population.
   iteracion: Integer. Number of evolutions of the network population.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   aptitudTemp: Float. New average aptitude of the network population.
   dif: Float. Difference between the two average aptitudes.
 Return Value: True if the difference between average aptitudes is smaller than
               the evolution limit.
 Calling Functions:
   generarSalidaNodulo(): Generate the nodule ouput depending on the entry data.
   generarSalidaRed(): Generate the network output depending on the nodules
                       output.
   medirAptitudRed(): Measure the network aptitude depending on its outputs.
   medirAptitudNodulos(): Measure the nodule aptitude given three parameters.
   normalizarAptitudNodulos(): Normalize the minimal aptitude to 0.
******************************************************************************/

bool medirCambio(double *aptitudRedes, int iteracion)
{
	int i, j;
	double aptitudTemp, dif;

	aptitudTemp = 0;
	/* We measure the networks to check its aptitude. */
	if(iteracion > 0) {
		fprintf(stderr,"Training\n");

		/* We first train the networks to have an initial value of the networks
		   and nodules aptitude. */
		for(i = 0; i < n_entrenamiento; i++) {
			/* We generate the nodule output. */
			for(j = nodulePopulation.n_nodulos - numNodules; j < nodulePopulation.n_nodulos; j++)
				generarSalidaNodulo(entrada[i], j, i, NULL);

			/* We measure the network aptitude. */
			for(j = 0; j < netPopulation.n_redes; j++) {
				generarSalidaRed(j, i);
				medirAptitudRed(salida[i], j);
			}
		}

		/* We normalize the networks aptitude. */
		for(i = 0; i < netPopulation.n_redes; i++)
			netPopulation.redes[i]->aptitud /= n_entrenamiento;

		/* We measure the nodules aptitude. */
		for(i = 0; i < nodulePopulation.n_nodulos; i++)
			medirAptitudNodulos(i);

		/* We normalize the nodules aptitude. */
		normalizarAptitudNodulos();
		aptitudTemp = 0.0;

		/* We measure the average aptitude of all the networks. */
		for(i = 0; i < netPopulation.n_redes; i++)
			aptitudTemp += netPopulation.redes[i]->aptitud;
		aptitudTemp /= netPopulation.n_redes;
	}

	/* If it's the first iteration or the difference between the previous ant the
	   current average aptitude exceed a limit the evolution continues. */
	if(iteracion == 0) {
		*aptitudRedes = 0.0;
		return false;
	} else {
		dif = aptitudTemp - *aptitudRedes;
		*aptitudRedes = aptitudTemp;
		if(dif > evolim)
			return false;
		return true;
	}
}

/*******************************************************************************
 File: medirCambio.c
 Function: medirCambioNodulo()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Measure the aptitude change suffer by a given nodule.
 Input Parameters:
 	nodule: Integer. Number of nodule to work with.
 Local Variables:
 	i: Integer. Counter.
 	j: Integer. Counter.
 Return Value: None
 Calling Functions:
	ordenarRedes(): Sort the networks by aptitude.
 	generarSalidaNodulo(): Generate the nodule output from its input data.
 	generarSalidaRed(): Generate the network output from its nodule output.
 	medirAptitudRed(): Measure the network aptitude from the generated outputs.
 	medirAptitudNodulo(): Measure the nodule aptitude from several parameters.
	normalizarAptitudNodulos(): Normalize the nodule aptitude.
******************************************************************************/

void medirCambioNodulo(int nodule)
{
	int i, j;

	for(i = 0; i < netPopulation.n_redes; i++) {
		/* We train the networks of the modified nodule. */
		if(netPopulation.redes[i]->nodulos[nodulePopulation.n_subpobl - 1] == nodulePopulation.nodulos[nodule]) {
			netPopulation.redes[i]->aptitud = 0;

			/* We calculate the networks aptitude. */
			for(j = 0; j < n_entrenamiento; j++) {
				generarSalidaNodulo(entrada[j], nodule, j, NULL);
				generarSalidaRed(i, j);
				medirAptitudRed(salida[j], i);
			}

			/* We normalize the networks aptitude. */
			netPopulation.redes[i]->aptitud /= n_entrenamiento;
		}
	}

	/* We sort the networks by aptitude. */
	ordenarRedes();

	/* We measure the new nodule aptitude. */
	medirAptitudNodulos(nodule);
	normalizarAptitudNodulos();
}
