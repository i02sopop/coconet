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
* Fichero: medirAptitud.c						       *
*									       *
* Función: medirAptitudRed()						       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Mide la aptitud de la red en función de sus salidas.*
*									       *
* Parámetros de Entrada:						       *
* 	salida: Vector de reales. Salida que debe de tener la red.	       *
* 	numRed: Entero. Número de la red a tratar.			       *
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	max_output: Vector de reales. Salida seleccionada por la red y su valor*
*		    correspondiente.					       *
* 	max_file: Vector de reales. Salida válida según los datos de entrada   *
*		    y su valor correspondiente.				       *
* 									       *
* Parámetros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la función:	NINGUNA				       *
*									       *
*******************************************************************************/

void medirAptitudRed(double *salida,int numRed)
{
  int i;
  double max_output[2],max_file[2];
  
  /*Se inicializaan las variables. */
  max_output[0]=predes.redes[numRed]->valores_salida[0];
  max_file[0]=salida[0];
  max_output[1]=max_file[1]=0;
  /*Se calcula la salida seleccionada. */
  for(i=0;i<predes.n_nodos_salida;i++)
  {
    if(max_file[0]<salida[i])
    {
      max_file[0]=salida[i];
      max_file[1]=i;
    }
    if(max_output[0]<predes.redes[numRed]->valores_salida[i])
    {
      max_output[0]=predes.redes[numRed]->valores_salida[i];
      max_output[1]=i;
    }
  }
  /*Se comprueba si la salida seleccionada es la correcta. */
  if(max_file[1]==max_output[1])
    predes.redes[numRed]->aptitud++;
}

/*******************************************************************************
* Fichero: medirAptitud.c						       *
*									       *
* Función: medirAptitudNodulos()					       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Mide la aptitud de los nódulos en función de varios *
*			   parámetros.					       *
*									       *
* Parámetros de Entrada:						       *
* 	numNodulo: Entero.Número del nódulo al que se le va a medir la aptitud.*
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	sust: Real. Aptitud parcial por sustitución.			       *
* 	dif: Real. Aptitud parcial por diferencia.			       *
* 	best: Real. Aptitud parcial según las mejores redes en las que         *
*	      participa el nódulo.					       *
* 	copia: Vector de redes. Copia de la población de redes.		       *
* 									       *
* Parámetros de Salida: NINGUNO						       *
* 									       *
* Funciones a las que llama la función:					       *
*	error()->Muestra un mensaje de error en función de un número de error. *
*	copiarRed()->Hace una copia de la población de redes.		       *
*	liberarRed()->Libera la memoria de una red.			       *
*	sustitucion()->Mide la aptitud por sustitución.			       *
*	diferencia()->Mide la aptitud por diferencia.			       *
*	mejores()->Mide la aptitud mediante las mejores redes en las que       *
*		   participa el nódulo.					       *
*									       *
*******************************************************************************/

void medirAptitudNodulos(int numNodulo)
{
  int i;
  double sust,dif,best;
  red **copia;
  
  copia=NULL;
  if((copia=(red **)malloc(predes.n_redes*sizeof(red)))==NULL)
    error(RES_MEM);
  /*Se hace una copia de la población de redes*/
  copiarRed(predes.redes,copia);
  /*Se mide la aptitud parcial del nódulo por sustitución*/
  sust=sustitucion(numNodulo,copia);

  for(i=0;i<predes.n_redes;i++)
    liberarRed(predes.redes[i]);
  free(predes.redes);
  if((predes.redes=(red **)malloc(predes.n_redes*sizeof(red)))==NULL)
    error(RES_MEM);
  /*Se restituye la población de redes*/
  copiarRed(copia,predes.redes);
  /*Se mide la aptitud parcial del nódulo por diferecia*/
  dif=diferencia(numNodulo,copia);

  /*Se restituye la población de redes*/
  for(i=0;i<predes.n_redes;i++)
    liberarRed(predes.redes[i]);
  free(predes.redes);
  predes.redes=copia;
  copia=NULL;
  best=mejores(numNodulo);

  /*Se mide la aptitud parcial del nódulo*/
  /*Se calcula la aptitud total mediante la ponderación de las aptitudes calculadas anteriormente*/
  pnodulos.nodulos[numNodulo]->aptitud=pond.sust*sust+pond.dif*dif+pond.best*best;
}

