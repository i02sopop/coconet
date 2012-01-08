/******************************************************************************
 Copyright (c) 2004-2011 coconet project (see AUTHORS)

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
 File: mutacionEstructural.c
 Function: copiarDescendencia()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Copy the actual nodule population into a new descendant
              population.
 Input Parameters: None
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   k: Integer. Counter.
 Return Value: None
 Calling Functions:
   error(): Function to show an error message depending on an error number.
******************************************************************************/

void copiarDescendencia()
{
	int i, j, k, noduleOrig, initialNodule;

	/* Initialization of variables. */
	initialNodule = num_nodulos * (pnodulos.n_subpobl - 1);
	descendencia = (nodulo **)malloc(num_nodulos * sizeof(nodulo));
	if(descendencia == NULL)
		error(RES_MEM);

	/* We copy the nodules to the new population. */
	for(i = 0; i < num_nodulos; i++) {
		descendencia[i] = (nodulo *)malloc(sizeof(nodulo));
		if(descendencia[i] == NULL)
			error(RES_MEM);

		/* We copy the nodule from the old to the new population. */
		noduleOrig = initialNodule + i;
		descendencia[i]->id = pnodulos.nodulos[noduleOrig]->id;
		descendencia[i]->n_nodos = pnodulos.nodulos[noduleOrig]->n_nodos;
		descendencia[i]->aptitud = pnodulos.nodulos[noduleOrig]->aptitud;

		descendencia[i]->conexiones_salida = (int **)malloc(max_nodos * sizeof(int));
		descendencia[i]->pesos_salida = (double **)malloc(max_nodos * sizeof(double));
		descendencia[i]->transf = (func *)malloc(max_nodos * sizeof(func));
		if(descendencia[i]->conexiones_salida == NULL ||
		   descendencia[i]->pesos_salida == NULL ||
		   descendencia[i]->transf == NULL)
			error(RES_MEM);

		for(j = 0; j < max_nodos; j++) {
			descendencia[i]->conexiones_salida[j] = (int *)malloc(predes.n_nodos_salida * sizeof(int));
			descendencia[i]->pesos_salida[j] = (double *)malloc(predes.n_nodos_salida * sizeof(double));
			if(descendencia[i]->conexiones_salida[j] == NULL ||
				descendencia[i]->pesos_salida[j] == NULL)
				error(RES_MEM);

			descendencia[i]->transf[j] = pnodulos.nodulos[noduleOrig]->transf[j];

			for(k = 0; k < predes.n_nodos_salida; k++) {
				descendencia[i]->conexiones_salida[j][k] = pnodulos.nodulos[noduleOrig]->conexiones_salida[j][k];
				descendencia[i]->pesos_salida[j][k] = pnodulos.nodulos[noduleOrig]->pesos_salida[j][k];
			} /* end for */
		} /* end for */

		descendencia[i]->pesos_entrada = (double **)malloc(predes.n_nodos_entrada * sizeof(double));
		descendencia[i]->conexiones_entrada = (int **)malloc(predes.n_nodos_entrada * sizeof(int));
		if(descendencia[i]->pesos_entrada == NULL ||
			descendencia[i]->conexiones_entrada == NULL)
			error(RES_MEM);

		for(j = 0; j < predes.n_nodos_entrada; j++) {
			descendencia[i]->conexiones_entrada[j] = (int *)malloc(max_nodos * sizeof(int));
			descendencia[i]->pesos_entrada[j] = (double *)malloc(max_nodos * sizeof(double))
			if(descendencia[i]->conexiones_entrada[j] == NULL ||
				descendencia[i]->pesos_entrada[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < max_nodos; k++) {
				descendencia[i]->conexiones_entrada[j][k] = pnodulos.nodulos[noduleOrig]->conexiones_entrada[j][k];
				descendencia[i]->pesos_entrada[j][k] = pnodulos.nodulos[noduleOrig]->pesos_entrada[j][k];
			} /* end for */
		} /* end for */

		descendencia[i]->salidas_parciales = (double **)malloc(n_entrenamiento * sizeof(double));
		if(descendencia[i]->salidas_parciales == NULL)
			error(RES_MEM);

		for(j = 0; j < n_entrenamiento; j++) {
			descendencia[i]->salidas_parciales[j] = (double *)malloc(predes.n_nodos_salida * sizeof(double));
			if(descendencia[i]->salidas_parciales[j] == NULL)
				error(RES_MEM);

			for(k = 0; k < predes.n_nodos_salida; k++)
				descendencia[i]->salidas_parciales[j][k] = pnodulos.nodulos[noduleOrig]->salidas_parciales[j][k];
		} /* end for */
	} /* end for */
}

/******************************************************************************
 File: mutacionEstructural.c
 Function: mutarNodulos()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Make an estructural mutation in a given nodule.
 Input Parameters:
   nodule: Integer. Number of nodule to mutate.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   num: Integer. Number of mutation elements.
 Return Value: None
 Calling Functions: 
   anadirNodo(): Add a node to the nodule.
   borrarNodo(): Delete a node from the nodule.
   anadirConexion(): Add a connection to the nodule.
   borrarConexion(): Delete a connection from the nodule.
   medirCambioNodulo(): Calculate the nodules aptitude variation after the
                        mutation.
******************************************************************************/

void mutarNodulos(int nodule)
{
	int i, j, num;

	/* We made the estructural mutation. */
	/* Deletion of nodes. */
	num = (int)(delta_min + aleatorio() * (1 - pnodulos.nodulos[nodule]->aptitud) * (delta_min - delta_max));
	if(num < 0)
		num = 0;
	else if(pnodulos.nodulos[numNodulo]->n_nodos < num)
		num = pnodulos.nodulos[nodule]->n_nodos;
	else if(num > 0)
		borrarNodo(nodule, num);

	/* Add nodes. */
	num = (int)(delta_min + aleatorio() * (1 - pnodulos.nodulos[nodule]->aptitud) * (delta_min - delta_max));
	if(num < 0)
		num = 0;
	else if((pnodulos.nodulos[numNodulo]->n_nodos + num) > max_nodos)
		num = max_nodos - pnodulos.nodulos[nodule]->n_nodos;
	else if(num > 0)
		anadirNodo(nodule, num); /* We add a new node,*/

	/* Delete connections. */
	num = (int)(delta_min + aleatorio() * (1 - pnodulos.nodulos[nodule]->aptitud) * (delta_min - delta_max));
	if(num < 0)
		num = 0;

	for(i = 0; i <num && pnodulos.nodulos[nodule]->n_nodos > 0; i++) {
		j = random() % (predes.n_nodos_entrada + predes.n_nodos_salida);
		borrarConexion(nodule, j);
	}

	/* Add connections. */
	num = (int)(delta_min + aleatorio() * (1 - pnodulos.nodulos[nodule]->aptitud) * (delta_min - delta_max));
	if(num < 0)
		num = 0;

	for(i = 0; i < num && pnodulos.nodulos[nodule]->n_nodos > 0; i++)
		anadirConexion(nodule);

	/*
	  We make the calculations over the modified nodule, both his partial
	  outputs and his aptitude.
	*/
	medirCambioNodulo(nodule);
}

/******************************************************************************
 File: mutacionEstructural.c
 Function: anadirConexion()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Finalidad de la función: Añadir una conexión a un ódulo determinado.
 Input Parameters:
   numNodulo: Integer. Number of nodule to modify.
 Local Variables:
   i. Integer. Counter.
   j: Integer. Counter.
   sel: Integer. Type of connection to add (input, internal or outpunt).
   origen: Integer vector. Posibles nodos de origen para la conexión.
   destino: Integer vector. Posibles nodos de destino para la conexión.
   num: Integer. Counter.
   pos: Integer. Position to add the connection.
 Return Value: None
 Calling Functions
   error(): Function to show an error message depending on an error number.
******************************************************************************/

void anadirConexion(int numNodulo)
{
  int i,j,sel,*origen,*destino,num,pos;

  num=0;
  sel=random()%2;
  if((origen=(int *)malloc(sizeof(int)))==NULL)
    error(RES_MEM);

  if((destino=(int *)malloc(sizeof(int)))==NULL)
    error(RES_MEM);

  /*Se añade una conexión de entrada*/
  if(sel==0){
    for(i=0;i<predes.n_nodos_entrada;i++)
      for(j=0;j<pnodulos.nodulos[numNodulo]->n_nodos;j++)
        if(pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j]==0){
          origen=(int *)realloc(origen,(num+1)*sizeof(int));
          destino=(int *)realloc(destino,(num+1)*sizeof(int));
          origen[num]=i;
          destino[num]=j;
          num++;
        } /* end if */

    if(num!=0){
      pos=random()%num;
      pnodulos.nodulos[numNodulo]->conexiones_entrada[origen[pos]][destino[pos]]=1;
      pnodulos.nodulos[numNodulo]->pesos_entrada[origen[pos]][destino[pos]]=aleatorio()/2;
      free(origen);
      free(destino);
    } /* end if */
  } /* end if */

  /*Se añade una conexión de salida*/
  if((sel==1 && num==0) || num==0){
    for(i=0;i<pnodulos.nodulos[numNodulo]->n_nodos;i++)
      for(j=0;j<predes.n_nodos_salida;j++)
        if(pnodulos.nodulos[numNodulo]->conexiones_salida[i][j]==0){
          origen=(int *)realloc(origen,(num+1)*sizeof(int));
          destino=(int *)realloc(destino,(num+1)*sizeof(int));
          origen[num]=i;
          destino[num]=j;
          num++;
        } /* end if */

    if(num!=0){
      pos=random()%num;
      pnodulos.nodulos[numNodulo]->conexiones_salida[origen[pos]][destino[pos]]=1;
      pnodulos.nodulos[numNodulo]->pesos_salida[origen[pos]][destino[pos]]=aleatorio()/2;
      free(origen);
      free(destino);
    } /* end if */
  } /* end if */

  if(num==0){
    free(origen);
    free(destino);
  } /* end if */
}

