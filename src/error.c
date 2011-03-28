/********************************************************************************
* Copyright 2005 Pablo Alvarez de Sotomayor Posadillo			        *
*									        *
* This file is part of Coconet.						        *
*									        *
* Coconet is free software; you can redistribute it and/or modify	        *
* it under the terms of the GNU General Public License as published by	        *
* the Free Software Foundation; either version 2 of the License, or	        *
* (at your option) any later version.					        *
*									        *
* Coconet is distributed in the hope that it will be useful,		        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of		*
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		        *
* GNU General Public License for more details.				        *
*									        *
* You should have received a copy of the GNU General Public License	        *
* along with Coconet; if not, write to the Free Software		        *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA     *
********************************************************************************/

#include <string.h>
#include <errno.h>
#include <definiciones.h>

/*******************************************************************************
* Fichero: error.c							       *
*									       *
* Funcion: error()							       *
*									       *
* Autor: Pablo Alvarez de Sotomayor Posadillo				       *
*									       *
* Finalidad de la funcion: Muestra un mensaje de error en funcion de un numero *
*			   de error.					       *
*									       *
* Parametros de Entrada:						       *
* 	num_error: Entero. Numero del error producido.			       *
*									       *
* Parametros Internos: NINGUNO						       *
*									       *
* Parametros de Salida:	NINGUNO						       *
*									       *
* Funciones a las que llama la funcion:	NINGUNA				       *
*									       *
*******************************************************************************/

void error(int num_error)
{
  system("clear");
  /* Según el error que se de muestra un mensaje u otro. */
  switch(num_error){
	case RES_MEM:
		fprintf(stderr,"Error en la reserva de memoria. %s\n",strerror(errno));
		break;
	case IO:
		fprintf(stderr,"Error de entrada/salida. %s\n",strerror(errno));
		break;
	case PARAMETROS:
		fprintf(stderr,"Error al introducir los parámetros en una función. "
			       "O formato de los parámetros incorrecto o está fuera "
			       "del rango\n");
		break;
  	case COMANDO:
		fprintf(stderr,"Error al introducir el comando de entrada. El formato "
			       "es:\n\tcoconet fichero_conf fichero_entrenamiento "
			       "fichero_generalizacion [-o fichero_salida]\n donde "
			       "fichero_conf es el fichero de configuración del "
			       "problema, fichero_entrenamiento es el fichero que "
			       "contiene los datos de entrenamiento de las redes y "
			       "fichero_generalización es el fichero que contiene los "
			       "datos para medir la generalización de las redes. "
			       "Fichero_salida es el nombre del fichero de salida, y "
			       "es opcional\n");
		break;
  	default:
		fprintf(stderr,"Error desconocido. %s\n", strerror(errno));
		break;
  }
  exit(1);
}
