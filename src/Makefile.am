AM_CFLAGS=-O2
AUTOMAKE_OPTIONS=std-options subdir-objects
bin_PROGRAMS=coconet

coconetdir=$(PROJ_DIR)
coconet_SHORTNAME = cnet
coconet_CFLAGS=-I$(INCLUDE_DIR) -DLOCALEDIR=\"$(localedir)\" -g -O2 -Wall -std=gnu11
coconet_LDADD=$(LIBMATH) $(LIBINTL)
coconet_LDFLAG=
coconet_SOURCES=createPopulations.c evolvePopulations.c freeMemory.c main.c \
	measureAptitude.c mutation.c sort.c files.c generateOutput.c math.c \
	measureChange.c print.c utils.c
coconet_HEADERS=$(INCLUDE_DIR)/definitions.h $(INCLUDE_DIR)/functions.h \
	$(INCLUDE_DIR)/gettext.h $(INCLUDE_DIR)/macros.h

.PHONY: check-syntax
check-syntax:
	gcc -o nul ${AM_CFLAGS} ${coconet_CFLAGS} -S ${CHK_SOURCES}
