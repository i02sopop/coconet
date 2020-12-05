SUBDIRS=src tests
ACLOCAL_AMFLAGS=-I m4
EXTRA_DIST=.build-aux/config.rpath
EXTRA_CLEAN=.build-aux ABOUT-NLS Makefile Makefile.in aclocal.m4 autom4te.cache \
			config.h* config.log config.status configure m4 stamp-h1 \
			tests/Makefile tests/Makefile.in src/Makefile src/Makefile.in \
			src/.deps libtool

.PHONY: check-syntax
check-syntax:
	gcc -o nul ${AM_CFLAGS} -S ${CHK_SOURCES}

.PHONY: clean-all
clean-all: clean
	@rm -f *~
	@for i in ${SUBDIRS}; do \
		rm -f $$i/*~ ; \
	done
	@for i in ${EXTRA_CLEAN}; do \
		rm -fr $$i; \
	done