/*******************************************************************************
* Fichero: medirAptitud.c						       *
*									       *
* Función: copiarRed()							       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Hace una copia de la población de redes.	       *
*									       *
* Parámetros de Entrada:						       *
* 	Origen: Vector de redes. Población de origen.			       *
* 	Nueva: Vector de redes. Población de destino.			       *
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 									       *
* Parámetros de Salida: NINGUNO						       *
* 									       *
* Funciones a las que llama la función:					       *
*	error()->Funcion que muestra un mensaje de error por pantalla.	       *
*									       *
*******************************************************************************/

void copiarRed(red **origen,red **nueva)
{
  int i,j;
  
  /*Se hace una copia de las poblaciones de redes*/
  for(i=0;i<predes.n_redes;i++)
  {
    if((nueva[i]=(red *)malloc(sizeof(red)))==NULL)
      error(RES_MEM);
    nueva[i]->aptitud=origen[i]->aptitud;
    if((nueva[i]->nodulos=(nodulo **)malloc(pnodulos.n_subpobl*sizeof(nodulo)))==NULL)
      error(RES_MEM);
    for(j=0;j<pnodulos.n_subpobl;j++)
      nueva[i]->nodulos[j]=origen[i]->nodulos[j];
    if((nueva[i]->valores_salida=(double *)malloc(predes.n_nodos_salida*sizeof(double)))==NULL)
      error(RES_MEM);
    for(j=0;j<predes.n_nodos_salida;j++)
      nueva[i]->valores_salida[j]=origen[i]->valores_salida[j];
  }
}

/*******************************************************************************
* Fichero: medirAptitud.c							       *
*									       *
* Función: diferencia()							       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Mide la aptitud parcial del nódulo por diferencia.  *
*									       *
* Parámetros de Entrada:						       *
* 	numNodulo: Entero. Número de nódulo al que se le mide la aptitud       *
*		   parcial.						       *
* 	copia: Vector deredes. Copia de la población de redes.		       *
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	k: Entero. Contador.						       *
* 	l: Entero. Contador.						       *
* 	numredes: Entero. Número de redes en las que participa el nódulo.      *
* 	redes: Vector de enteros. Identificador de las redes en las que        *
*	       participa el nódulo.					       *
* 									       *
* Parámetros de Salida:							       *
* 	suma: Real. Aptitud parcial del nódulo.				       *
* 									       *
* Funciones a las que llama la función:					       *
*	medirAptitudRed()->Función que mide la aptitud de una red.	       *
*	error()->Funcion que muestra por pantalla un mensaje de error.	       *
* 									       *
*******************************************************************************/

double diferencia(int numNodulo,red **copia)
{
  int i,j,k,l,numredes,*redes;
  double suma;
  
  numredes=0;
  suma=0.0;

  if((redes=(int *)malloc(max_redes*sizeof(int)))==NULL)
    error(RES_MEM);
  /*Se almacenan las redes en las que participa el nódulo*/
  for(i=0;i<predes.n_redes;i++)
    if(predes.redes[i]->nodulos[pnodulos.n_subpobl-1]==pnodulos.nodulos[numNodulo])
    {
      predes.redes[i]->aptitud=0;
      redes[numredes]=i;
      numredes++;
    }
  for(i=0;i<numredes;i++)
  {
    for(j=0;j<n_entrenamiento;j++)
    {
      /*Se generan las salidas de las redes sin tener en cuenta las salidas parciales del nódulo al que se está midiendo la aptitud*/
      for(k=0;k<predes.n_nodos_salida;k++)
      {
        predes.redes[redes[i]]->valores_salida[k]=0.0;
        for(l=0;l<pnodulos.n_subpobl-1;l++)
          predes.redes[redes[i]]->valores_salida[k]+=(*(net_transf))(predes.redes[redes[i]]->nodulos[l]->salidas_parciales[j][k]);
      }
      /*Se miden las aptitudes de las redes*/
      if(pnodulos.n_subpobl>1)
        medirAptitudRed(salida[j],redes[i]);
      else
        predes.redes[redes[i]]->aptitud=0.0;
    }
    predes.redes[redes[i]]->aptitud/=n_entrenamiento;
    /*Se añade la diferencia entre la aptitud que tiene la red con el nódulo y la que tiene sin el*/
    suma+=predes.redes[redes[i]]->aptitud-copia[redes[i]]->aptitud;
  }
  /*Se hace la media de las diferencias de aptitudes calculadas anteriormente*/
  if(numredes>0)
    suma/=numredes;
  free(redes);

  return suma;
}

