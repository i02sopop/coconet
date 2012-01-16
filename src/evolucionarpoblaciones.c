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

/*****************************************************************************
 File: evolucionarPoblaciones.c
 Function: evolucionarPoblaciones()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Evolve the networks and nodules populations.
 Input Parameters: None
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   k: Integer. Counter.
   aptitudRedes: Float. Store the average aptitude of the networks.
 Return Value: None
 Calling Functions:
   retropropagacion(): Run the backpropagation algorithm over a given nodule.
   generarSalidaNodulo(): Generate the outout of a nodule with an input data
                          given.
   generarSalidaRed(): Generate the output of a network combining the nodule
                       outputs.
   medirAptitudRed(): Measure the network aptitude.
   medirAptitudNodulos(): Measure the nodule aptitude.
   normalizarAptitudNodulos(): Normalize the nodule aptitude.
   ordenarRedes(): Sort the networks by aptitude.
   enfriamientoSimulado(): Run the simulated annealing over a nodule.
   ordenarNodulos(): Sort the nodules by aptitude.
   copiarDescendencia(): Make a copy of the nodules subpopulation.
   mutarNodulos(): Make an structural mutation of a nodule.
   sustituirNodulos(): Substitution of the worst nodules by the mutation of
                       the best nodules in the subpopulation.
*****************************************************************************/

void evolucionarPoblaciones()
{
  int i,j,k;
  double aptitudRedes;

  /*Se inicializan las variables*/
  aptitudRedes=0.0;
  for(i=0;i<predes.n_redes;i++)
    predes.redes[i]->aptitud=0;
  /*Se hace la evolución hasta que la aptitud media de las redes no mejore lo suficiente*/
  for(i=0;medirCambio(&aptitudRedes,i)==false;i++)
  {
    if(i==0)
    {
      /*Si se acaba de crear una nueva subpoblación de nódulos se ejecuta la retropropagación para que reduzca su error*/
      fprintf(stderr,"retropropagacion\n");
      for(j=0;j<num_nodulos;j++)
        retropropagacion(num_nodulos*(pnodulos.n_subpobl-1)+j,n_entrenamiento,5000);
      fprintf(stderr,"Entrenar\n");
      /*Se entrenan por primera vez las redes para tener un valor inicial de la aptitud de los nódulos y de las redes*/
      for(j=0;j<n_entrenamiento;j++)
      {
        /*Se genera la salida de los nódulos*/
        for(k=pnodulos.n_nodulos-num_nodulos;k<pnodulos.n_nodulos;k++)
          generarSalidaNodulo(entrada[j],k,j,NULL);
	/*Se mide la aptitud de las redes*/
	for(k=0;k<predes.n_redes;k++)
        {
          generarSalidaRed(k,j);
          medirAptitudRed(salida[j],k);
        }
      }
      /*Se normalizan las aptitudes de las redes*/
      for(j=0;j<predes.n_redes;j++)
        predes.redes[j]->aptitud/=n_entrenamiento;
      /*Se miden y se normalizan las aptitudes de los nódulos*/
      for(j=0;j<pnodulos.n_nodulos;j++)
        medirAptitudNodulos(j);
      normalizarAptitudNodulos();
    }
    /*Se ordenan las redes según su aptitud*/
    fprintf(stderr,"ordenar redes\n");
    ordenarRedes();
    /*Se entrenan las redes mediante el enfriamiento simulado*/
    fprintf(stderr,"enfriamiento\n");
    for(j=num_nodulos*(pnodulos.n_subpobl-1);j<pnodulos.n_nodulos;j++)
      enfriamientoSimulado(j);
    /*Se normalizan las aptitudes de los nódulos*/
    fprintf(stderr,"normalizar aptitud nodulos\n");
    normalizarAptitudNodulos();
    /*Se ordenan los nódulos según su aptitud*/
    fprintf(stderr,"ordenar nodulos\n");
    ordenarNodulos();
    /*Se crea una población descendente a partir de la actual*/
    fprintf(stderr,"copiar descendencia\n");
    copiarDescendencia();
    /*Se mutan estructuralmente los nódulos*/
    fprintf(stderr,"mutar nodulos\n");
    for(j=num_nodulos*(pnodulos.n_subpobl-1);j<pnodulos.n_nodulos;j++)
      mutarNodulos(j);
    /*Se sustituyen los peores nódulos de la población padre por los mejores nódulos de la población descendente*/
    fprintf(stderr,"Sustituir peores nodulos\n");
    sustituirNodulos();
    fprintf(stderr,"Fin de la evolucion\n");
  }
}

