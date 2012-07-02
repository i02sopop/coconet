PROJ_DIR=..
INCLUDE_DIR=$(PROJ_DIR)/include
#LDADD=
AM_CFLAGS=-O3
coconet_CFLAGS=-I$(INCLUDE_DIR)
coconet_LDADD=$(LIBMATH)
coconet_LDFLAG=

bin_PROGRAMS=coconet
coconet_SOURCES=createPopulations.c evolvePopulations.c freeMemory.c main.c \
	measureAptitude.c mutation.c sort.c error.c files.c generateOutput.c \
	math.c measureChange.c print.c definitions.h functions.h macros.h
