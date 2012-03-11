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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void cambioPesos(double *einicial,double **pesos,double *salida,double **F_W,int nodos,func *transf);
void retropropagacion(int numNodulo,int n_patrones,int iteraciones);
void sustituirNodulos();
double sustitucion(int numNodulo,red **copia);
void pasoAleatorio(int numNodulo);
void ordenarRedes();
void ordenarNodulos();
void normalizarAptitudNodulos();
void mutarNodulos(int numNodulo);
double mejores(int numNodulo);
void medirCambioNodulo(int numNodulo);
bool medirCambio(double *aptitudRedes, int iteracion);
void medirAptitudRed(double *salida, int numRed);
void medirAptitudNodulos(int numNodulo);
void copiarRed(red **origen, red **nueva);
double Logistic(double x);
double HyperbolicTangent(double x);
double Normal(double media,double sigma);
void liberarNodulo(nodulo *eliminar);
void guardarPesos(int numNodulo,float *pesos);
void leerGeneralizacion(char *generalizacion);
void imprimirRed(red *imp);
void imprimirNodulo(nodulo *imp,int numEntrada);
void generarSalidaRed(int numRed, int numEntrada);
void generarSalidaNodulo(double *in, int numNodulo, int numEntrada,double *nod_out);
void exportarMejorRed(char *ficheroSalida);
void evolucionarPoblaciones();
void error(int num_error);
void enfriamientoSimulado(int numNodulo);
double diferencia(int numNodulo,red **copia);
void crearRedes();
void crearPoblacionNodulos();
void crearNodulos();
void liberarRed(red *borrar);
void crearDescendencia();
void anadirNodo(int numNodulo,int numNodos);
void copiarDescendencia();
void cargarFichero(char *config, char *entrenamiento);
void borrarNodo(int numNodulo,int numNodos);
void borrarConexion(int numNodulo,int nodo);
void anadirNodulosRedes();
void anadirConexion(int numNodulo);
void escalarDatosEntrada(int iteraciones);
void escalarDatosSalida(double **datos,int iteraciones,double lbound,double ubound);
void ajustarMatrices();

#endif /* FUNCTIONS_H */
