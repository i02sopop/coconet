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
* Fichero: crearPoblaciones.c						       *
*									       *
* Funcion: anadirNoduloRedes()						       *
*									       *
* Autor: Pablo Alvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funcion: Se a�ade una nueva subpoblacion de nodulos a la     *
* 			   poblacion de redes.				       *
*									       *
* Parametros de Entrada: NINGUNO					       *
*									       *
* Parametros Internos: NINGUNO						       *
*									       *
* Parametros de Salida:	NINGUNO						       *
*									       *
* Funciones a las que llama la funcion:					       *
*	crearRedes()->Funcion que crea una nueva poblacion de redes a partir de*
* 		      la poblacion de nodulos existente.		       *
*	crearDescendencia()->Funcion que crea la descendencia de la poblacion  *
*			     de redes a partir de la nueva subpoblacion de     *
*			     nodulos y de la poblacion de redes antigua.       *
*	error()->Funcion que imprime por pantalla un mensaje de error.	       *
*									       *
*******************************************************************************/

void anadirNodulosRedes()
{
  /*Si es la primera poblaci�n de n�dulos que se a�ade se crea una red por 
    n�dulo*/
  if(pnodulos.n_subpobl==1){
    predes.n_redes=num_nodulos;
    if((predes.redes=(red **)malloc(num_nodulos*sizeof(red)))==NULL)
      error(RES_MEM);
    crearRedes();
  }else /*Se crea la descendencia de la poblaci�n de redes a�adiendo un nuevo 
	  n�dulo a cada red*/
    crearDescendencia();
}

/*******************************************************************************
* Fichero: crearPoblaciones.c						       *
*									       *
* Funci�n: crearDescendencia()						       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Crea la descendencia de la poblaci�n de redes a     *
*			   partir de la nueva subpoblaci�n de n�dulos y de la  *
*			   poblaci�n de redes existente.		       *
*									       *
* Par�metros de Entrada: NINGUNO					       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	redsel: Entero.	Selecci�n de una red de la poblaci�n actual.	       *
* 	nodsel: Entero. Selecci�n de un n�dulo de la nueva subpoblaci�n.       *
* 	nueva: vector de redes. Nueva poblaci�n de redes.		       *
* 									       *
* Par�metros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:					       *
*	liberarRed()->Funci�n que libera una red dada.			       *
*	error()->Funcion que imprime por pantalla un mensaje de error.	       *
*									       *
*******************************************************************************/

void crearDescendencia()
{
  int i,j,redsel,nodsel;
  red **nueva; /*nueva poblaci�n de redes. Descendencia de la poblaci�n actual*/
  
  /*creaci�n de la nueva red*/
  if((nueva=(red **)malloc(max_redes*sizeof(red)))==NULL)
    error(RES_MEM);
  for(i=0;i<max_redes;i++){
    if((nueva[i]=(red *)malloc(sizeof(red)))==NULL)
      error(RES_MEM);
    nueva[i]->aptitud=0;
    if((nueva[i]->valores_salida=(double *)malloc(predes.n_nodos_salida*sizeof(double)))==NULL)
      error(RES_MEM);
    if((nueva[i]->nodulos=(nodulo **)malloc(pnodulos.n_subpobl*sizeof(nodulo)))==NULL)
      error(RES_MEM);
    /*Seleccionar una red aleatoriamente*/
    redsel=random()%predes.n_redes;
    
    /*selecci�n aleatoria del n�dulo a a�adir*/
    nodsel=random()%num_nodulos;

    /*creaci�n de la nueva red*/
    for(j=0;j<pnodulos.n_subpobl-1;j++)
      nueva[i]->nodulos[j]=predes.redes[redsel]->nodulos[j];
    nueva[i]->nodulos[pnodulos.n_subpobl-1]=pnodulos.nodulos[(pnodulos.n_subpobl-1)*num_nodulos+nodsel];
  }

  /*La nueva red se hace fija*/
  for(i=0;i<predes.n_redes;i++)
    liberarRed(predes.redes[i]);
  free(predes.redes);
  predes.redes=nueva;
  nueva=NULL;
  free(nueva);
}

/*******************************************************************************
* Fichero: crearPoblaciones.c						       *
*									       *
* Funci�n: liberarRed()							       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Funci�n que libera una red dada.		       *
*									       *
* Par�metros de Entrada:						       *
* 	borrar: Red. Red que se va a borrar.				       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 									       *
* Par�metros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:	NINGUNA				       *
*									       *
*******************************************************************************/