/*******************************************************************************
* Fichero: evolucionarPoblaciones.c					       *
*									       *
* Función: retropropagacion()						       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Ejecuta el algoritmo de retropropagación sobre un   *
*			   nódulo determinado.				       *
*									       *
* Parámetros de Entrada: 						       *
* 	numNodulo: Entero. Número del nódulo a tratar.			       *
* 	n_patrones: Entero. Número de patrones de entrada.		       *
* 	iteraciones: Entero. Número de iteraciones que va a tener el algoritmo.*
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	iter: Entero. Contador.						       *
* 	patron: Entero. Número de patrón elegido para ejecutar la	       *
*		retropropagación.					       *
* 	numNodos: Entero. Número de nodos que tiene el nódulo.		       *
* 	einicial: Vector de reales. Almacena el error inicial del nódulo.     *
* 	F_W: Matriz de reales. Almacena el cambio que se debe de introducir en *
*	     los pesos del nódulo.					       *
* 	out: Vector de reales. Almacena la salida de cada nodo del nódulo.     *
* 	pesos: Matriz de reales. Almacena los pesos de conexión de todo el     *
*	       nódulo.							       *
* 	transf: Vector de funciones. Almacena las funciones de transferencia de*
*		todos los nodos del nódulo.				       *
* 									       *
* Parámetros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la función:					       *
* 	generarSalidaNodulo()->Genera la salida de un nódulo a partir de un    *
*			       patrón de entrada.			       *
*	cambioPesos()->Función que calcula el valor en que tiene que cambiar   *
*		       cada peso para reducir el error.			       *
*	error()->Funcion que imprime por pantalla un mensaje de error.	       *
*									       *
*******************************************************************************/

void retropropagacion(int numNodulo,int n_patrones,int iteraciones)
{
  int i,j,iter,patron,numNodos;
  double *einicial,**F_W,*out,**pesos;
  func *transf;

  /*Se inicializan todas las variables*/
  numNodos=pnodulos.nodulos[numNodulo]->n_nodos+predes.n_nodos_entrada+predes.n_nodos_salida;
  if((transf=(func *)malloc((predes.n_nodos_salida+pnodulos.nodulos[numNodulo]->n_nodos)*sizeof(func)))==NULL)
    error(RES_MEM);
  if((out=(double *)malloc(numNodos*sizeof(double)))==NULL)
      error(RES_MEM);
  if((F_W=(double **)malloc(numNodos*sizeof(double)))==NULL)
    error(RES_MEM);
  if((pesos=(double **)malloc(numNodos*sizeof(double)))==NULL)
    error(RES_MEM);
  if((einicial=(double *)malloc(predes.n_nodos_salida*sizeof(double)))==NULL)
    error(RES_MEM);
  for(i=0;i<numNodos;i++)
  {
    if((F_W[i]=(double *)malloc(numNodos*sizeof(double)))==NULL)
      error(RES_MEM);
    if((pesos[i]=(double *)malloc(numNodos*sizeof(double)))==NULL)
      error(RES_MEM);
    for(j=0;j<numNodos;j++)
    {
      F_W[i][j]=0.0;
      pesos[i][j]=0.0;
    }
  }
  for (i=0;i<pnodulos.nodulos[numNodulo]->n_nodos;i++)
    transf[i]=pnodulos.nodulos[numNodulo]->transf[i];
  for (;i<pnodulos.nodulos[numNodulo]->n_nodos+predes.n_nodos_salida;i++)
    transf[i]=net_transf;

  /* Retropropagación. */
  for(iter=0;iter<iteraciones;iter++)
  {
    /* Seleccionar un patrón. */
    patron=random()%n_patrones;
    generarSalidaNodulo(entrada[patron],numNodulo,patron, out);

    /*Se asignan los pesos a la matriz de pesos. */
    for(j=0;j<predes.n_nodos_salida;j++)
      einicial[j]=out[j+predes.n_nodos_entrada+pnodulos.nodulos[numNodulo]->n_nodos]-salida[patron][j];
    for(i=0;i<pnodulos.nodulos[numNodulo]->n_nodos;i++)
      for(j=0;j<predes.n_nodos_entrada;j++)
        pesos[i+predes.n_nodos_entrada][j]=pnodulos.nodulos[numNodulo]->pesos_entrada[j][i];
    for(i=0;i<predes.n_nodos_salida;i++)
      for(j=0;j<pnodulos.nodulos[numNodulo]->n_nodos;j++)
        pesos[i+predes.n_nodos_entrada+pnodulos.nodulos[numNodulo]->n_nodos][j+predes.n_nodos_entrada]=pnodulos.nodulos[numNodulo]->pesos_salida[j][i];
    /* Obtener el cambio de pesos. */
    cambioPesos(einicial,pesos,out,F_W,pnodulos.nodulos[numNodulo]->n_nodos,transf);

    /* Se realiza la actualización de pesos. */
    for(i=0;i<pnodulos.nodulos[numNodulo]->n_nodos;i++)
      for(j=0;j<predes.n_nodos_entrada;j++)
        pnodulos.nodulos[numNodulo]->pesos_entrada[j][i]-=alpharet*F_W[i+predes.n_nodos_entrada][j];
    for(i=0;i<predes.n_nodos_salida;i++)
      for(j=0;j<pnodulos.nodulos[numNodulo]->n_nodos;j++)
        pnodulos.nodulos[numNodulo]->pesos_salida[j][i]-=alpharet*F_W[i+predes.n_nodos_entrada+pnodulos.nodulos[numNodulo]->n_nodos][j+predes.n_nodos_entrada];
  }
  free(einicial);
  free(out);
  free(transf);
  for(i=0;i<numNodos;i++)
  {
    free(pesos[i]);
    free(F_W[i]);
  }
  free(pesos);
  free(F_W);
}