/********************************************************************************
* Fichero: mutacionEstructural.c					        *
*									        *
* Función: anadirNodo()							        *
*									        *
* Autor: Pablo Álvarez de Sotomayor Posadillo				        *
*									        *
* Finalidad de la función: Añadir un número determinado de nodos a un nódulo.   *
*									        *
* Parámetros de Entrada:						        *
* 	numNodulo: Entero. Nümero del nódulo a tratar.			        *
* 	numNodos: Entero. Número de nodos que se van a añadir.		        *
* 									        *
* Parámetros Internos:							        *
* 	i: Entero. Contador.						        *
* 									        *
* Parámetros de Salida:	NINGUNO						        *
* 									        *
* Funciones a las que llama la función:	NINGUNA				        *
*									        *
********************************************************************************/

void anadirNodo(int numNodulo,int numNodos)
{
  int i;

  /*Se le asigna al nodo la función de transferencia*/
  for(i=0;i<numNodos;i++){
    if((random()%2)==0)
      pnodulos.nodulos[numNodulo]->transf[pnodulos.nodulos[numNodulo]->n_nodos+i]=(func)&HyperbolicTangent;
    else
      pnodulos.nodulos[numNodulo]->transf[pnodulos.nodulos[numNodulo]->n_nodos+i]=(func)&Logistic;
  }

  /*Se actualiza el número de nodos*/
  pnodulos.nodulos[numNodulo]->n_nodos+=numNodos;
}

