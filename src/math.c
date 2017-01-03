/*********************************************************************************
 * Copyright (c) 2004-2017 coconet project (see AUTHORS)                         *
 *                                                                               *
 * This file is part of Coconet.                                                 *
 *                                                                               *
 * Coconet is free software: you can redistribute it and/or modify it under the  *
 * terms of the GNU General Public License as published by the Free Software     *
 * Foundation, either version 3 of the License, or (at your option) any later    *
 * version.                                                                      *
 *                                                                               *
 * Coconet is distributed in the hope that it will be useful, but WITHOUT ANY    *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR *
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.   *
 *                                                                               *
 * You should have received a copy of the GNU General Public License along with  *
 * coconet. If not, see <http://www.gnu.org/licenses/>.                          *
 ********************************************************************************/

#include <definitions.h>
#include <math.h>

/*********************************************************************************
 * Calculate the logistic function of an input data.                             *
 * @param double x: Input data to calculate the logistic function.               *
 * @return Double. a / (1 + e(-b * x)).                                          *
 ********************************************************************************/
double
Logistic(double x) {
	/* y = a / (1 + e(-b * x)). */
	return (double)(pTransfer.logistic_a / (1 + exp((double)(-pTransfer.logistic_b * x))));
}

/*********************************************************************************
 * Calculate the Hyperbolic Tangent of a given data.                             *
 * @param double x: Input data to make the calculations.                         *
 * @return Double. a * tanh(b * x).                                              *
 ********************************************************************************/
double
HyperbolicTangent(double x) {
	/* y = a * tanh(b * x). */
	return (double)(pTransfer.htan_a * tanh(pTransfer.htan_b * x));
}

/*********************************************************************************
 * Calculate a random value following the normal distribution.                   *
 * @param double media: Value where the normal distribution is centered.         *
 * @param double sigma: Width of the normal distribution.                        *
 * @return Doubld. Random value generated.                                       *
 ********************************************************************************/
double
Normal(double media, double sigma) {
	static double glgset;
	static int gliset = 0;
	double fac; /* Adjust factor for the random-generated value. */
	double r; /* Random value between 0 and 1 to normalize the generated value. */
	double v1, v2; /* Randon values to generate the output value. */
	double gasdev; /* Random value generated. */

	if (!gliset) {
		do {
			v1 = 2 * doubleRandom() - 1;
			v2 = 2 * doubleRandom() - 1;
			r = pow(v1, 2) + pow(v2, 2);
		} while (r >= 1);

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

/*********************************************************************************
 * Scale the data of the input nodes.                                            *
 * @param int iterations: Number of input data.                                  *
 * @return void                                                                  *
 ********************************************************************************/
void
scaleInputData(int iterations) {
	double max, min, a, b;

	/* Scale the input data. */
	for (int i = 0; i < cNetPopulation.numInputNodes; i++) {
		/* Obtain the max and min values of the column. */
		min = max = inputData[0][i];
		for (int j = 0; j < iterations; j++) {
			if (max < inputData[j][i])
				max = inputData[j][i];
			else if (min > inputData[j][i])
				min = inputData[j][i];
		}

		/* Calculate a & b coeficients. If the coeficients are constants we
		 * don't do anything. */
		if (max != min) {
			a = (MAX_ESC - MIN_ESC) / (max - min);
			b = (MIN_ESC * max - MAX_ESC * min) / (max - min);
      
			/* Scale the column. */
			for (int j = 0; j < iterations; j++)
				inputData[j][i] = a * inputData[j][i] + b;
		}
	}
}

/*********************************************************************************
 * Scale the data of the output nodes.                                           *
 * @param double **data: Input data for the output nodes.                        *
 * @param int iterations: Number of input data.                                  *
 * @param double lbound: Min output value of the transfer function.              *
 * @param double ubound: Max output value of the transfer function.              *
 * @return void                                                                  *
 ********************************************************************************/
void
scaleOutputData(double **data, int iterations, double lbound, double ubound) {
	double max, min, a, b;

	/* Scale the output. */
	for (int i = 0; i < cNetPopulation.numOutputNodes; i++) {
		/* Calculate the min and max values of the column. */
		min = max = data[0][i];
		for (int j = 0; j < iterations; j++) {
			if (max < data[j][i])
				max = data[j][i];
			else if (min > data[j][i])
				min = data[j][i];
		}

		/* Calculate the coeficients a & b. If they are constants we don't do
		 * anything. */
		if (max != min) {
			a = (ubound - lbound) / (max - min);
			b = (lbound * max-ubound * min) / (max - min);

			/* Scale the column. */
			for (int j = 0; j < iterations; j++)
				data[j][i] = a * data[j][i] + b;
		}
	}
}