/*******************************************************************************
* Fichero: evolucionarPoblaciones.c					       *
*									       *
* Función: cambioPesos()						       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Calcula el valor que tiene que cambiar cada peso    *
*			   para reducir el error.			       *
*									       *
* Parámetros de Entrada: 						       *
* 	nodos: Entero. Número de nodos que tiene el nódulo.		       *
* 	einicial: Vector de reales. Almacena el error inicial del nódulo.     *
* 	F_W: Matriz de reales. Almacena el cambio que se debe de introducir en *
*	     los pesos del nódulo.					       *
* 	out: Vector de reales. Almacena la salida de cada nodo del nódulo.     *
* 	pesos: Matriz de reales. Almacena los pesos de conexión de todo el     *
*	       nódulo.							       *
* 	transf: Vector de funciones. Almacena las funciones de transferencia de*
*		todos los nodos del nódulo.				       *
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	N: Entero. Número de nodos de entrada y ocultos quetiene el nódulo.    *
* 	enodo: Vector de reales. Error de salida de un nódulo.		       *
* 	ered: Vector de reales. Error de la red en un nodo determinado.        *
* 									       *
* Parámetros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la función:	NINGUNA				       *
*	error()->Funcion que muestra un mensaje de error por pantalla	       *
*									       *
*******************************************************************************/

void cambioPesos(double *einicial,double **pesos,double *out,double **F_W,int nodos,func *transf)
{
  int i,j,N;
  double *enodo,*ered;

  N=predes.n_nodos_entrada+nodos;
  if((enodo=(double *)malloc((N+predes.n_nodos_salida)*sizeof(double)))==NULL)
    error(RES_MEM);
  if((ered=(double *)malloc((N+predes.n_nodos_salida)*sizeof(double)))==NULL)
    error(RES_MEM);

  /* Se inicializan las variables. */

  for(i=0;i<N;i++)
    enodo[i]=0.0;

  for(i=0;i<predes.n_nodos_salida;i++)
    enodo[i+N]=einicial[i];

  /* Se obtienen las actualizaciones. */
  for(i=N+predes.n_nodos_salida-1;i>=predes.n_nodos_entrada;i--)
  {
    for(j=i+1;j<N+predes.n_nodos_salida;j++)
      enodo[i]+=pesos[j][i]*ered[j];

    if(transf[i-predes.n_nodos_entrada]==(func)&Logistic)
      ered[i]=enodo[i]*ptransferencia.logistic_b*out[i]*(1.0-out[i]/ptransferencia.logistic_a);
    else if (transf[i-predes.n_nodos_entrada]==(func)&HyperbolicTangent)
      ered[i]=enodo[i]*(ptransferencia.htan_b/ptransferencia.htan_a)*(ptransferencia.htan_a-out[i])*(ptransferencia.htan_a+out[i]);

    for(j=0;j<i;j++)
      F_W[i][j]=ered[i]*out[j];
  }
}

/*******************************************************************************
* Fichero: evolucionarPoblaciones.c					       *
*									       *
* Función: enfriamientoSimulado()					       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Ejecuta el algoritmo del enfriaiento simulado sobre *
*			   un nódulo dado.				       *
*									       *
* Parámetros de Entrada:						       *
* 	numNodulo: Entero. Número del nódulo que se va a tratar.	       *
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	k: Entero. Contador.						       *
* 	pasos: Entero. Número de iteraciones que va a tener el enfriamiento    *
*	       simulado.						       *
* 	T: Real. Temperatura del enfriamiento simulado.			       *
* 	pesos: Vector de reales. Pesos antiguos.			       *
* 	apt_antigua: Real. Aptitud antigua del nódulo.			       *
* 									       *
* Parámetros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la función:					       *
*	pasoAleatorio()->Da un paso aleatorio en los pesos de conexión.	       *
*	medirCambioNodulo()->Mide el cambio de aptitud que ha tenido el nódulo.*
*									       *
*******************************************************************************/