/********************************************************************************
* Fichero: mutacionEstructural.c					        *
*									        *
* Función: borrarConexion()						        *
*									        *
* Autor: Pablo Álvarez de Sotomayor Posadillo				        *
*									        *
* Finalidad de la función: Borra una conexión de un nódulo determinado.	        *
*									        *
* Parámetros de Entrada:						        *
* 	numNodulo: Entero. Número del nódulo a tratar.			        *
* 	nodo: Entero. Nodo del que se va a borrar una conexión		        *
* 									        *
* Parámetros Internos:							        *
* 	i: Entero. Contador.						        *
* 	sel: Entero. Conexión a borrar.					        *
* 									        *
* Parámetros de Salida:	NINGUNO						        *
* 									        *
* Funciones a las que llama la función:	NINGUNA				        *
*									        *
********************************************************************************/

void borrarConexion(int numNodulo,int nodo)
{
  int i,sel;

  /*Se borra una conexión de entrada*/
  if(nodo<predes.n_nodos_entrada){
    for(i=0;i<pnodulos.nodulos[numNodulo]->n_nodos && \
	  pnodulos.nodulos[numNodulo]->conexiones_entrada[nodo][i]!=1;i++);
    if(i<pnodulos.nodulos[numNodulo]->n_nodos){
      sel=random()%pnodulos.nodulos[numNodulo]->n_nodos;
      for(;pnodulos.nodulos[numNodulo]->conexiones_entrada[nodo][sel]!=1;sel=random()%pnodulos.nodulos[numNodulo]->n_nodos);
      pnodulos.nodulos[numNodulo]->conexiones_entrada[nodo][sel]=0;
      pnodulos.nodulos[numNodulo]->pesos_entrada[nodo][sel]=0;
    } /* end if */
  }else{
    /*Se borra una conexión de salida*/
    nodo-=predes.n_nodos_entrada;
    for(i=0;i<pnodulos.nodulos[numNodulo]->n_nodos && \
	  pnodulos.nodulos[numNodulo]->conexiones_salida[i][nodo]!=1;i++);
    if(i<pnodulos.nodulos[numNodulo]->n_nodos){
      sel=random()%pnodulos.nodulos[numNodulo]->n_nodos;
      for(;pnodulos.nodulos[numNodulo]->conexiones_salida[sel][nodo]!=1;sel=random()%pnodulos.nodulos[numNodulo]->n_nodos);
      pnodulos.nodulos[numNodulo]->conexiones_salida[sel][nodo]=0;
      pnodulos.nodulos[numNodulo]->pesos_salida[sel][nodo]=0;
    } /* end if */
  } /* end if */
}