void liberarRed(red *borrar)
{
  int i;
  
  /*Se borran la red por completo. */
  for(i=0;i<pnodulos.n_subpobl;i++)
    borrar->nodulos[i]=NULL;
  free(borrar->nodulos);
  free(borrar->valores_salida);
  free(borrar);
}

/*******************************************************************************
* Fichero: crearPoblaciones.c						       *
*									       *
* Funci�n: crearNodulos()						       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Crea una nueva subpoblaci�n de n�dulos.	       *
*									       *
* Par�metros de Entrada: NINGUNO					       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	k: Entero. Contador.						       *
* 									       *
* Par�metros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:					       *
*	error()->Funcion que imprime por pantalla un mensaje de error.	       *
*									       *
*******************************************************************************/

void crearNodulos()
{
  int i,j,k;

  /*Se crea la subpoblaci�n de n�dulos*/
  for(i=pnodulos.n_nodulos-num_nodulos;i<pnodulos.n_nodulos;i++){
    if((pnodulos.nodulos[i]=(nodulo *)malloc(sizeof(nodulo)))==NULL)
      error(RES_MEM);

    
    pnodulos.nodulos[i]->id=i;   /* Identificador del n�dulo. */
    pnodulos.nodulos[i]->n_nodos=random()%max_nodos;     /* N�mero de nodos ocultos
							   del n�dulo. */
    if(pnodulos.nodulos[i]->n_nodos==0)
      pnodulos.nodulos[i]->n_nodos++;
    pnodulos.nodulos[i]->aptitud=0; /* Aptitud inicial. */
  
    /* Reserva de memoria para la creaci�n de conexiones y pesos. */
    /* Conexiones nodos de entrada->nodos ocultos. */
    if((pnodulos.nodulos[i]->conexiones_entrada=(int **)malloc(predes.n_nodos_entrada*sizeof(int)))==NULL)
      error(RES_MEM);

    /* Conexiones nodos ocultos->nodos de salida. */
    if((pnodulos.nodulos[i]->conexiones_salida=(int **)malloc(max_nodos*sizeof(int)))==NULL)
      error(RES_MEM);

    /* Pesos de conexiones nodos de entrada->nodos ocultos. */
    if((pnodulos.nodulos[i]->pesos_entrada=(double **)malloc(predes.n_nodos_entrada*sizeof(double)))==NULL)
      error(RES_MEM);

    /* Pesos de conexiones nodos ocultos->nodos de salida. */
    if((pnodulos.nodulos[i]->pesos_salida=(double **)malloc(max_nodos*sizeof(double)))==NULL)
      error(RES_MEM);

    if((pnodulos.nodulos[i]->salidas_parciales=(double **)malloc(n_entrenamiento*sizeof(double)))==NULL)
      error(RES_MEM);

    for(j=0;j<n_entrenamiento;j++){
      if((pnodulos.nodulos[i]->salidas_parciales[j]=(double *)malloc(predes.n_nodos_salida*sizeof(double)))==NULL)
        error(RES_MEM);
      for(k=0;k<predes.n_nodos_salida;k++)
        pnodulos.nodulos[i]->salidas_parciales[j][k]=0.0;
    }
  
    /* Creaci�n de conexiones junto con sus pesos. */
    /* Conexiones y pesos de entrada. */
    for(j=0;j<predes.n_nodos_entrada;j++){
      if((pnodulos.nodulos[i]->conexiones_entrada[j]=(int *)malloc(max_nodos*sizeof(int)))==NULL)
        error(RES_MEM);

      if((pnodulos.nodulos[i]->pesos_entrada[j]=(double *)malloc(max_nodos*sizeof(double)))==NULL)
        error(RES_MEM);

      for(k=0;k<max_nodos;k++){
        if(random()%2==1 && k<pnodulos.nodulos[i]->n_nodos){
          pnodulos.nodulos[i]->conexiones_entrada[j][k]=1;
	  pnodulos.nodulos[i]->pesos_entrada[j][k]=aleatorio()/2;
        }else{
          pnodulos.nodulos[i]->conexiones_entrada[j][k]=0;
	  pnodulos.nodulos[i]->pesos_entrada[j][k]=0.0;
        }
      }
    }

    /* Conexiones y pesos de salida. */
    for(j=0;j<max_nodos;j++){
      if((pnodulos.nodulos[i]->conexiones_salida[j]=(int *)malloc(predes.n_nodos_salida*sizeof(int)))==NULL)
        error(RES_MEM);

      if((pnodulos.nodulos[i]->pesos_salida[j]=(double *)malloc(predes.n_nodos_salida*sizeof(double)))==NULL)
        error(RES_MEM);

      for(k=0;k<predes.n_nodos_salida;k++){
        if(random()%2==1 && j<pnodulos.nodulos[i]->n_nodos){
	  pnodulos.nodulos[i]->conexiones_salida[j][k]=1;
          pnodulos.nodulos[i]->pesos_salida[j][k]=aleatorio()/2;
        }else{
	  pnodulos.nodulos[i]->conexiones_salida[j][k]=0;
          pnodulos.nodulos[i]->pesos_salida[j][k]=0.0;
        }
      }
    }
 
    /*Asignaci�n de la funci�n de transferencia a cada nodo*/
    if((pnodulos.nodulos[i]->transf=(func *)malloc(max_nodos*sizeof(func)))==NULL)
      error(RES_MEM);
  
    for(j=0;j<pnodulos.nodulos[i]->n_nodos;j++){
      if(random()%2==0)
        pnodulos.nodulos[i]->transf[j]=(func)&HyperbolicTangent;
      else
        pnodulos.nodulos[i]->transf[j]=(func)&Logistic;
    }
  }
}

