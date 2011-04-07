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

/********************************************************************************
* Fichero: ordenar.c							        *
*									        *
* Función: ordenarNodulos()						        *
*									        *
* Autor: Pablo Álvarez de Sotomayor Posadillo				        *
*									        *
* Finalidad de la función: Ordena los nódulos según su aptitud.		        *
*									        *
* Parámetros de Entrada: NINGUNO					        *
* 									        *
* Parámetros Internos:							        *
* 	i: Entero. Contador.						        *
* 	j: Entero. Contador.						        *
* 	aux: Nódulo. Variable auxiliar parael intercambio de nódulos.	        *
* 									        *
* Parámetros de Salida:	NINGUNO						        *
* 									        *
* Funciones a las que llama la función:	NINGUNA				        *
*									        *
********************************************************************************/

void ordenarNodulos()
{
  int i,j;
  nodulo *aux;
  
  /*Se ordenan los nódulos y se copian a la nueva población*/
  for(i=num_nodulos*(pnodulos.n_subpobl-1);i<pnodulos.n_nodulos;i++)
    for(j=i;j<pnodulos.n_nodulos;j++){
      if(pnodulos.nodulos[i]->aptitud<pnodulos.nodulos[j]->aptitud){
        aux=pnodulos.nodulos[i];
	pnodulos.nodulos[i]=pnodulos.nodulos[j];
	pnodulos.nodulos[j]=aux;
      } /* end if */
      pnodulos.nodulos[i]->id=i;
    } /* end for */
  /* end for */
  aux=NULL;
}

/********************************************************************************
* Fichero: ordenar.c							        *
*									        *
* Función: ordenarRedes()						        *
*									        *
* Autor: Pablo Álvarez de Sotomayor Posadillo				        *
*									        *
* Finalidad de la función: Ordena las redes según su aptitud.		        *
*									        *
* Parámetros de Entrada: NINGUNO					        *
* 									        *
* Parámetros Internos:							        *
* 	i: Entero. Contador.						        *
* 	j: Entero. Contador.						        *
* 	aux: Red. Variable auxiliar para el intercambio de redes.	        *
* 									        *
* Parámetros de Salida:	NINGUNO						        *
* 									        *
* Funciones a las que llama la función:	NINGUNA				        *
*									        *
********************************************************************************/

void ordenarRedes()
{
  int i,j;
  red *aux;
  
  /*Se ordenan las redes de mayor a menor aptitud*/
  for(i=0;i<predes.n_redes;i++)
    for(j=i;j<predes.n_redes;j++)
      if(predes.redes[i]->aptitud<predes.redes[j]->aptitud){
        aux=predes.redes[i];
	predes.redes[i]=predes.redes[j];
	predes.redes[j]=aux;
      } /* end if */
    /* end for */
  /* end for */
  aux=NULL;
}
