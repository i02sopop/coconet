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

#include <string.h>
#include <definiciones.h>

/*******************************************************************************
* Fichero: main.c							       *
*									       *
* Funcion: main()							       *
*									       *
* Autor: Pablo Alvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funcion: Lleva el flujo del programa.			       *
*									       *
* Parametros de Entrada:						       *
* 	argc: Entero. Numero de argumentos de entrada al programa.	       *
* 	argv: Vector de cadenas de caracteres. Argumentos hechos al programa.  *
*									       *
* Parametros Internos:							       *
*	i: Entero. Contador.						       *
*	j: Entero. Contador.						       *
*	aptitudRedes: Real. Almacena la aptitud media de las redes.	       *
*									       *
* Parametros de Salida:	La funcion devuelve 0 si todo va bien y otro valor en  *
*			caso contrario.					       *
*									       *
* Funciones a las que llama la funcion:					       *
*	error()->Funcion que muestra un mensaje de error en funcion del numero *
* 		 de error producido.					       *
*	cargarFichero()->Funcion que carga los datos de entrada del fichero de *
*			 entrenamiento y del fichero de configuración          *
*	escalarDatosEntrada()->Escala los datos de los nodos de entrada.       *
*	escalarDatosSalida()->Escala los datos de los nodos de salida.	       *
*	medirCambio()->Mide el cambio en la aptitud media de la poblacion de   *
*		       redes.						       *
*	crearPoblacionNodulos()->Crea una nueva subpoblacion de nodulos.       *
*	anadirNoduloRedes()->Añade una nueva subpoblacion de nodulos a la      *
*			     poblacion de redes.			       *
*	evolucionarPoblaciones()->Evoluciona las poblaciones de nodulos y de   *
*				  redes.				       *
*	ordenarRedes()->Ordena las redes en funcion de su aptitud.	       *
*	leerGeneralizacion()->Lee los datos para medir la generalizacion de las*
*			      redes.					       *
*	exportarMejorRed()->Se mide la generalización de la red que mejor se   *
*			    adapta al problema y se muestran sus datos en un   *
*			    fichero.					       *
*	ajustarMatrices()->Se ajusta el tamaño de las matrices que se utilizan *
*			   en la nueva subpoblacion de nodulos		       *
*									       *
*******************************************************************************/

int main(int argc,char **argv)
{
  int i;
  double aptitudRedes;

  /* Se comprueba si los argumentos de entrada son correctos. */
  if(argc<4)
    error(COMANDO);
  else if(argc>4 && (argc!=6 || strcmp(argv[4],"-o")!=0))
    error(COMANDO);

  /* Se cargan los datos de entrada del fichero de entrenamiento y del fichero de configuración. */
  cargarFichero(argv[1],argv[2]);
  if(nodsel>num_nodulos)
    nodsel=num_nodulos;
  
  /*Se inicializa la aptitud media de las redes*/
  aptitudRedes=0.0;
  
  /*Se escalan los datos de entrada y de salida del fichero de entrenamiento*/
  fprintf(stderr,"escalar datos entrada\n");
  escalarDatosEntrada(n_entrenamiento);
  fprintf(stderr,"escalar datos salida\n");
  if(net_transf==(func)&Logistic)
    escalarDatosSalida(salida,n_entrenamiento,0.0+ptransferencia.epsilon,
		       ptransferencia.logistic_a-ptransferencia.epsilon);
  else
    escalarDatosSalida(salida,n_entrenamiento,-ptransferencia.htan_a+ptransferencia.epsilon,
		       ptransferencia.htan_a-ptransferencia.epsilon);

  /*Se desarrollan las poblaciones de redes y de nódulos*/
  for(i=0;medirCambio(&aptitudRedes,i)==false;i++) {
    /*Se crea una nueva población de nodulos*/
    fprintf(stderr,"Especie %d\n",i+1);
    crearPoblacionNodulos();

    /*Se añaden los nódulos de la nueva población a la población existente de redes*/
    fprintf(stderr,"Añadir nodulos a redes\n");
    anadirNodulosRedes();

    /*Se evolucionan las poblaciones de redes y de nódulos*/
    fprintf(stderr,"Evolucionar poblaciones\n");
    evolucionarPoblaciones();

    /*Se ajusta el tamaño de las matrices de la última subpoblación de nódulos*/
    fprintf(stderr,"Ajustar matrices\n");
    ajustarMatrices();
  }

  /*Se ordenan las redes según su aptitud*/
  fprintf(stderr,"ordenar redes\n");
  ordenarRedes();

  /*Se leen los datos de entrada del fichero de generalización*/
  fprintf(stderr,"leerGeneralizacion\n");
  leerGeneralizacion(argv[3]);

  /*Se escalan los datos de entrada y de salida del fichero de generalización*/
  fprintf(stderr,"escalar datos entrada\n");
  escalarDatosEntrada(n_generalizacion);
  fprintf(stderr,"escalar datos salida\n");
  if(net_transf==(func)&Logistic)
    escalarDatosSalida(salida,n_generalizacion,0.0+ptransferencia.epsilon,ptransferencia.logistic_a-ptransferencia.epsilon);
  else
    escalarDatosSalida(salida,n_generalizacion,-ptransferencia.htan_a+ptransferencia.epsilon,ptransferencia.htan_a-ptransferencia.epsilon);
  
  /*Se exporta la mejor red encontrada*/
  fprintf(stderr,"exportarMejoRed\n");
  if(argc==6)
    exportarMejorRed(argv[5]);/*Se exporta la red con mejor aptitud*/
  else
    exportarMejorRed("salida.txt");/*Se exporta la red con mejor aptitud*/
  fprintf(stderr,"Fin\n");
  
  return 0;
}
