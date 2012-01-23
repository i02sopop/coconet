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
#include <time.h>

/******************************************************************************
 File: ficheros.c
 Function: cargarFichero()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Load the input data from training and configuration files.
 Input Parameters:
   config: String. Name of the configuration file.
   entrenamiento: String. Name of the training file.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   fichero_configuracion:	Pointer to the configuration file.
   fichero_entrenamiento:	Pointer to the training file.
 Return Value: None
 Calling Functions:
   error(): Function to print an error message.
******************************************************************************/

void cargarFichero(char *config, char *entrenamiento)
{
	int i, j;
	FILE *fichero_configuracion, *fichero_entrenamiento;
  
	pnodulos.n_subpobl = 0;

	/* We open the configuration and training files. */
	fichero_configuracion = fopen(config,"r");
	fichero_entrenamiento = fopen(entrenamiento,"r");
	if(fichero_configuracion == NULL || fichero_entrenamiento == NULL)
		error(IO);
  
	if(fscanf(fichero_entrenamiento, "$ %d\n", &n_entrenamiento) == EOF ||
	   fscanf(fichero_entrenamiento, "$ %d\n", &predes.n_nodos_entrada) == EOF ||
	   fscanf(fichero_entrenamiento,"$ %d\n",&predes.n_nodos_salida) == EOF)
		error(IO);

	/* Loading the training data. */
	entrada = (double **)malloc(n_entrenamiento * sizeof(double));
	salida = (double **)malloc(n_entrenamiento * sizeof(double));
	if(entrada == NULL || salida == NULL)
		error(RES_MEM);

	for(i = 0; i < n_entrenamiento; i++) {
		entrada[i] = (double *)malloc(predes.n_nodos_entrada * sizeof(double));
		salida[i] = (double *)malloc(predes.n_nodos_salida * sizeof(double));
		if(entrada[i] == NULL || salida[i] == NULL)
			error(RES_MEM);

		for(j = 0; j < predes.n_nodos_entrada; j++)
			if(fscanf(fichero_entrenamiento, "%lf", &entrada[i][j]) == EOF)
				error(IO);
		
		for(j = 0; j < predes.n_nodos_salida; j++)
			if(fscanf(fichero_entrenamiento, "%lf", &salida[i][j]) == EOF)
				error(IO);
	}
  
	if(fscanf(fichero_configuracion, "Redes: %d\n", &max_redes) == EOF ||
	   fscanf(fichero_configuracion, "Nodos: %d\n", &max_nodos) == EOF ||
	   fscanf(fichero_configuracion, "Nodulos: %d\n", &num_nodulos) == EOF)
		error(IO);

	/* Parameters of the transfer functions. */
	if(fscanf(fichero_configuracion, "Htan a: %lf\n", &ptransferencia.htan_a) == EOF ||
	   fscanf(fichero_configuracion, "Htan b: %lf\n", &ptransferencia.htan_b) == EOF ||
	   fscanf(fichero_configuracion, "Logistic a: %lf\n", &ptransferencia.logistic_a) == EOF ||
	   fscanf(fichero_configuracion, "Logistic b: %lf\n", &ptransferencia.logistic_b) == EOF ||
	   fscanf(fichero_configuracion, "Epsilon: %lf\n", &ptransferencia.epsilon) == EOF)
		error(IO);

	/* Ponderation for nodule aptitude. */
	if(fscanf(fichero_configuracion, "Sustitucion: %lf\n", &pond.sust) == EOF ||
	   fscanf(fichero_configuracion, "Diferencia: %lf\n", &pond.dif) == EOF ||
	   fscanf(fichero_configuracion, "Mejores: %lf\n", &pond.best) == EOF)
		error(IO);

	if(fscanf(fichero_configuracion, "Redes Seleccionadas: %d\n", &redsel) == EOF ||
	   fscanf(fichero_configuracion, "Nodulos Seleccionados: %d\n", &nodsel) == EOF)
		error(IO);

	if(fscanf(fichero_configuracion, "Iteraciones SA: %d\n", &iteraciones_sa) == EOF ||
	   fscanf(fichero_configuracion, "To SA: %lf\n", &ToSA) == EOF ||
	   fscanf(fichero_configuracion, "alpha SA: %lf\n", &alphasa) == EOF)
		error(IO);

	if(fscanf(fichero_configuracion, "Delta Minimo: %d\n", &delta_min) == EOF ||
	   fscanf(fichero_configuracion, "Delta Maximo: %d\n", &delta_max) == EOF)
		error(IO);

	if(fscanf(fichero_configuracion, "Limite Evolucion: %lf\n", &evolim) == EOF ||
	   fscanf(fichero_configuracion, "Alpha Retropropagacion: %lf\n", &alpharet) == EOF)
		error(IO);

	if(fclose(fichero_configuracion) == EOF || fclose(fichero_entrenamiento) == EOF)
		error(IO);

	srandom(time(NULL));
	if(random() % 2 == 0)
		net_transf=(func)&HyperbolicTangent;
	else
		net_transf = (func)&Logistic;

	/* The max number of networks is as the max number of nodules per subpopulation. */
	if(max_redes < num_nodulos)
		max_redes = num_nodulos;

	if(nodsel > num_nodulos)
		nodsel = num_nodulos;
}

