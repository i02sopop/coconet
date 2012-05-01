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

void cambioPesos(double *, double **, double *, double **, int, func *);
void retropropagacion(int, int, int);
void sustituirNodulos();
double sustitucion(int, network **);
void pasoAleatorio(int);
void ordenarRedes();
void ordenarNodulos();
void normalizarAptitudNodulos();
void mutarNodulos(int);
double mejores(int);
void medirCambioNodulo(int);
bool medirCambio(double *, int);
void medirAptitudRed(double *, int);
void medirAptitudNodulos(int);
void copiarRed(network **, network **);
double Logistic(double);
double HyperbolicTangent(double);
double Normal(double, double);
void liberarNodulo(nodule *);
void guardarPesos(int, float *);
void leerGeneralizacion(char *);
void imprimirRed(network *);
void imprimirNodulo(nodule *, int);
void generarSalidaRed(int, int);
void generarSalidaNodulo(double *, int, int, double *);
void exportarMejorRed(char *);
void evolucionarPoblaciones();
void error(int);
void enfriamientoSimulado(int);
double diferencia(int, network **);
void crearRedes();
void crearPoblacionNodulos();
void crearNodulos();
void liberarRed(network *);
void crearDescendencia();
void anadirNodo(int, int);
void copiarDescendencia();
void cargarFichero(char *, char *);
void borrarNodo(int, int);
void borrarConexion(int, int);
void anadirNodulosRedes();
void anadirConexion(int);
void escalarDatosEntrada(int);
void escalarDatosSalida(double **, int, double, double);
void ajustarMatrices();

#endif /* FUNCTIONS_H */
