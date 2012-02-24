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

/*******************************************************************************
* Fichero: imprimir.c		       					       *
*									       *
* Funcion: imprimirNodulo()						       *
*									       *
* Autor: Pablo Alvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funcion: Imprime informacion de un nodulo en un fichero de   *
*			   salida.				       	       *
*									       *
* Parametros de Entrada:						       *
* 	imp: Nodulo. Nodulo a imprimir.				               *
* 	iter: Entero. Numero de patrones de entrada, los cuales generan una    *
*	      salida.							       *
*									       *
* Parametros Internos:						               *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	salida: Puntero a fichero. Fichero de salida.			       *
*	numEntrada: Entero. Contador de los patrones de entrada para mostrar su*
*	salida.								       *
*									       *
* Parametros de Salida: NINGUNO					               *
*									       *
* Funciones a las que llama la funcion:	NINGUNA			               *
*									       *
*******************************************************************************/

void imprimirNodulo(nodulo *imp,int iter)
{
  int i,j,numEntrada;
  FILE *salida;

  /*Se abre el fichero donde se va a escribir la información del nódulo*/
  salida=fopen("nodulos.txt","a");
  /*Se imprime la información del nódulo*/
  fprintf(salida,"id %d\n",imp->id);
  fprintf(salida,"n_nodos %d\n",imp->n_nodos);
  fprintf(salida,"aptitud %lf\n",imp->aptitud);
  fprintf(salida,"\nMatriz de conexiones de entrada");
  for(i=0;i<predes.n_nodos_entrada;i++)
  {
    fprintf(salida,"\n");
    for(j=0;j<imp->n_nodos;j++)
      fprintf(salida,"%d ",imp->conexiones_entrada[i][j]);
  }
  fprintf(salida,"\nMatriz de conexiones de salida");
  for(i=0;i<imp->n_nodos;i++)
  {
    fprintf(salida,"\n");
    for(j=0;j<predes.n_nodos_salida;j++)
      fprintf(salida,"%d ",imp->conexiones_salida[i][j]);
  }
  fprintf(salida,"\nMatriz de pesos de entrada");
  for(i=0;i<predes.n_nodos_entrada;i++)
  {
    fprintf(salida,"\n");
    for(j=0;j<imp->n_nodos;j++)
      fprintf(salida,"%lf ",imp->pesos_entrada[i][j]);
  }
  fprintf(salida,"\nMatriz de pesos de salida");
  for(i=0;i<imp->n_nodos;i++)
  {
    fprintf(salida,"\n");
    for(j=0;j<predes.n_nodos_salida;j++)
      fprintf(salida,"%lf ",imp->pesos_salida[i][j]);
  }
  fprintf(salida,"\nSalidas parciales\n");
  for(numEntrada=0;numEntrada<iter;numEntrada++)
    for(i=0;i<predes.n_nodos_salida;i++)
      fprintf(salida,"%lf ",imp->salidas_parciales[numEntrada][i]);
  fprintf(salida,"\n\n");
  /*Se cierra el fichero dee salida*/
  fclose(salida);
}

/*******************************************************************************
* Fichero: imprimir.c							       *
*									       *
* Funcion: imprimirRed()						       *
*									       *
* Autor: Pablo Alvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funcion: Imprime datos sobre la red en un fichero de salida. *
*									       *
* Parametros de Entrada:						       *
* 	imp: Red. Red que se va a imprimir.				       *
*									       *
* Parametros Internos:							       *
* 	i: Entero. Contador.						       *
* 	salida: Puntero a fichero. Fichero de salida.			       *
*									       *
* Parametros de Salida:	NINGUNO						       *
*									       *
* Funciones a las que llama la funcion:	NINGUNA				       *
*									       *
*******************************************************************************/

void imprimirRed(red *imp)
{
  int i;
  FILE *salida;
  
  /*Se abre el fichero donde se va a escribir la red a imprimir*/
  salida=fopen("red.txt","a");
  /*Se imprimen los datos de la red*/
  fprintf(salida,"nodulos ");
  for(i=0;i<pnodulos.n_subpobl;i++)
    fprintf(salida,"%d ",imp->nodulos[i]->id);
  fprintf(salida,"\naptitud %lf\n",imp->aptitud);
  fprintf(salida,"Valores salida\n");
  for(i=0;i<predes.n_nodos_salida;i++)
    fprintf(salida,"%lf ",imp->valores_salida[i]);
  fprintf(salida,"\n");
  if(net_transf==(func)&Logistic)
    fprintf(salida,"Logistic\n");
  else
    fprintf(salida,"HyperbolicTangent\n");
  /*Se cierra el fichero de salida*/
  fclose(salida);
}
