/*******************************************************************************
*Copyright 2005 Pablo Alvarez de Sotomayor Posadillo			       *
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
* Fichero: generarSalida.c						       *
*									       *
* Funcion: generarSalidaNodulo()					       *
*									       *
* Autor: Pablo Alvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funcion: Genera la salida de un nodulo a partir de los datos *
*			   de entrada.					       *
*									       *
* Parametros de Entrada:						       *
* 	in: Vector de reales. Datos de entrada de cada nodo de entrada.	       *
* 	numNodulo: Entero. Numero de nodulo a tratar.			       *
* 	numEntrada: Entero. Numero de datos de entrada a los que se le ha      *
*		    generado una salida.				       *
* 	nod_out: Vector de reales. Almacena las salidas parciales de cada nodo.*
*									       *
* Parametros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	salidas: Vector de reales. Almacena las salidas parciales de cada nodo.*
* 	activacion: Vector de reales. Almacena las entradas de cada nodo.      *
*									       *
* Parametros de Salida:	NINGUNO						       *
*									       *
* Funciones a las que llama la funcion:					       *
*	error()->funcion que muestra un mensaje de error por pantalla.	       *
*									       *
*******************************************************************************/

void generarSalidaNodulo(double *in,int numNodulo,int numEntrada,double *nod_out)
{
  int i,j;
  double *salidas,*activacion;
 
  /*Se inicializan las variables y se reserva memoria*/
  if((salidas=(double *)malloc(pnodulos.nodulos[numNodulo]->n_nodos*sizeof(double)))==NULL)
    error(RES_MEM);
  if((activacion=(double *)malloc(pnodulos.nodulos[numNodulo]->n_nodos*sizeof(double)))==NULL)
    error(RES_MEM);
  for(i=0;i<pnodulos.nodulos[numNodulo]->n_nodos;i++)
  {
    salidas[i]=0.0;
    activacion[i]=0.0;
  }
  for(i=0;i<predes.n_nodos_salida;i++)
    pnodulos.nodulos[numNodulo]->salidas_parciales[numEntrada][i]=0.0;

  /*Los nodos de entrada propagan su dato de entrada a la primera capa*/
  for(i=0;i<predes.n_nodos_entrada;i++)
    for(j=0;j<pnodulos.nodulos[numNodulo]->n_nodos;j++)
      if(pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j]==1)
        activacion[j]+=(in[i]*pnodulos.nodulos[numNodulo]->pesos_entrada[i][j]);

  /*Se genera la salida de la capa de nodos ocultos*/
  for(i=0;i<pnodulos.nodulos[numNodulo]->n_nodos;i++)
    if(activacion[i]!=0.0)
      salidas[i]=(*(pnodulos.nodulos[numNodulo]->transf[i]))(activacion[i]);
   
  /*Se propagan la salida de los nodos a la capa de salida*/
  for(i=0;i<pnodulos.nodulos[numNodulo]->n_nodos;i++)
    if(salidas[i]!=0)
      for(j=0;j<predes.n_nodos_salida;j++)
        if(pnodulos.nodulos[numNodulo]->conexiones_salida[i][j]==1)
          pnodulos.nodulos[numNodulo]->salidas_parciales[numEntrada][j]+=(salidas[i]*pnodulos.nodulos[numNodulo]->pesos_salida[i][j]);

  /*Se almacenan las salidas de todos los nódulos en el buffer nod_out*/
  if(nod_out!=NULL)
  {
    for(i=0;i<predes.n_nodos_entrada;i++)
      nod_out[i]=in[i];
  
    for(i=0;i<pnodulos.nodulos[numNodulo]->n_nodos;i++)
      nod_out[i+predes.n_nodos_entrada]=salidas[i];
    
    for(i=0;i<predes.n_nodos_salida;i++)
      nod_out[i+predes.n_nodos_entrada+pnodulos.nodulos[numNodulo]->n_nodos]=(*(net_transf))(pnodulos.nodulos[numNodulo]->salidas_parciales[numEntrada][i]);
  }
  free(salidas);
  free(activacion);

}

/*******************************************************************************
* Fichero: generarSalida.c						       *
*									       *
* Funcion: generarSalidaRed()						       *
*									       *
* Autor: Pablo Alvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funcion: Genera la salida de una red a partir de las salidas *
*			   de sus nodulos.				       *
*									       *
* Parametros de Entrada:						       *
* 	numRed: Entero. Es el numero de la red a la que se le va a generar la  *
*		salida.							       *
* 	numEntrada: Entero. Es el numero de datos de entrada a los que se ha   *
*		    generado la salida.					       *
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

void generarSalidaRed(int numRed,int numEntrada)
{
  int i,j;

  /*Se genera la salida de la red a partir de las salidas parciales y de la función de transferencia*/
  for(i=0;i<predes.n_nodos_salida;i++)
  {
    predes.redes[numRed]->valores_salida[i]=0.0;
    for(j=0;j<pnodulos.n_subpobl;j++)
      predes.redes[numRed]->valores_salida[i]+=(*(net_transf))(predes.redes[numRed]->nodulos[j]->salidas_parciales[numEntrada][i]);
  }
}