/******************************************************************************
 File: ficheros.c
 Function: leerGeneralizacion()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Read the input data to measure the generalization.
 Input Parameters:
 	generalizacion: String. Name of the generalization file.
 Local Variables:
 	i: Integer. Counter.
 	j: Integer. Counter.
 	fichero_generalizacion: Pointer to file. File that contains the input data.
 Return Value: None
 Calling Functions:
   error(): Function to print an error message.
*******************************************************************************/

void leerGeneralizacion(char *generalizacion)
{
	int i, j;
	FILE *fichero_generalizacion;

	/* We open the generalization file. */
	if((fichero_generalizacion = fopen(generalizacion,"r")) == NULL)
		error(IO);

	/* We load the generalization data. */
	if(fscanf(fichero_generalizacion, "$ %d\n", &n_generalizacion) == EOF ||
	   fscanf(fichero_generalizacion, "$ %d\n", &predes.n_nodos_entrada) == EOF ||
	   fscanf(fichero_generalizacion, "$ %d\n", &predes.n_nodos_salida) == EOF)
		error(IO);

	for(i = 0; i < n_entrenamiento; i++) {
		free(entrada[i]);
		free(salida[i]);
	}
	free(entrada);
	free(salida);

	entrada = (double **)malloc(n_generalizacion * sizeof(double));
	salida = (double **)malloc(n_generalizacion * sizeof(double));
	if(entrada == NULL || salida == NULL)
		error(RES_MEM);

	for(i = 0; i < n_generalizacion; i++) {
		entrada[i] = (double *)malloc(predes.n_nodos_entrada * sizeof(double));
		salida[i] = (double *)malloc(predes.n_nodos_salida * sizeof(double));
		if(entrada[i] == NULL || salida[i] == NULL)
			error(RES_MEM);

		for(j = 0; j < predes.n_nodos_entrada; j++)
			if(fscanf(fichero_generalizacion, "%lf", &entrada[i][j]) == EOF)
				error(IO);

		for(j = 0; j < predes.n_nodos_salida; j++)
			if(fscanf(fichero_generalizacion, "%lf", &salida[i][j]) == EOF)
				error(IO);
	}

	/* We close the file. */
	if(fclose(fichero_generalizacion) == EOF)
		error(IO);
}

