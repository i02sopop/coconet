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
* Fichero: medirAptitud.c						       *
*									       *
* Funci�n: medirAptitudRed()						       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Mide la aptitud de la red en funci�n de sus salidas.*
*									       *
* Par�metros de Entrada:						       *
* 	salida: Vector de reales. Salida que debe de tener la red.	       *
* 	numRed: Entero. N�mero de la red a tratar.			       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	max_output: Vector de reales. Salida seleccionada por la red y su valor*
*		    correspondiente.					       *
* 	max_file: Vector de reales. Salida v�lida seg�n los datos de entrada   *
*		    y su valor correspondiente.				       *
* 									       *
* Par�metros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:	NINGUNA				       *
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
* Funci�n: medirAptitudNodulos()					       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Mide la aptitud de los n�dulos en funci�n de varios *
*			   par�metros.					       *
*									       *
* Par�metros de Entrada:						       *
* 	numNodulo: Entero.N�mero del n�dulo al que se le va a medir la aptitud.*
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	sust: Real. Aptitud parcial por sustituci�n.			       *
* 	dif: Real. Aptitud parcial por diferencia.			       *
* 	best: Real. Aptitud parcial seg�n las mejores redes en las que         *
*	      participa el n�dulo.					       *
* 	copia: Vector de redes. Copia de la poblaci�n de redes.		       *
* 									       *
* Par�metros de Salida: NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:					       *
*	error()->Muestra un mensaje de error en funci�n de un n�mero de error. *
*	copiarRed()->Hace una copia de la poblaci�n de redes.		       *
*	liberarRed()->Libera la memoria de una red.			       *
*	sustitucion()->Mide la aptitud por sustituci�n.			       *
*	diferencia()->Mide la aptitud por diferencia.			       *
*	mejores()->Mide la aptitud mediante las mejores redes en las que       *
*		   participa el n�dulo.					       *
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
  /*Se hace una copia de la poblaci�n de redes*/
  copiarRed(predes.redes,copia);
  /*Se mide la aptitud parcial del n�dulo por sustituci�n*/
  sust=sustitucion(numNodulo,copia);

  for(i=0;i<predes.n_redes;i++)
    liberarRed(predes.redes[i]);
  free(predes.redes);
  if((predes.redes=(red **)malloc(predes.n_redes*sizeof(red)))==NULL)
    error(RES_MEM);
  /*Se restituye la poblaci�n de redes*/
  copiarRed(copia,predes.redes);
  /*Se mide la aptitud parcial del n�dulo por diferecia*/
  dif=diferencia(numNodulo,copia);

  /*Se restituye la poblaci�n de redes*/
  for(i=0;i<predes.n_redes;i++)
    liberarRed(predes.redes[i]);
  free(predes.redes);
  predes.redes=copia;
  copia=NULL;
  best=mejores(numNodulo);

  /*Se mide la aptitud parcial del n�dulo*/
  /*Se calcula la aptitud total mediante la ponderaci�n de las aptitudes calculadas anteriormente*/
  pnodulos.nodulos[numNodulo]->aptitud=pond.sust*sust+pond.dif*dif+pond.best*best;
}