/*******************************************************************************
* Fichero: medirAptitud.c						       *
*									       *
* Función: mejores()							       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Mide la aptitud parcial de los nódulos en función de*
*			   las mejores redes en las que participa.	       *
*									       *
* Parámetros de Entrada:						       *
* 	numNodulo: Entero. Número de nódulo al que se le va a hacer la medición*
*		   de la aptitud.					       *
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	num: Entero. Número de redes en las que participa el nódulo y que se   *
*	     utilizan para medir la aptitud parcial.			       *
* 									       *
* Parámetros de Salida:							       *
* 	suma: Real. Aptitud parcial del nódulo.				       *
* 									       *
* Funciones a las que llama la función:	NINGUNA				       *
*									       *
*******************************************************************************/

double mejores(int numNodulo)
{
  int i,num;
  double suma;

  suma=0;
  num=0;
  
  for(i=0;i<predes.n_redes && num<redsel;i++)
  {
    if(predes.redes[i]->nodulos[pnodulos.n_subpobl-1]==pnodulos.nodulos[numNodulo])
    {
      num++;
      suma+=predes.redes[i]->aptitud;
    }
  }
  /*Se hace la media de las mejores redes encontradas en las que participe el nódulo a tratar*/
  if(suma>0)
    suma/=num;

  return suma;
}

/*******************************************************************************
* Fichero: medirAptitud.c						       *
*									       *
* Función: sustitucion()						       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Mide la aptitud parcial del nódulo por sustitción.  *
*									       *
* Parámetros de Entrada:						       *
* 	numNodulo: Entero. Número del nódulo a tratar.			       *
* 	copia: Vector de redes. Copia de lapoblación de redes.		       *
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	numRedes: Entero. Número de redes en las que participa el nódulo.      *
* 	seleccionadas: Entero. Número de redes seleccionadas.		       *
* 									       *
* Parámetros de Salida:							       *
* 	suma: Real. Aptitud parcial del nódulo.				       *
* 									       *
* Funciones a las que llama la función:					       *
*	generarSalidaRed()->Genera la salida de una red a partir de un patrón  *
*			    de entrada.					       *
*	meditAptitudRed()->Mide la aptitud de la red a partir de la salida     *
*			   generada.					       *
*									       *
*******************************************************************************/

double sustitucion(int numNodulo,red **copia)
{
  int i,j,numRedes,seleccionadas;
  double suma;
  
  numRedes=0;  
  suma=0;
  seleccionadas=redsel;
  
  for(i=0;i<seleccionadas && i<predes.n_redes;i++)
  {
    if(predes.redes[i]->nodulos[pnodulos.n_subpobl-1]!=pnodulos.nodulos[numNodulo])
    {
      /*Se cambian los nódulos de su subpoblación por el nódulo a medir la aptitud*/
      predes.redes[i]->nodulos[pnodulos.n_subpobl-1]=pnodulos.nodulos[numNodulo];
      /*Se inicializan las aptitudes de las redes para realizar una nueva medición*/
      predes.redes[i]->aptitud=0;
      /*Se entrenan las redes modificadas*/
      for(j=0;j<n_entrenamiento;j++)
      {
        generarSalidaRed(i,j);
        medirAptitudRed(salida[j],i);
      }
      /*Se normaliza la aptitud de la red modificada según las entradas del fichero de entrenamiento*/
      predes.redes[i]->aptitud/=n_entrenamiento;
      suma+=predes.redes[i]->aptitud-copia[i]->aptitud;
    }
    /*Se añade el número de red al vector*/
    else
    {
      numRedes++;
      seleccionadas++;
    }
  }
  /*Se devuelve la diferencia media de las aptitudes de las redes modificadas*/
  suma/=(seleccionadas-numRedes);
  
  return suma;
}

/*******************************************************************************
* Fichero: medirAptitud.c						       *
*									       *
* Función: normalizarAptitudNodulos()					       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Normaliza la aptitud de los nódulos poniendo la     *
*			   aptitud mínima a 0.				       *
*									       *
* Parámetros de Entrada: NINGUNO					       *
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	min: Real. Aptitud mínima.					       *
* 									       *
* Parámetros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la función:	NINGUNA				       *
*									       *
*******************************************************************************/

void normalizarAptitudNodulos()
{
  int i;
  double min;
  
  /*Se calcula el valor mínimo de la aptitud de los nódulos*/
  min=0;
  for(i=0;i<pnodulos.n_nodulos;i++)
    if(min>pnodulos.nodulos[i]->aptitud)
      min=pnodulos.nodulos[i]->aptitud;
  /*Se pone el valor mínimo de la aptitud de los nódulos a 0*/
  if(min<0)
    for(i=0;i<pnodulos.n_nodulos;i++)
      pnodulos.nodulos[i]->aptitud-=min;
}