void enfriamientoSimulado(int numNodulo)
{
  int i,j,k,pasos;
  double T,*pesos,apt_antigua;

  T=ToSA;
  pesos=NULL;
  apt_antigua=pnodulos.nodulos[numNodulo]->aptitud;
  
  /*Enfriamiento simulado*/
  for(pasos=0;pasos<iteraciones_sa;pasos++)
  {
    /*Se guardan los pesos antiguos*/
    k=0;
    /*Se guardan los pesos de entrada*/
    for(i=0;i<predes.n_nodos_entrada;i++)
      for(j=0;j<pnodulos.nodulos[numNodulo]->n_nodos;j++)
        if(pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j])
        {
          pesos=(double *)realloc(pesos,(k+1)*sizeof(double));
          pesos[k]=pnodulos.nodulos[numNodulo]->pesos_entrada[i][j];
          k++;
        }

    /*Se guardan los pesos de salida*/
    for(i=0;i<pnodulos.nodulos[numNodulo]->n_nodos;i++)
      for(j=0;j<predes.n_nodos_salida;j++)
        if(pnodulos.nodulos[numNodulo]->conexiones_salida[i][j])
        {
          pesos=(double *)realloc(pesos,(k+1)*sizeof(double));
          pesos[k]=pnodulos.nodulos[numNodulo]->pesos_salida[i][j];
          k++;
        }

    /*Se realiza un paso aleatorio*/
    pasoAleatorio(numNodulo);

    /*Se calcula la aptitud del nuevo nodulo*/
    medirCambioNodulo(numNodulo);
      
    /*Si la aptitud ha empeorado mucho se rechaza el cambio*/
    if((apt_antigua>pnodulos.nodulos[numNodulo]->aptitud) && aleatorio()<(apt_antigua-pnodulos.nodulos[numNodulo]->aptitud)*T)
    {
      /*Se recuperan los pesos antiguos*/
      k=0;
      /*Se recuperan los pesos de entrada*/
      for(i=0;i<predes.n_nodos_entrada;i++)
        for(j=0;j<pnodulos.nodulos[numNodulo]->n_nodos;j++)
          if(pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j]==1)
	  {
 	    pnodulos.nodulos[numNodulo]->pesos_entrada[i][j]=pesos[k];
	    k++;
	  }

      /*Se recuperan los pesos de salida*/
      for(i=0;i<pnodulos.nodulos[numNodulo]->n_nodos;i++)
	for(j=0;j<predes.n_nodos_salida;j++)
	  if(pnodulos.nodulos[numNodulo]->conexiones_salida[i][j]==1)
	  {
	    pnodulos.nodulos[numNodulo]->pesos_salida[i][j]=pesos[k];
	    k++;
	  }
      
      /*Se calcula la aptitud del nuevo nodulo*/
      medirCambioNodulo(numNodulo);
    }
    else
      apt_antigua=pnodulos.nodulos[numNodulo]->aptitud;
    /*Se actualiza la temperatura de la aptitud*/
    T = alphasa * T;
  }
  
  free(pesos);
}

/*******************************************************************************
* Fichero: evolucionarPoblaciones.c					       *
*									       *
* Función: pasoAleatorio()						       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Da un paso aleatorio en cada peso del nódulo.       *
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
*	Normal()->genera un valor aleatorio dentro de una distribución normal. *
*									       *
*******************************************************************************/

void pasoAleatorio(int numNodulo)
{
  int i,j;

  /*Se da un paso aleatorio en los pesos de las conexiones de entrada*/
  for(i=0;i<predes.n_nodos_entrada;i++)
    for(j=0;j<pnodulos.nodulos[numNodulo]->n_nodos;j++)
      if(pnodulos.nodulos[numNodulo]->conexiones_entrada[i][j]==1)
        pnodulos.nodulos[numNodulo]->pesos_entrada[i][j]+=Normal(0.0,1.0);

  /*Se da un paso aleatorio en los pesos de las conexiones de salida*/
  for(i=0;i<pnodulos.nodulos[numNodulo]->n_nodos;i++)
    for(j=0;j<predes.n_nodos_salida;j++)
      if(pnodulos.nodulos[numNodulo]->conexiones_salida[i][j]==1)
        pnodulos.nodulos[numNodulo]->pesos_salida[i][j]+=Normal(0.0,1.0);  
}
