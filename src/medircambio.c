/*******************************************************************************
*Copyright 2005 Pablo Álvarez de Sotomayor Posadillo			       *
*									       *
*This file is part of Coconet.						       *
*									       *
*Coconet is free software; you can redistribute it and/or modify	       *
*it under the terms of the GNU General Public License as published by	       *
*the Free Software Foundation; either version 2 of the License, or	       *
*(at your option) any later version.					       *
*									       *
*Coconet is distributed in the hope that it will be useful,		       *
*but WITHOUT ANY WARRANTY; without even the implied warranty of		       *
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		       *
*GNU General Public License for more details.				       *
*									       *
*You should have received a copy of the GNU General Public License	       *
*along with Coconet; if not, write to the Free Software			       *
*Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA     *
*******************************************************************************/

#include <definiciones.h>

/*******************************************************************************
* Fichero: medirCambio.c						       *
*									       *
* Función: medirCambio()						       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Mide el cambio en la aptitud media de la población  *
*			   de redes.					       *
*									       *
* Parámetros de Entrada:						       *
* 	aptitudRedes: Real. Aptitud media anterior de las redes.	       *
* 	iteracion: Entero. Indica cuantas veces se ha evolucionado la población*
*		   de redes.						       *
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	aptitudTemp: Real. Nueva aptitud media de la población de redes.       *
* 	dif: Real. diferencia entre la nueva aptitud media de la población de  *
*	     redes y la antigua.					       *
* 									       *
* Parámetros de Salida:	Devuelve true si la diferencia entre la nueva aptitud  *
*			media de la población de redes y la antigua es menor al*
*			límite de la evolución.  			       *
* 									       *
* Funciones a las que llama la función:					       *
*	generarSalidaNodulo()->Genera la salida de un nódulo en función de unos*
*			       datos de entrada.			       *
*	generarSalidaRed()->Genera la salida de una red en función de la salida*
*			    de sus nódulos				       *
* 	medirAptitudRed()->Mide la aptitud de una red en función de las salidas*
*			   generadas.					       *
* 	medirAptitudNodulos()->Mide la aptitud de los nódulos basándose en tres*
*			       parámetros.				       *
* 	normalizarAptitudNodulos()->Hace que la aptitud mínima de la población *
*				    de nódulos sea cero.		       *
* 									       *
*******************************************************************************/

bool medirCambio(double *aptitudRedes, int iteracion)
{
  int i,j;
  double aptitudTemp,dif;

  aptitudTemp=0;
  /*Se entrenan las redes para comprobar su aptitud*/
  if(iteracion>0)
  {
    fprintf(stderr,"Entrenar\n");
    /*Se entrenan por primera vez las redes para tener un valor inicial de la aptitud de los nódulos y de las redes*/
    for(i=0;i<n_entrenamiento;i++)
    {
      /*Se genera la salida del nódulo*/
      for(j=pnodulos.n_nodulos-num_nodulos;j<pnodulos.n_nodulos;j++)
        generarSalidaNodulo(entrada[i],j,i,NULL);
      /*Se mide la aptitud de la red*/
      for(j=0;j<predes.n_redes;j++)
      {
        generarSalidaRed(j,i);
        medirAptitudRed(salida[i],j);
      }
    }
    /*Se normalizan las aptitudes de las redes*/
    for(i=0;i<predes.n_redes;i++)
      predes.redes[i]->aptitud/=n_entrenamiento;
    /*Se mide la aptitud de los nódulos*/
    for(i=0;i<pnodulos.n_nodulos;i++)
      medirAptitudNodulos(i);
    /*Se normalizan las aptitudes de los nódulos*/
    normalizarAptitudNodulos();
    aptitudTemp=0.0;
    /*Se mide la aptitud media de todas las redes*/
    for(i=0;i<predes.n_redes;i++)
      aptitudTemp+=predes.redes[i]->aptitud;
    aptitudTemp/=predes.n_redes;
  }
  /*Si es la primera iteración o la diferencia entre la aptitud media anterior y la actual supera un límite la evolución continua*/
  if(iteracion==0)
  {
    *aptitudRedes=0.0;
    return false;
  }  
  else
  {
    dif=aptitudTemp-*aptitudRedes;
    *aptitudRedes=aptitudTemp;
    if(dif>evolim)
      return false;
    return true;
  }
}

/*******************************************************************************
* Fichero: medirCambio.c						       *
*									       *
* Función: medirCambioNodulo()						       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Mide el cambio de aptitud que ha habido en un nódulo*
*			   determinado.					       *
*									       *
* Parámetros de Entrada:						       *
* 	numNodulo: Entero. Número del nódulo a tratar.			       *
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 									       *
* Parámetros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la función:					       *
*	ordenarRedes()->Ordena las redes según su aptitud.		       *
* 	generarSalidaNodulo()->Genera la salida de un nódulo en función de la  *
*			       entrada.					       *
* 	generarSalidaRed()->Genera la salida de la red en función de la salida *
*			    de sus nódulos.				       *
* 	medirAptitudRed()->Mide la aptitud de la red en función de las salidas *
*			   generadas.					       *
* 	medirAptitudNodulo()->Mide la aptitud de los nódulos en función de     *
*			      varios parámetros.			       *
*	normalizarAptitudNodulos()->Normaliza la aptitud de los nodulos,       *
*				    poniendo la minima a 0.		       *
* 									       *
*******************************************************************************/

void medirCambioNodulo(int numNodulo)
{
  int i,j;

  for(i=0;i<predes.n_redes;i++)
  {
   /*Se entrenan las redes del nódulo modificado*/
    if(predes.redes[i]->nodulos[pnodulos.n_subpobl-1]==pnodulos.nodulos[numNodulo])
    {
      predes.redes[i]->aptitud=0;
      /*Se calculan las aptitudes de las redes*/
      for(j=0;j<n_entrenamiento;j++)
      {
        generarSalidaNodulo(entrada[j],numNodulo,j,NULL);
        generarSalidaRed(i,j);
        medirAptitudRed(salida[j],i);
      }
      /*Se normalizan las aptitudes de las redes*/
      predes.redes[i]->aptitud/=n_entrenamiento;
    }
  }
  /*Se ordenan las redes por aptitud*/
  ordenarRedes();
  /*Se mide la nueva aptitud del nódulo*/
  medirAptitudNodulos(numNodulo);
  normalizarAptitudNodulos();
}