/********************************************************************************
* Fichero: mutacionEstructural.c					        *
*									        *
* Función: borrarNodo()							        *
*									        *
* Autor: Pablo Álvarez de Sotomayor Posadillo				        *
*									        *
* Finalidad de la función: Borra un número determinado de nodos de un nódulo    *
*			   dado.					        *
*									        *
* Parámetros de Entrada:						        *
* 	numNodulo: Entero. Número de nódulo que se va a borrar.		        *
* 	numNodos. Entero. Número de nodos a borrar.			        *
* 									        *
* Parámetros Internos:							        *
* 	i: Entero. Contador.						        *
* 	j: Entero. Contador.						        *
* 	k: Entero. Contador.						        *
* 	nodo: Entero. Nodo a borrar.					        *
* 									        *
* Parámetros de Salida:	NINGUNO						        *
* 									        *
* Funciones a las que llama la función:					        *
*	error()->Muestra un mensaje de error por pantalla.		        *
*									        *
********************************************************************************/

void borrarNodo(int numNodulo,int numNodos)
{
  int i,j,k,nodo;
  
  for(k=0;k<numNodos;k++){
    nodo=random()%pnodulos.nodulos[numNodulo]->n_nodos;
    /*Todos los nodos posteriores al nodo a borrar se adelantan un puesto en las 
      conexiones internas y de salida*/
    for(i=nodo+1;i<pnodulos.nodulos[numNodulo]->n_nodos;i++){
      pnodulos.nodulos[numNodulo]->transf[i-1]=pnodulos.nodulos[numNodulo]->transf[i];
      for(j=0;j<predes.n_nodos_salida;j++){
        pnodulos.nodulos[numNodulo]->conexiones_salida[i-1][j]=pnodulos.nodulos[numNodulo]->conexiones_salida[i][j];
        pnodulos.nodulos[numNodulo]->pesos_salida[i-1][j]=pnodulos.nodulos[numNodulo]->pesos_salida[i][j];
        pnodulos.nodulos[numNodulo]->conexiones_salida[i][j]=0;
        pnodulos.nodulos[numNodulo]->pesos_salida[i][j]=0;
      } /* end for */
    } /* end for */

    for(j=0;j<predes.n_nodos_salida;j++){
      pnodulos.nodulos[numNodulo]->conexiones_salida[i-1][j]=0;
      pnodulos.nodulos[numNodulo]->pesos_salida[i-1][j]=0;
    } /* end for */

    /*Todos los nodos posteriores al nodo a borrar se adelantan un puesto en las conexiones de entrada*/
    for(i=0;i<predes.n_nodos_entrada;i++){
      for(j=nodo+1;j<pnodulos.nodulos[numNodulo]->n_nodos;j++){
        pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j-1]=pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j];
        pnodulos.nodulos[numNodulo]->pesos_entrada[i][j-1]=pnodulos.nodulos[numNodulo]->pesos_entrada[i][j];
        pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j]=0;
        pnodulos.nodulos[numNodulo]->pesos_entrada[i][j]=0;
      } /* end for */
      pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j-1]=0;
      pnodulos.nodulos[numNodulo]->pesos_entrada[i][j-1]=0;
    } /* end for */
  
    pnodulos.nodulos[numNodulo]->n_nodos--;
  } /* end for */
}

