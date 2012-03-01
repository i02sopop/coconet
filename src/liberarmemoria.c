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
 File: liberarMemoria.c
 Function: liberarNodulo()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Free the memory allocated by a nodule.
 Input Parameters:
 	nodule: Nodulo. Nodule to free.
 Local Variables:
 	i: Integer. Counter.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void liberarNodulo(nodulo *nodule)
{
  int i;

  /* 
	 We free the memory allocated by the matrix and vectors of the given
	 nodule.
  */
  for(i = 0; i < max_nodos; i++) {
    free(nodule->conexiones_salida[i]);
    free(nodule->pesos_salida[i]);
  } for(i = 0; i < predes.n_nodos_entrada; i++) {
    free(nodule->conexiones_entrada[i]);
    free(nodule->pesos_entrada[i]);
  } for(i = 0; i < predes.n_nodos_salida; i++)
    nodule->transf = NULL;
  for(i = 0; i < n_entrenamiento; i++)
    free(nodule->salidas_parciales[i]);

  free(nodule->conexiones_entrada);
  free(nodule->conexiones_salida);
  free(nodule->pesos_entrada);
  free(nodule->pesos_salida);
  free(nodule->transf);
  free(nodule->salidas_parciales);
}

/******************************************************************************
 File: liberarMemoria.c
 Function: ajustarMatrices()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Adjust the matriz size of the last nodule subpopulation.
 Input Parameters: None
 Local Variables:
 	i: Integer. Counter.
 	j: Integer. Counter.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void ajustarMatrices()
{
  int i, j;
  
  /* We adjust the matriz size of the last nodule subpopulation. */
  for(i = num_nodulos * (pnodulos.n_subpobl - 1); i < pnodulos.n_nodulos; i++) {
    for(j = 0; j < predes.n_nodos_entrada; j++) {
      pnodulos.nodulos[i]->conexiones_entrada[j] = (int *)realloc(pnodulos.nodulos[i]->conexiones_entrada[j], pnodulos.nodulos[i]->n_nodos * sizeof(int));
      pnodulos.nodulos[i]->pesos_entrada[j] = (double *)realloc(pnodulos.nodulos[i]->pesos_entrada[j], pnodulos.nodulos[i]->n_nodos * sizeof(double));
    }

    pnodulos.nodulos[i]->conexiones_salida = (int **)realloc(pnodulos.nodulos[i]->conexiones_salida, pnodulos.nodulos[i]->n_nodos * sizeof(int));
    pnodulos.nodulos[i]->pesos_salida = (double **)realloc(pnodulos.nodulos[i]->pesos_salida, pnodulos.nodulos[i]->n_nodos * sizeof(double));
    pnodulos.nodulos[i]->transf = (func *)realloc(pnodulos.nodulos[i]->transf, pnodulos.nodulos[i]->n_nodos * sizeof(func));
  }
}
