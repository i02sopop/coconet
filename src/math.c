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

#include <definitions.h>
#include <math.h>

/*****************************************************************************
 File: math.c
 Function: Logistic()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Calculate the logistic function of an input data.
 Input Parameters:
   x: Float. Input data to calculate the logistic function.
 Local Variables: None
 Return Value:  a / (1 + e(-b * x)).
 Calling Functions: None
******************************************************************************/

double Logistic(double x)
{
	/* y = a / (1 + e(-b * x)). */
	return (double)(pTransfer.logistic_a / (1 + exp((double)(-pTransfer.logistic_b * x))));
}

/******************************************************************************
 File: math.c
 Function: HyperbolicTangent()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Calculate the Hyperbolic Tangent of a given data.
 Input Parameters:
   x: Float. Input data to make the calculations.
 Local Variables: None
 Return Value: a * tanh(b * x).
 Calling Functions: None
******************************************************************************/

double HyperbolicTangent(double x)
{
	/* y = a * tanh(b * x). */
	return (double)(pTransfer.htan_a * tanh(pTransfer.htan_b * x));
}

/******************************************************************************
 File: math.c
 Function: Normal()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Calculate a random value following the normal distribution.
 Input Parameters:
   media: Float. Value where the normal distribution is centered.
   sigma: Float. Width of the normal distribution.
 Local Variables:
   glgset: Float.
   gliset: Integer. Works as boolean.
   fac: Float. Adjust factor for the random-generated value.
   r: Float. Random value between 0 and 1 to normalize the generated value.
   v1: Float. Randon value to generate the output value.
   v2: Float. Randon value to generate the output value.
   gasdev: Float. Random value generated.
 Return Value: Random value generated.
 Calling Functions:
   doubleRandom(): Returns a random float of double precision.
*******************************************************************************/

double Normal(double media, double sigma)
{
	static double glgset;
	static int gliset = 0;
	double fac, r, v1, v2, gasdev;

	if(!gliset) {
		do {
			v1 = 2 * doubleRandom() -1;
			v2 = 2 * doubleRandom() - 1;
			r = v1 * v1 + v2 * v2;
		} while(r >= 1);

		fac = sqrt(-2 * log(r) / r);
		glgset = v1 * fac;
		gasdev = v2 * fac;
		gliset = 1;
	} else {
		gasdev = glgset;
		gliset = 0;
	}

	return (double)((gasdev * sigma) + media);
}

/*******************************************************************************
 File: math.c
 Function: escalarDatosEntrada()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Scale the data of the input nodes.
 Input Parameters:
   iterations: Integer. Number of input data.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   max: Float. Max value of the input data.
   min: Float. Min value of the input data.
   a: Float. First parameter of the scalar value.
   b: Float. Second parameter of the scalar value.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void escalarDatosEntrada(int iterations)
{
	int i, j;
	double max, min, a, b;

	/* We scale the input data. */
	for(i = 0; i < cNetPopulation.numInputNodes; i++) {
		/* We obtain the max and min values of the column. */
		min = max = inputData[0][i];
		for(j = 0; j < iterations; j++) {
			if(max < inputData[j][i])
				max = inputData[j][i];
			else if(min > inputData[j][i])
				min = inputData[j][i];
		}

		/* We calculate a & b coeficients. */
		/* If the coeficients are constants we don't do anything. */
		/* Si son constantes no se hace nada. */
		if(max != min) {
			a = (MAX_ESC - MIN_ESC) / (max - min);
			b = (MIN_ESC * max - MAX_ESC * min) / (max - min);
      
			/* We scale the column. */
			for(j = 0; j < iterations; j++)
				inputData[j][i] = a * inputData[j][i] + b;
		}
	}
}

/******************************************************************************
 File: math.c
 Function: escalarDatosSalida()
 Author: Pablo Álvarez de Sotomayor Posadillo
 Description: Scale the data of the output nodes.
 Input Parameters:
   data: Matrix of floats. Input data for the output nodes.
   iterations: Integer. Number of input data.
   lbound: Float. Min output value of the transfer function.
   ubound:	Float. Max output value of the transfer function.
 Local Variables:
   i: Integer. Counter.
   j: Integer. Counter.
   max: Float. Max value of the input data.
   min: Float. Min value of the input data.
   a: Float. First paramameter of the scalar value.
   b: Float. Second parameter of the scalar value.
 Return Value: None
 Calling Functions: None
******************************************************************************/

void escalarDatosSalida(double **data, int iterations, double lbound, double ubound)
{
	int i, j;
	double max, min, a, b;

	/* We scale the output. */
	for(i = 0; i < cNetPopulation.numOutputNodes; i++) {
		/* We calculate the min and max values of the column. */
		min = max = data[0][i];
		for(j = 0; j < iterations; j++) {
			if(max < data[j][i])
				max = data[j][i];
			else if(min > data[j][i])
				min = data[j][i];
		}

		/* We calculate the coeficients a & b. */
		/* If they are constants we don't do anything. */
		if(max != min) {
			a = (ubound - lbound) / (max - min);
			b = (lbound * max-ubound * min) / (max - min);

			/* We scale the column. */
			for(j = 0; j < iterations; j++)
				data[j][i] = a * data[j][i] + b;
		}
	}
}