/********************************************************************************
* Fichero: mutacionEstructural.c						*
*									        *
* Función: sustituirNodulos()						        *
*									        *
* Autor: Pablo Álvarez de Sotomayor Posadillo				        *
*									        *
* Finalidad de la función: Sustituye los peores nódulos de una población por    *
*			   los mejores de la población descendente.	        *
*									        *
* Parámetros de Entrada: NINGUNO					        *
* 									        *
* Parámetros Internos:							        *
* 	i: Entero. Contador.						        *
* 	j: Entero. Contador.						        *
* 	id: Vector de enteros. Almacena los id de los nódulos para ordenarlos   *
*	    por aptitud.						        *
* 	orden: Vector de reales. Almacena la aptitud de los nódulos para        *
*	       ordenarlos.						        *
* 									        *
* Parámetros de Salida:	NINGUNO						        *
* 									        *
* Funciones a las que llama la función:	NINGUNA				        *
*	liberarNodulo()->Libera la memoria ocupada por un nodulo determinado.   *
*	ordenarNodulos()->Se ordenan los nodulos de la poblacion.	        *
*	error()->Muestra un mensaje de error.				        *
*									        *
********************************************************************************/

void sustituirNodulos()
{
  int i,j,*id;
  double *orden;

  /*Se ordenan los nódulos por aptitud*/
  if((id=(int *)malloc(num_nodulos*sizeof(int)))==NULL)
    error(RES_MEM);

  if((orden=(double *)malloc(num_nodulos*sizeof(double)))==NULL)
    error(RES_MEM);

  for(i=0;i<num_nodulos;i++){
    id[i]=((pnodulos.n_subpobl-1)*num_nodulos)+i;
    orden[i]=pnodulos.nodulos[(pnodulos.n_subpobl-1)*num_nodulos+i]->aptitud;
  }

  for(i=0;i<num_nodulos;i++)
    for(j=i;j<num_nodulos;j++)
      if(orden[i]<orden[j]){
        id[i]=((pnodulos.n_subpobl-1)*num_nodulos)+j;
        id[j]=((pnodulos.n_subpobl-1)*num_nodulos)+i;
        orden[i]=pnodulos.nodulos[((pnodulos.n_subpobl-1)*num_nodulos)+j]->aptitud;
	orden[j]=pnodulos.nodulos[((pnodulos.n_subpobl-1)*num_nodulos)+i]->aptitud;
      }

/*Se sustituyen los mejores nódulos de la subpoblación hija por los peores nódulos de la subpoblación padre*/
  for(i=num_nodulos;i>nodsel;i--){
    for(j=0;j<predes.n_redes;j++)
      if(predes.redes[j]->nodulos[pnodulos.n_subpobl-1]==pnodulos.nodulos[id[i-1]])
        predes.redes[j]->nodulos[pnodulos.n_subpobl-1]=descendencia[i-1-nodsel];
    liberarNodulo(pnodulos.nodulos[id[i-1]]);
    free(pnodulos.nodulos[id[i-1]]);
    pnodulos.nodulos[id[i-1]]=descendencia[i-1-nodsel];
    descendencia[i-1-nodsel]=NULL;
  }

  for(i=num_nodulos-nodsel;i<num_nodulos;i++)
    liberarNodulo(descendencia[i]);
  free(descendencia);
  descendencia=NULL;
  ordenarNodulos();
  free(id);
  free(orden);
}
