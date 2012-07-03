AM_CFLAGS=-O3
coconet_CFLAGS=-I$(INCLUDE_DIR) -DLOCALEDIR=\"$(localedir)\"
coconet_LDADD=$(LIBMATH) $(LIBINTL)
coconet_LDFLAG=

bin_PROGRAMS=coconet
coconet_SOURCES=createPopulations.c evolvePopulations.c freeMemory.c main.c \
	measureAptitude.c mutation.c sort.c error.c files.c generateOutput.c \
	math.c measureChange.c print.ci definitions.h functions.h macros.h \
	gettext.h

.PHONY: check-syntax
check-syntax:
	gcc -o nul -S ${CHK_SOURCES}
