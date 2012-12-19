SUBDIRS=src
ACLOCAL_AMFLAGS=-I m4
EXTRA_DIST = .build-aux/config.rpath

.PHONY: update-pot
update-pot:
	@xgettext -L C -j src/*.c include/*.h -o po/coconet.pot

.PHONY: check-syntax
check-syntax:
	gcc -o nul ${AM_CFLAGS} -S ${CHK_SOURCES}

.PHONY: clean-all
clean-all: clean
	@rm -fr .build-aux/ ABOUT-NLS Makefile Makefile.in aclocal.m4 autom4te.cache/ config.h config.h.in* config.log config.status configure m4 stamp-h1 intl po/Makefile.in po/Makefile.in.in
