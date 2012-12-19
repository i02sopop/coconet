SUBDIRS=po src
ACLOCAL_AMFLAGS=-I m4
EXTRA_DIST = .build-aux/config.rpath

.PHONY: update-pot
update-pot:
	@xgettext -L C -j src/*.c include/*.h -o po/coconet.pot

.PHONY: check-syntax
check-syntax:
	gcc -o nul ${AM_CFLAGS} -S ${CHK_SOURCES}
