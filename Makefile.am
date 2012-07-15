SUBDIRS=po src

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = .build-aux/config.rpath

.PHONY: update-po
update-po:
	$(MAKE) -C $(PO_DIR) update-po

.PHONY: check-syntax
check-syntax:
	gcc -o nul ${AM_CFLAGS} -S ${CHK_SOURCES}
