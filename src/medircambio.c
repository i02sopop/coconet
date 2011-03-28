/*******************************************************************************
*Copyright 2005 Pablo �lvarez de Sotomayor Posadillo			       *
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
* Funci�n: medirCambio()						       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Mide el cambio en la aptitud media de la poblaci�n  *
*			   de redes.					       *
*									       *
* Par�metros de Entrada:						       *
* 	aptitudRedes: Real. Aptitud media anterior de las redes.	       *
* 	iteracion: Entero. Indica cuantas veces se ha evolucionado la poblaci�n*
*		   de redes.						       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	aptitudTemp: Real. Nueva aptitud media de la poblaci�n de redes.       *
* 	dif: Real. diferencia entre la nueva aptitud media de la poblaci�n de  *
*	     redes y la antigua.					       *
* 									       *
* Par�metros de Salida:	Devuelve true si la diferencia entre la nueva aptitud  *
*			media de la poblaci�n de redes y la antigua es menor al*
*			l�mite de la evoluci�n.  			       *
* 									       *
* Funciones a las que llama la funci�n:					       *
*	generarSalidaNodulo()->Genera la salida de un n�dulo en funci�n de unos*
*			       datos de entrada.			       *
*	generarSalidaRed()->Genera la salida de una red en funci�n de la salida*
*			    de sus n�dulos				       *
* 	medirAptitudRed()->Mide la aptitud de una red en funci�n de las salidas*
*			   generadas.					       *
* 	medirAptitudNodulos()->Mide la aptitud de los n�dulos bas�ndose en tres*
*			       par�metros.				       *
* 	normalizarAptitudNodulos()->Hace que la aptitud m�nima de la poblaci�n *
*				    de n�dulos sea cero.		       *
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
    /*Se entrenan por primera vez las redes para tener un valor inicial de la aptitud de los n�dulos y de las redes*/
    for(i=0;i<n_entrenamiento;i++)
    {
      /*Se genera la salida del n�dulo*/
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
    /*Se mide la aptitud de los n�dulos*/
    for(i=0;i<pnodulos.n_nodulos;i++)
      medirAptitudNodulos(i);
    /*Se normalizan las aptitudes de los n�dulos*/
    normalizarAptitudNodulos();
    aptitudTemp=0.0;
    /*Se mide la aptitud media de todas las redes*/
    for(i=0;i<predes.n_redes;i++)
      aptitudTemp+=predes.redes[i]->aptitud;
    aptitudTemp/=predes.n_redes;
  }
  /*Si es la primera iteraci�n o la diferencia entre la aptitud media anterior y la actual supera un l�mite la evoluci�n continua*/
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
* Funci�n: medirCambioNodulo()						       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Mide el cambio de aptitud que ha habido en un n�dulo*
*			   determinado.					       *
*									       *
* Par�metros de Entrada:						       *
* 	numNodulo: Entero. N�mero del n�dulo a tratar.			       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 									       *
* Par�metros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:					       *
*	ordenarRedes()->Ordena las redes seg�n su aptitud.		       *
* 	generarSalidaNodulo()->Genera la salida de un n�dulo en funci�n de la  *
*			       entrada.					       *
* 	generarSalidaRed()->Genera la salida de la red en funci�n de la salida *
*			    de sus n�dulos.				       *
* 	medirAptitudRed()->Mide la aptitud de la red en funci�n de las salidas *
*			   generadas.					       *
* 	medirAptitudNodulo()->Mide la aptitud de los n�dulos en funci�n de     *
*			      varios par�metros.			       *
*	normalizarAptitudNodulos()->Normaliza la aptitud de los nodulos,       *
*				    poniendo la minima a 0.		       *
* 									       *
*******************************************************************************/

void medirCambioNodulo(int numNodulo)
{
  int i,j;

  for(i=0;i<predes.n_redes;i++)
  {
   /*Se entrenan las redes del n�dulo modificado*/
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
  /*Se mide la nueva aptitud del n�dulo*/
  medirAptitudNodulos(numNodulo);
  normalizarAptitudNodulos();
}
