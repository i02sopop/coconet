/******************************************************************************
 Copyright (c) 2004-2018 coconet project (see AUTHORS)

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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void changeWeight(double *, double **, double *, double **, int, func *);
void backpropagation(int, int, int);
void replaceNodules();
double replace(int, network **);
void randomStep(int);
void sortNetworks();
void sortNodules();
void normalizeNoduleAptitude();
void mutateNodules(int);
double best(int);
void measureNoduleChange(int);
bool measureChange(double *, int);
void measureNetworkAptitude(double *, int);
void measureNoduleAptitude(int);
void copyNetwork(network **, network **);
double Logistic(double);
double HyperbolicTangent(double);
double Normal(double, double);
void storeWeights(int, float *);
void readGeneralization(char *);
void printNetwork(network *);
void printNodule(nodule *, int);
void generateNetOutput(int, int);
void generateNoduleOutput(double *, int, int, double *);
void exportBestNetwork(char *);
void evolvePopulations();
void error(int);
void simulatedAnnealing(int);
double differ(int, network **);
void createNetworks();
void createNodules();
void createNodulePopulation();
void freeNetwork(network *);
void freeNodule(nodule *);
void createDescendant();
void copyDescendant();
void loadFile(char *, char *);
void addNode(int, int);
void addConnection(int);
void addNodulesNetworks();
void deleteNode(int, int);
void deleteConnection(int, int);
void scaleIputData(int);
void scaleOutputData(double **, int, double, double);
void adjustMatrix();
void scaleInputData(int);

/* util.c */
void *xmalloc(size_t);
void *xcalloc(size_t, size_t);
void *xrealloc(void *, size_t);

#endif /* FUNCTIONS_H */
