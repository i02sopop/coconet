SUBDIRS= po src

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = .build-aux/config.rpath

.PHONY: check-syntax
check-syntax:
	gcc -o nul -S ${CHK_SOURCES}
