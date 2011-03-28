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
* Fichero: evolucionarPoblaciones.c					       *
*									       *
* Funci�n: evolucionarPoblaciones()					       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Evoluciona las poblaciones de redes y de n�dulos.   *
*									       *
* Par�metros de Entrada: NINGUNO					       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	k: Entero. Contador.						       *
* 	aptitudRedes: Real. Almacena la aptitud media de las redes.	       *
* 									       *
* Par�metros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:					       *
*	retropropagacion()->Ejecuta el algoritmo de retropropagaci�n sobre un  *
*			    n�dulo dado.  				       *
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
* 	ordenarRedes()->Ordena las redes en funci�n de su aptitud.	       *
* 	enfriamientoSimulado()->Ejecuta el enfriamiento simulado sobre un      *
*				n�dulo determinado.			       *
* 	ordenarNodulos()->Ordena los n�dulos en funci�n de su aptitud.	       *
* 	copiarDescendencia()->Hace una copia de la subpoblaci�n de n�dulos.    *
* 	mutarNodulos()->Muta estructuralmente un n�dulo.		       *
* 	sustituirNodulos()->Sustituye los peores n�dulos de la subpoblaci�n por*
*			    la mutaci�n de los mejores n�dulos de la	       *
*			    subpoblaci�n.				       *
* 									       *
*******************************************************************************/

