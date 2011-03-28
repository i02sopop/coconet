/********************************************************************************
* Copyright 2004 Pablo Álvarez de Sotomayor Posadillo				*
*										*
* This file is part of Coconet.							*
*										*
* Coconet is free software; you can redistribute it and/or modify		*
* it under the terms of the GNU General Public License as published by		*
* the Free Software Foundation; either version 2 of the License, or		*
* (at your option) any later version.						*
*										*
* Coconet is distributed in the hope that it will be useful,			*
* but WITHOUT ANY WARRANTY; without even the implied warranty of		*
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			*
* GNU General Public License for more details.					*
*										*
* You should have received a copy of the GNU General Public License		*
* along with Coconet; if not, write to the Free Software			*
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA	*
********************************************************************************/

#ifndef MACROS_H
#define MACROS_H

#define subabs(x,y) (x-y)>0?x-y:y-x
#define aleatorio() (double)((double)random()/(double)RAND_MAX)
#define saleatorio() (random()%2)==1?aleatorio():-aleatorio()

/*macros de errores*/
#define	RES_MEM		0
#define	IO		1
#define	PARAMETROS	2
#define COMANDO		3

#define MAX_ESC 1
#define MIN_ESC -1

#define true 1
#define false 0

typedef int bool;
typedef double (*func)(double);

#endif /*MACROS_H*/