/*******************************************************************************
* Fichero: crearPoblaciones.c						       *
*									       *
* Funci�n: crearPoblacionNodulos()					       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Crea o amplia la poblaci�n de n�dulos.	       *
*									       *
* Par�metros de Entrada: NINGUNO					       *
* 									       *
* Par�metros Internos: NINGUNO						       *
* 									       *
* Par�metros de Salida: NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:					       *
*	crearNodulos()->Funci�n que crea una nueva subpoblaci�n de n�dulos.    *
*	error()->Funcion que imprime por pantalla un mensaje de error.	       *
*									       *
*******************************************************************************/

void crearPoblacionNodulos()
{
  /* Se actualizan el n�mero de subpoblaciones y el n�mero de n�dulos de la poblaci�n de n�dulos. */
  pnodulos.n_subpobl++;
  pnodulos.n_nodulos=pnodulos.n_subpobl*num_nodulos;
  /* Se reserva memoria para la nueva subpoblaci�n. */
  if(pnodulos.n_subpobl==1){
    if((pnodulos.nodulos=(nodulo **)malloc(num_nodulos*sizeof(nodulo)))==NULL)
      error(RES_MEM);
  }else{
    if((pnodulos.nodulos=(nodulo **)realloc(pnodulos.nodulos,pnodulos.n_nodulos*sizeof(nodulo)))==NULL)
      error(RES_MEM);
  }

  /* Se crean los n�dulos de la nueva subpoblaci�n. */
  crearNodulos();
}

/*******************************************************************************
* Fichero: crearPoblaciones.c						       *
*									       *
* Funcion: crearRedes()							       *
*									       *
* Autor: Pablo Alvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funcion: Se crea una nueva poblacion de redes.	       *
*									       *
* Parametros de Entrada: NINGUNO					       *
*									       *
* Parametros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
*									       *
* Parametros de Salida:	NINGUNO						       *
*									       *
* Funciones a las que llama la funcion:					       *
*	error()->Funcion que imprime por pantalla un mensaje de error.	       *
*									       *
*******************************************************************************/

void crearRedes()
{
  int i,j;
  
  for(i=0;i<num_nodulos;i++){
    if((predes.redes[i]=(red *)malloc(sizeof(red)))==NULL)
      error(RES_MEM);

    /* Se reserva memoria y se inicializan las variables */
    predes.redes[i]->aptitud=0.0;
    if((predes.redes[i]->nodulos=(nodulo **)malloc(sizeof(nodulo)))==NULL)
      error(RES_MEM);

    /* Se le asigna la red su primer n�dulo. */
    predes.redes[i]->nodulos[0]=pnodulos.nodulos[i];
    if((predes.redes[i]->valores_salida=(double *)malloc(predes.n_nodos_salida*sizeof(double)))==NULL)
      error(RES_MEM);

    for(j=0;j<predes.n_nodos_salida;j++)
      predes.redes[i]->valores_salida[j]=0.0;
  }
}