/*******************************************************************************
* Fichero: medirAptitud.c						       *
*									       *
* Funci�n: copiarRed()							       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Hace una copia de la poblaci�n de redes.	       *
*									       *
* Par�metros de Entrada:						       *
* 	Origen: Vector de redes. Poblaci�n de origen.			       *
* 	Nueva: Vector de redes. Poblaci�n de destino.			       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 									       *
* Par�metros de Salida: NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:					       *
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
* Funci�n: diferencia()							       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Mide la aptitud parcial del n�dulo por diferencia.  *
*									       *
* Par�metros de Entrada:						       *
* 	numNodulo: Entero. N�mero de n�dulo al que se le mide la aptitud       *
*		   parcial.						       *
* 	copia: Vector deredes. Copia de la poblaci�n de redes.		       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	k: Entero. Contador.						       *
* 	l: Entero. Contador.						       *
* 	numredes: Entero. N�mero de redes en las que participa el n�dulo.      *
* 	redes: Vector de enteros. Identificador de las redes en las que        *
*	       participa el n�dulo.					       *
* 									       *
* Par�metros de Salida:							       *
* 	suma: Real. Aptitud parcial del n�dulo.				       *
* 									       *
* Funciones a las que llama la funci�n:					       *
*	medirAptitudRed()->Funci�n que mide la aptitud de una red.	       *
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
  /*Se almacenan las redes en las que participa el n�dulo*/
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
      /*Se generan las salidas de las redes sin tener en cuenta las salidas parciales del n�dulo al que se est� midiendo la aptitud*/
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
    /*Se a�ade la diferencia entre la aptitud que tiene la red con el n�dulo y la que tiene sin el*/
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
* Funci�n: mejores()							       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Mide la aptitud parcial de los n�dulos en funci�n de*
*			   las mejores redes en las que participa.	       *
*									       *
* Par�metros de Entrada:						       *
* 	numNodulo: Entero. N�mero de n�dulo al que se le va a hacer la medici�n*
*		   de la aptitud.					       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	num: Entero. N�mero de redes en las que participa el n�dulo y que se   *
*	     utilizan para medir la aptitud parcial.			       *
* 									       *
* Par�metros de Salida:							       *
* 	suma: Real. Aptitud parcial del n�dulo.				       *
* 									       *
* Funciones a las que llama la funci�n:	NINGUNA				       *
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
  /*Se hace la media de las mejores redes encontradas en las que participe el n�dulo a tratar*/
  if(suma>0)
    suma/=num;

  return suma;
}

/*******************************************************************************
* Fichero: medirAptitud.c						       *
*									       *
* Funci�n: sustitucion()						       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Mide la aptitud parcial del n�dulo por sustitci�n.  *
*									       *
* Par�metros de Entrada:						       *
* 	numNodulo: Entero. N�mero del n�dulo a tratar.			       *
* 	copia: Vector de redes. Copia de lapoblaci�n de redes.		       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	numRedes: Entero. N�mero de redes en las que participa el n�dulo.      *
* 	seleccionadas: Entero. N�mero de redes seleccionadas.		       *
* 									       *
* Par�metros de Salida:							       *
* 	suma: Real. Aptitud parcial del n�dulo.				       *
* 									       *
* Funciones a las que llama la funci�n:					       *
*	generarSalidaRed()->Genera la salida de una red a partir de un patr�n  *
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
      /*Se cambian los n�dulos de su subpoblaci�n por el n�dulo a medir la aptitud*/
      predes.redes[i]->nodulos[pnodulos.n_subpobl-1]=pnodulos.nodulos[numNodulo];
      /*Se inicializan las aptitudes de las redes para realizar una nueva medici�n*/
      predes.redes[i]->aptitud=0;
      /*Se entrenan las redes modificadas*/
      for(j=0;j<n_entrenamiento;j++)
      {
        generarSalidaRed(i,j);
        medirAptitudRed(salida[j],i);
      }
      /*Se normaliza la aptitud de la red modificada seg�n las entradas del fichero de entrenamiento*/
      predes.redes[i]->aptitud/=n_entrenamiento;
      suma+=predes.redes[i]->aptitud-copia[i]->aptitud;
    }
    /*Se a�ade el n�mero de red al vector*/
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
* Funci�n: normalizarAptitudNodulos()					       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Normaliza la aptitud de los n�dulos poniendo la     *
*			   aptitud m�nima a 0.				       *
*									       *
* Par�metros de Entrada: NINGUNO					       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	min: Real. Aptitud m�nima.					       *
* 									       *
* Par�metros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:	NINGUNA				       *
*									       *
*******************************************************************************/

void normalizarAptitudNodulos()
{
  int i;
  double min;
  
  /*Se calcula el valor m�nimo de la aptitud de los n�dulos*/
  min=0;
  for(i=0;i<pnodulos.n_nodulos;i++)
    if(min>pnodulos.nodulos[i]->aptitud)
      min=pnodulos.nodulos[i]->aptitud;
  /*Se pone el valor m�nimo de la aptitud de los n�dulos a 0*/
  if(min<0)
    for(i=0;i<pnodulos.n_nodulos;i++)
      pnodulos.nodulos[i]->aptitud-=min;
}
