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

/******************************************************************************
 File: imprimir.c
 Function: imprimirNodulo()
 Author: Pablo Alvarez de Sotomayor Posadillo
 Description: Print the information related with a given nodule to an output
              file.
 Input Parameters:
 	imp: Nodule. Nodule to print.
 	iter: Integer. Number of input patterns.
 Local Variables:
 	i: Integer. Counter.
 	j: Integer. Counter.
 	salida: Pointer to file. Output file.
	numEntrada: Integer. Counter of the input patterns to generate its output.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void imprimirNodulo(nodulo *imp,int iter)
{
	int i, j, numEntrada;
	FILE *salida;

	/* We open the output file and print the nodule information. */
	salida = fopen("nodulos.txt", "a");
	fprintf(salida, "id %d\nn_nodos %d\naptitud %lf\n\nMatriz de conexiones de entrada", imp->id, imp->n_nodos, imp->aptitud);
	for(i = 0; i < predes.n_nodos_entrada; i++) {
		fprintf(salida, "\n");
		for(j = 0; j < imp->n_nodos; j++)
			fprintf(salida, "%d ", imp->conexiones_entrada[i][j]);
	}

	fprintf(salida, "\nMatriz de conexiones de salida");
	for(i=0;i<imp->n_nodos;i++) {
		fprintf(salida, "\n");
		for(j = 0; j < predes.n_nodos_salida; j++)
			fprintf(salida, "%d ", imp->conexiones_salida[i][j]);
	}

	fprintf(salida, "\nMatriz de pesos de entrada");
	for(i = 0; i < predes.n_nodos_entrada; i++) {
		fprintf(salida, "\n");
		for(j = 0; j < imp->n_nodos; j++)
			fprintf(salida, "%lf ", imp->pesos_entrada[i][j]);
	}

	fprintf(salida, "\nMatriz de pesos de salida");
	for(i = 0; i < imp->n_nodos; i++) {
		fprintf(salida, "\n");
		for(j = 0; j < predes.n_nodos_salida; j++)
			fprintf(salida, "%lf ", imp->pesos_salida[i][j]);
	}

	fprintf(salida, "\nSalidas parciales\n");
	for(numEntrada = 0; numEntrada < iter; numEntrada++)
		for(i = 0; i < predes.n_nodos_salida; i++)
			fprintf(salida, "%lf ", imp->salidas_parciales[numEntrada][i]);
	fprintf(salida, "\n\n");

	/* We close the putput file. */
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
