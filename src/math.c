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
* Funci�n: Logistic()							       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Calcula la funci�n log�stica de un par�metro de     *
*			   entrada.					       *
*									       *
* Par�metros de Entrada:						       *
* 	x: Real. Par�metro de entrada al que se le va a calcular la funci�n    *
*	   log�stica							       *
* 									       *
* Par�metros Internos: NINGUNO						       *
* 									       *
* Par�metros de Salida:	La funci�n devuelve y = a/(1 + e (-bx)).	       *
* 									       *
* Funciones a las que llama la funci�n:	NINGUNA				       *
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
* Funci�n: HyperbolicTangent()						       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Calcula la tangente hiperb�lica de un par�metro de  *
*			   entrada.					       *
*									       *
* Par�metros de Entrada:						       *
* 	x: Real. Par�metro de entrada al que se le va a calcular la funci�n    *
*	   log�stica							       *
* 									       *
* Par�metros Internos: NINGUNO						       *
* 									       *
* Par�metros de Salida:	La funci�n devuelve y = a tanh (bx).		       *
* 									       *
* Funciones a las que llama la funci�n:	NINGUNA				       *
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
* Funci�n: Normal()							       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Calcula un valor seg�n una distribuci�n normal.     *
*									       *
* Par�metros de Entrada:						       *
* 	media: Real. Valor donde est� centrada la distribuci�n normal.	       *
* 	sigma: Real. Amplitud de la distribuci�n normal.		       *
* 									       *
* Par�metros Internos:							       *
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
* Par�metros de Salida:	La funci�n devuelve el valor aleatorio generado.       *
* 									       *
* Funciones a las que llama la funci�n:	NINGUNA				       *
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
* Funci�n: escalarDatosEntrada()					       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Escala los datos de los nodos de entrada.	       *
*									       *
* Par�metros de Entrada:						       *
* 	iteraciones: Entero. N�mero de datos de entrada.		       *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	max: Real. Valor m�ximo de los datos de entrada.		       *
* 	min: Real. Valor M�nimo de los valores de entrada.		       *
* 	a: Real. Primer par�metro del valor escalar.			       *
* 	b: Real. Segundo par�metro del valor escalar.			       *
* 									       *
* Par�metros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:	NINGUNA				       *
*									       *
*******************************************************************************/

void escalarDatosEntrada(int iteraciones)
{
  int i,j;
  double max,min,a,b;

  /* Se escalan los datos de entrada. */
  for(i=0;i<predes.n_nodos_entrada;i++)
  {
    /* Se obtienen los valores m�nimo y m�ximo de la columna. */
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
* Funci�n: escalarDatosSalida()						       *
*									       *
* Autor: Pablo �lvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funci�n: Escala los datos de los nodos de salida.	       *
*									       *
* Par�metros de Entrada:						       *
* 	datos: Matriz de reales. Datos de entrada de los nodos de salida.      *
* 	iteraciones: Entero. N�mero de datos de entrada.		       *
* 	lbound: Real. Valor m�nimo de salida de la funci�n de transferencia.   *
* 	ubound:	Real. Valor m�ximo de salida de la funci�n de transferencia.   *
* 									       *
* Par�metros Internos:							       *
* 	i: Entero. Contador.						       *
* 	j: Entero. Contador.						       *
* 	max: Real. Valor m�ximo de los datos de entrada.		       *
* 	min: Real. Valor M�nimo de los valores de entrada.		       *
* 	a: Real. Primer par�metro del valor escalar.			       *
* 	b: Real. Segundo par�metro del valor escalar.			       *
* 									       *
* Par�metros de Salida:	NINGUNO						       *
* 									       *
* Funciones a las que llama la funci�n:	NINGUNA				       *
*									       *
*******************************************************************************/

void escalarDatosSalida(double **datos,int iteraciones,double lbound,double ubound)
{
  int i,j;
  double max,min,a,b;

  /* Se escala la salida. */
  for (i=0;i<predes.n_nodos_salida;i++)
  {
    /* Se calculan los valores m�nimo y m�ximo de la columna. */
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
