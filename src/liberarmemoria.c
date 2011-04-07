/********************************************************************************
* Copyright (c) 2004-2011 coconet project (see AUTHORS)			        *
*									        *
* This file is part of Coconet.						        *
*									        *
* Coconet is free software: you can redistribute it and/or modify it under the  *
* terms of the GNU General Public License as published by the Free Software     *
* Foundation, either version 3 of the License, or (at your option) any later    *
* version.                                                                      *
*									        *
* Coconet is distributed in the hope that it will be useful, but WITHOUT ANY    *
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR *
* A PARTICULAR PURPOSE.  See the GNU General Public License for more details.   *
*									        *
* You should have received a copy of the GNU General Public License along with  *
* coconet. If not, see <http://www.gnu.org/licenses/>.                          *
********************************************************************************/

#include <definiciones.h>

/*******************************************************************************
* Fichero: liberarMemoria.c						       *
*									       *
* Funcion: liberarNodulo()						       *
*									       *
* Autor: Pablo Alvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funcion: Libera la memoria ocupada por un n—dulo.	       *
*									       *
* Parametros de Entrada:						       *
* 	eliminar: Nodulo. Nodulo a eliminar.				       *
*									       *
* Parametros Internos:							       *
* 	i: Entero. Contador.						       *
*									       *
* Parametros de Salida:	NINGUNO						       *
*									       *
* Funciones a las que llama la funcion:	NINGUNA				       *
*									       *
*******************************************************************************/

void liberarNodulo(nodulo *eliminar)
{
  int i;
  
  /*Se eliminan las matrices y los vectores del nódulo dado. */
  for(i=0;i<max_nodos;i++)
  {
    free(eliminar->conexiones_salida[i]);
    free(eliminar->pesos_salida[i]);
  }
  for(i=0;i<predes.n_nodos_entrada;i++)
  {
    free(eliminar->conexiones_entrada[i]);
    free(eliminar->pesos_entrada[i]);
  }
  free(eliminar->conexiones_entrada);
  free(eliminar->conexiones_salida);
  free(eliminar->pesos_entrada);
  free(eliminar->pesos_salida);
  for(i=0;i<predes.n_nodos_salida;i++)
    eliminar->transf=NULL;
  free(eliminar->transf);
  for(i=0;i<n_entrenamiento;i++)
    free(eliminar->salidas_parciales[i]);
  free(eliminar->salidas_parciales);
}

/*******************************************************************************
* Fichero: liberarMemoria.c						       *
*									       *
* Funcion: ajustarMatrices()						       *
*									       *
* Autor: Pablo Alvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funcion: Ajusta el tamaño de las matrices que utiliza la     *
*			   ultima subpoblacion de nodulos.		       *
*									       *
* Parametros de Entrada: NINGUNO					       *
*									       *
* Parametros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
*									       *
* Parametros de Salida:	NINGUNO						       *
*									       *
* Funciones a las que llama la funcion:	NINGUNA				       *
*									       *
*******************************************************************************/

void ajustarMatrices()
{
  int i,j;
  
  /*Se ajustan las matrices y los vectores de la última subpoblación de nódulos*/
  for(i=num_nodulos*(pnodulos.n_subpobl-1);i<pnodulos.n_nodulos;i++)
  {
    for(j=0;j<predes.n_nodos_entrada;j++)
    {
      pnodulos.nodulos[i]->conexiones_entrada[j]=(int *)realloc(pnodulos.nodulos[i]->conexiones_entrada[j],pnodulos.nodulos[i]->n_nodos*sizeof(int));
      pnodulos.nodulos[i]->pesos_entrada[j]=(double *)realloc(pnodulos.nodulos[i]->pesos_entrada[j],pnodulos.nodulos[i]->n_nodos*sizeof(double));
    }
    pnodulos.nodulos[i]->conexiones_salida=(int **)realloc(pnodulos.nodulos[i]->conexiones_salida,pnodulos.nodulos[i]->n_nodos*sizeof(int));
    pnodulos.nodulos[i]->pesos_salida=(double **)realloc(pnodulos.nodulos[i]->pesos_salida,pnodulos.nodulos[i]->n_nodos*sizeof(double));
    pnodulos.nodulos[i]->transf=(func *)realloc(pnodulos.nodulos[i]->transf,pnodulos.nodulos[i]->n_nodos*sizeof(func));
  }
}
