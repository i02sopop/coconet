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
#include <math.h>

/*******************************************************************************
* Fichero: math.c							       *
*									       *
* Función: Logistic()							       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Calcula la función logística de un parámetro de     *
*			   entrada.					       *
*									       *
* Parámetros de Entrada:						       *
* 	x: Real. Parámetro de entrada al que se le va a calcular la función    *
*	   logística							       *
* 									       *
* Parámetros Internos: NINGUNO						       *
* 									       *
* Parámetros de Salida:	La función devuelve y = a/(1 + e (-bx)).	       *
* 									       *
* Funciones a las que llama la función:	NINGUNA				       *
*									       *
*******************************************************************************/

double Logistic (double x)
{
  /* y = a/(1 + e (-bx)). */
  return (double)(ptransferencia.logistic_a/(1+exp((double)(-ptransferencia.logistic_b*x))));
}

/*******************************************************************************
* Fichero: math.c							       *
*									       *
* Función: HyperbolicTangent()						       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Calcula la tangente hiperbólica de un parámetro de  *
*			   entrada.					       *
*									       *
* Parámetros de Entrada:						       *
* 	x: Real. Parámetro de entrada al que se le va a calcular la función    *
*	   logística							       *
* 									       *
* Parámetros Internos: NINGUNO						       *
* 									       *
* Parámetros de Salida:	La función devuelve y = a tanh (bx).		       *
* 									       *
* Funciones a las que llama la función:	NINGUNA				       *
*									       *
*******************************************************************************/

double HyperbolicTangent (double x)
{
  /* y = a tanh (bx). */
  return (double)(ptransferencia.htan_a*tanh(ptransferencia.htan_b*x));
}

/*******************************************************************************
* Fichero: math.c							       *
*									       *
* Función: Normal()							       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Calcula un valor según una distribución normal.     *
*									       *
* Parámetros de Entrada:						       *
* 	media: Real. Valor donde está centrada la distribución normal.	       *
* 	sigma: Real. Amplitud de la distribución normal.		       *
* 									       *
* Parámetros Internos:							       *
* 	glgset: Real.							       *
* 	gliset: Entero.	Variable que funciona como booleana.		       *
* 	fac: Real. Factor de ajuste del valor aleatorio generado.	       *
* 	r: Real. Variable aleatoria entre 0 y 1 basado en v1 y v2 que sirve    *
*	   para normalizar el valor generado.				       *
* 	v1: Real. Valor generado aleatoriamente y utilizado para generar el    *
*	    valor de salida.						       *
* 	v2: Real. Valor generado aleatoriamente y utilizado para generar el    *
*	    valor de salida.						       *
* 	gasdev: Real. Valor generado aleatoriamente.			       *
* 									       *
* Parámetros de Salida:	La función devuelve el valor aleatorio generado.       *
* 									       *
* Funciones a las que llama la función:	NINGUNA				       *
*									       *
*******************************************************************************/

double Normal(double media,double sigma)
{
  static double glgset;
  static int gliset=0;
  double fac,r,v1,v2,gasdev;

  if(gliset==0)
  {
    do{
      v1=2*aleatorio()-1;
      v2=2*aleatorio()-1;
      r=v1*v1+v2*v2;
    }while(r>=1);
    fac=sqrt(-2*log(r)/r);
    glgset=v1*fac;
    gasdev=v2*fac;
    gliset=1;
  }
  else
  {
    gasdev=glgset;
    gliset=0;
  }
  return (double)((gasdev*sigma)+media);
}

/*******************************************************************************
* Fichero: math.c							       *
*									       *
* Función: escalarDatosEntrada()					       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Escala los datos de los nodos de entrada.	       *
*									       *
* Parámetros de Entrada:						       *
* 	iteraciones: Entero. Número de datos de entrada.		       *
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	max: Real. Valor máximo de los datos de entrada.		       *
* 	min: Real. Valor Mínimo de los valores de entrada.		       *
* 	a: Real. Primer parámetro del valor escalar.			       *
* 	b: Real. Segundo parámetro del valor escalar.			       *
* 									       *
* Parámetros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la función:	NINGUNA				       *
*									       *
*******************************************************************************/

void escalarDatosEntrada(int iteraciones)
{
  int i,j;
  double max,min,a,b;

  /* Se escalan los datos de entrada. */
  for(i=0;i<predes.n_nodos_entrada;i++)
  {
    /* Se obtienen los valores mínimo y máximo de la columna. */
    min=max=entrada[0][i];
    for(j=0;j<iteraciones;j++)
    {
      if(max<entrada[j][i])
	max=entrada[j][i];
      else if(min>entrada[j][i])
        min=entrada[j][i];
    }

    /* Se calculan los coeficientes a y b. */
    /* Si son constantes no se hace nada. */
    if (max!=min)
    {
      a=(MAX_ESC-MIN_ESC)/(max-min);
      b=(MIN_ESC*max-MAX_ESC*min)/(max-min);
      
      /* Se escala la columna. */
      for(j=0;j<iteraciones;j++)
        entrada[j][i]=a*entrada[j][i]+b;
    }
  }
}


/*******************************************************************************
* Fichero: math.c							       *
*									       *
* Función: escalarDatosSalida()						       *
*									       *
* Autor: Pablo Álvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la función: Escala los datos de los nodos de salida.	       *
*									       *
* Parámetros de Entrada:						       *
* 	datos: Matriz de reales. Datos de entrada de los nodos de salida.      *
* 	iteraciones: Entero. Número de datos de entrada.		       *
* 	lbound: Real. Valor mínimo de salida de la función de transferencia.   *
* 	ubound:	Real. Valor máximo de salida de la función de transferencia.   *
* 									       *
* Parámetros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	max: Real. Valor máximo de los datos de entrada.		       *
* 	min: Real. Valor Mínimo de los valores de entrada.		       *
* 	a: Real. Primer parámetro del valor escalar.			       *
* 	b: Real. Segundo parámetro del valor escalar.			       *
* 									       *
* Parámetros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la función:	NINGUNA				       *
*									       *
*******************************************************************************/

void escalarDatosSalida(double **datos,int iteraciones,double lbound,double ubound)
{
  int i,j;
  double max,min,a,b;

  /* Se escala la salida. */
  for (i=0;i<predes.n_nodos_salida;i++)
  {
    /* Se calculan los valores mínimo y máximo de la columna. */
    min=max=datos[0][i];
    for(j=0;j<iteraciones;j++)
    {
      if(max<datos[j][i])
        max=datos[j][i];
      else if(min>datos[j][i])
        min=datos[j][i];
    }
    /* Se calculan los coeficientes a y b . */
    /* Si son constantes no se hace nada. */
    if(max!=min)
    {
      a=(ubound-lbound)/(max-min);
      b=(lbound*max-ubound *min)/(max-min);

      /* Se escala la columna. */
      for(j=0;j<iteraciones;j++)
	datos[j][i]=a*datos[j][i]+b;
    }
  }
}