/*******************************************************************************
* Fichero: ficheros.c							       *
*									       *
* Funcion: exportarMejorRed()						       *
*									       *
* Autor: Pablo Alvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funcion: Exporta la red que mejor se ha adaptado al problema *
*			   a un fichero de salida.			       *
*									       *
* Parametros de Entrada: 						       *
* 	ficheroSalida: Cadena de caracteres. Nombre del fichero de salida.     *
*									       *
* Parametros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	k: Entero. Contador.						       *
* 	l: Entero. Contador.						       *
* 	nodos: Entero. Almacena el numero de nodos que tiene la red.	       *
* 	max: Real. Generalizacion maxima.				       *
* 	idmax: Numero de red con generalizacion maxima.			       *
* 	conexiones: Numero de conexiones de la red.			       *
* 	aptitud: Real. Guarda la aptitud para medir la generalizacion	       *
* 	out: Puntero a fichero. Fichero de salida.			       *
*									       *
* Parametros de Salida:	NINGUNO						       *
*									       *
* Funciones a las que llama la funcion:					       *
*	generarSalidaRed()->Genera la salida de la red a partir de la salida de*
*			    los nodulos.				       *
*	generarSalidaNodulo()->Genera la salida de un nodulo a partir de un    *
*			       par‡metro de entrada.			       *
*	medirAptitudRed()->Mide la aptitud de la red a partir de las salidas   *
*			   generadas.					       *
*	error()->funcion que muestra un mensaje de error por pantalla.	       *
*									       *
*******************************************************************************/