void evolucionarPoblaciones()
{
  int i,j,k;
  double aptitudRedes;

  /*Se inicializan las variables*/
  aptitudRedes=0.0;
  for(i=0;i<predes.n_redes;i++)
    predes.redes[i]->aptitud=0;
  /*Se hace la evoluci�n hasta que la aptitud media de las redes no mejore lo suficiente*/
  for(i=0;medirCambio(&aptitudRedes,i)==false;i++)
  {
    if(i==0)
    {
      /*Si se acaba de crear una nueva subpoblaci�n de n�dulos se ejecuta la retropropagaci�n para que reduzca su error*/
      fprintf(stderr,"retropropagacion\n");
      for(j=0;j<num_nodulos;j++)
        retropropagacion(num_nodulos*(pnodulos.n_subpobl-1)+j,n_entrenamiento,5000);
      fprintf(stderr,"Entrenar\n");
      /*Se entrenan por primera vez las redes para tener un valor inicial de la aptitud de los n�dulos y de las redes*/
      for(j=0;j<n_entrenamiento;j++)
      {
        /*Se genera la salida de los n�dulos*/
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
      /*Se miden y se normalizan las aptitudes de los n�dulos*/
      for(j=0;j<pnodulos.n_nodulos;j++)
        medirAptitudNodulos(j);
      normalizarAptitudNodulos();
    }
    /*Se ordenan las redes seg�n su aptitud*/
    fprintf(stderr,"ordenar redes\n");
    ordenarRedes();
    /*Se entrenan las redes mediante el enfriamiento simulado*/
    fprintf(stderr,"enfriamiento\n");
    for(j=num_nodulos*(pnodulos.n_subpobl-1);j<pnodulos.n_nodulos;j++)
      enfriamientoSimulado(j);
    /*Se normalizan las aptitudes de los n�dulos*/
    fprintf(stderr,"normalizar aptitud nodulos\n");
    normalizarAptitudNodulos();
    /*Se ordenan los n�dulos seg�n su aptitud*/
    fprintf(stderr,"ordenar nodulos\n");
    ordenarNodulos();
    /*Se crea una poblaci�n descendente a partir de la actual*/
    fprintf(stderr,"copiar descendencia\n");
    copiarDescendencia();
    /*Se mutan estructuralmente los n�dulos*/
    fprintf(stderr,"mutar nodulos\n");
    for(j=num_nodulos*(pnodulos.n_subpobl-1);j<pnodulos.n_nodulos;j++)
      mutarNodulos(j);
    /*Se sustituyen los peores n�dulos de la poblaci�n padre por los mejores n�dulos de la poblaci�n descendente*/
    fprintf(stderr,"Sustituir peores nodulos\n");
    sustituirNodulos();
    fprintf(stderr,"Fin de la evolucion\n");
  }
}

/*******************************************************************************
* Fichero: evolucionarPoblaciones.c					       *
*									       *
* Funci�n: retropropagacion()						       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Ejecuta el algoritmo de retropropagaci�n sobre un   *
*			   n�dulo determinado.				       *
*									       *
* Par�metros de Entrada: 						       *
* 	numNodulo: Entero. N�mero del n�dulo a tratar.			       *
* 	n_patrones: Entero. N�mero de patrones de entrada.		       *
* 	iteraciones: Entero. N�mero de iteraciones que va a tener el algoritmo.*
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	iter: Entero. Contador.						       *
* 	patron: Entero. N�mero de patr�n elegido para ejecutar la	       *
*		retropropagaci�n.					       *
* 	numNodos: Entero. N�mero de nodos que tiene el n�dulo.		       *
* 	einicial: Vector de reales. Almacena el error inicial del n�dulo.     *
* 	F_W: Matriz de reales. Almacena el cambio que se debe de introducir en *
*	     los pesos del n�dulo.					       *
* 	out: Vector de reales. Almacena la salida de cada nodo del n�dulo.     *
* 	pesos: Matriz de reales. Almacena los pesos de conexi�n de todo el     *
*	       n�dulo.							       *
* 	transf: Vector de funciones. Almacena las funciones de transferencia de*
*		todos los nodos del n�dulo.				       *
* 									       *
* Par�metros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:					       *
* 	generarSalidaNodulo()->Genera la salida de un n�dulo a partir de un    *
*			       patr�n de entrada.			       *
*	cambioPesos()->Funci�n que calcula el valor en que tiene que cambiar   *
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

  /* Retropropagaci�n. */
  for(iter=0;iter<iteraciones;iter++)
  {
    /* Seleccionar un patr�n. */
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

    /* Se realiza la actualizaci�n de pesos. */
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
* Funci�n: cambioPesos()						       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Calcula el valor que tiene que cambiar cada peso    *
*			   para reducir el error.			       *
*									       *
* Par�metros de Entrada: 						       *
* 	nodos: Entero. N�mero de nodos que tiene el n�dulo.		       *
* 	einicial: Vector de reales. Almacena el error inicial del n�dulo.     *
* 	F_W: Matriz de reales. Almacena el cambio que se debe de introducir en *
*	     los pesos del n�dulo.					       *
* 	out: Vector de reales. Almacena la salida de cada nodo del n�dulo.     *
* 	pesos: Matriz de reales. Almacena los pesos de conexi�n de todo el     *
*	       n�dulo.							       *
* 	transf: Vector de funciones. Almacena las funciones de transferencia de*
*		todos los nodos del n�dulo.				       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	N: Entero. N�mero de nodos de entrada y ocultos quetiene el n�dulo.    *
* 	enodo: Vector de reales. Error de salida de un n�dulo.		       *
* 	ered: Vector de reales. Error de la red en un nodo determinado.        *
* 									       *
* Par�metros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:	NINGUNA				       *
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
* Funci�n: enfriamientoSimulado()					       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Ejecuta el algoritmo del enfriaiento simulado sobre *
*			   un n�dulo dado.				       *
*									       *
* Par�metros de Entrada:						       *
* 	numNodulo: Entero. N�mero del n�dulo que se va a tratar.	       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	k: Entero. Contador.						       *
* 	pasos: Entero. N�mero de iteraciones que va a tener el enfriamiento    *
*	       simulado.						       *
* 	T: Real. Temperatura del enfriamiento simulado.			       *
* 	pesos: Vector de reales. Pesos antiguos.			       *
* 	apt_antigua: Real. Aptitud antigua del n�dulo.			       *
* 									       *
* Par�metros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:					       *
*	pasoAleatorio()->Da un paso aleatorio en los pesos de conexi�n.	       *
*	medirCambioNodulo()->Mide el cambio de aptitud que ha tenido el n�dulo.*
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
* Funci�n: pasoAleatorio()						       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Da un paso aleatorio en cada peso del n�dulo.       *
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
*	Normal()->genera un valor aleatorio dentro de una distribuci�n normal. *
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