void exportarMejorRed(char *ficheroSalida)
{
  int i,j,k,l,nodos,idmax,conexiones;
  double *aptitud,max;
  FILE *out;

  if((aptitud=(double *)malloc(5*sizeof(double)))==NULL)
    error(RES_MEM);

  for(i=0;i<pnodulos.n_nodulos;i++)
  {
    for(j=0;j<n_entrenamiento;j++)
      free(pnodulos.nodulos[i]->salidas_parciales[j]);
    free(pnodulos.nodulos[i]->salidas_parciales);
    if((pnodulos.nodulos[i]->salidas_parciales=(double **)malloc(n_generalizacion*sizeof(double)))==NULL)
      error(RES_MEM);
    for(j=0;j<n_generalizacion;j++){
      if((pnodulos.nodulos[i]->salidas_parciales[j]=(double *)malloc(predes.n_nodos_salida*sizeof(double)))==NULL)
        error(RES_MEM);
      for(k=0;k<predes.n_nodos_salida;k++)
	pnodulos.nodulos[i]->salidas_parciales[j][k]=0.0;
    }
  }

  /*Se abre el fichero de salida*/
  if((out=fopen(ficheroSalida,"w"))==NULL)
    error(IO);

  /*Se guardan las aptitudes de las redes seleccionadas*/
  for(i=0;i<5;i++)
  {
    aptitud[i]=predes.redes[i]->aptitud;
    predes.redes[i]->aptitud=0.0;
  }

  /*Se calcula la generalización de las redes*/
  for(i=0;i<n_generalizacion;i++)
  {
    /*Se generan las salidas parciales de los nódulos a partir de los datos de generalización*/
    for(j=0;j<pnodulos.n_nodulos;j++)
      generarSalidaNodulo(entrada[i],j,i,NULL);
    for(j=0;j<5;j++)
    {
      /*Se generan las salidas de las redes a partir de las salidas parciales de los nódulos*/
      generarSalidaRed(j,i);
      /*Se mide la aptitud de las redes según las salidas generadas*/
      medirAptitudRed(salida[i],j);
    }
  }
  /*Se normaliza la aptitud de generalización en función de los datos de entrada del fichero de generalización*/
  for(i=0;i<5;i++)
    predes.redes[i]->aptitud/=n_generalizacion;

  /*Se selecciona la red con mejor generalización de las 5 de mejor aptitud. Otros parámetros de selección es el número de nodos de la red,
  la aptitud de las redes y el número de conexiones de las redes*/
  max=0.0;
  idmax=0;
  for(i=0;i<5;i++)
  {
    if(predes.redes[i]->aptitud>max)
    {
      max=predes.redes[i]->aptitud;
      idmax=i;
    }
    else if(predes.redes[i]->aptitud==max)
    {
      if(aptitud[idmax]<aptitud[i])
      {
        max=predes.redes[i]->aptitud;
        idmax=i;
      }
      else if(aptitud[idmax]==aptitud[i])
      {
        nodos=0;
        for(j=0;j<pnodulos.n_subpobl;j++)
          nodos+=(predes.redes[idmax]->nodulos[j]->n_nodos-predes.redes[i]->nodulos[j]->n_nodos);
        if(nodos>0)
        {
          max=predes.redes[i]->aptitud;
          idmax=i;
        }
	else if(nodos==0)
	{
          conexiones=0;
          for(l=0;l<pnodulos.n_subpobl;l++)
          {
            for(j=0;j<predes.n_nodos_entrada;j++)
              for(k=0;k<predes.redes[idmax]->nodulos[l]->n_nodos;k++)
                if(predes.redes[idmax]->nodulos[l]->conexiones_entrada[j][k]==1)
	          conexiones++;
            for(j=0;j<predes.redes[idmax]->nodulos[l]->n_nodos;j++)
              for(k=0;k<predes.n_nodos_salida;k++)
                if(predes.redes[idmax]->nodulos[l]->conexiones_salida[j][k]==1)
	          conexiones++;
            for(j=0;j<predes.n_nodos_entrada;j++)
              for(k=0;k<predes.redes[i]->nodulos[l]->n_nodos;k++)
                if(predes.redes[i]->nodulos[l]->conexiones_entrada[j][k]==1)
	          conexiones--;
            for(j=0;j<predes.redes[i]->nodulos[l]->n_nodos;j++)
              for(k=0;k<predes.n_nodos_salida;k++)
                if(predes.redes[i]->nodulos[l]->conexiones_salida[j][k]==1)
	          conexiones--;
          }
	  if(conexiones>0)
          {
            max=predes.redes[i]->aptitud;
            idmax=i;
          }
	}
      }
    }
  }
  /*Escribe en un fichero de salida la aptitud, la generalización y el número de nodos de entrada, de salida y ocultos de la mejor red, así
  como su número de conexiones de entrada y de salida*/
  if(fprintf(out,"Aptitud: %lf\n",aptitud[idmax])==EOF)
    error(IO);
  if(fprintf(out,"Generalizacion: %lf\n",predes.redes[idmax]->aptitud)==EOF)
    error(IO);
  if(fprintf(out,"Número de Nódulos: %d\n",pnodulos.n_subpobl)==EOF)
    error(IO);
  if(fprintf(out,"Número de Nodos de Entrada: %d\n",predes.n_nodos_entrada)==EOF)
    error(IO);
  nodos=0;
  for(i=0;i<pnodulos.n_subpobl;i++)
    nodos+=predes.redes[idmax]->nodulos[i]->n_nodos;
  if(fprintf(out,"Número de Nodos Ocultos: %d\n",nodos)==EOF)
    error(IO);
  if(fprintf(out,"Número de Nodos de Salida: %d\n",predes.n_nodos_salida)==EOF)
    error(IO);
  conexiones=0;
  for(i=0;i<pnodulos.n_subpobl;i++)
  {
    for(j=0;j<predes.n_nodos_entrada;j++)
      for(k=0;k<predes.redes[idmax]->nodulos[i]->n_nodos;k++)
        if(predes.redes[idmax]->nodulos[i]->conexiones_entrada[j][k]==1)
	  conexiones++;
  }
  if(fprintf(out,"Número de Conexiones de Entrada: %d\n",conexiones)==EOF)
    error(IO);
  conexiones=0;
  for(i=0;i<pnodulos.n_subpobl;i++)
  {
    for(j=0;j<predes.redes[idmax]->nodulos[i]->n_nodos;j++)
      for(k=0;k<predes.n_nodos_salida;k++)
        if(predes.redes[idmax]->nodulos[i]->conexiones_salida[j][k]==1)
	  conexiones++;
  }
  if(fprintf(out,"Número de Conexiones de Salida: %d\n",conexiones)==EOF)
    error(IO);
  
  /*Se cierra el fichero de salida*/
  if(fclose(out)==EOF)
    error(IO);
  free(aptitud);
}
