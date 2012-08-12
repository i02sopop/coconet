SUBDIRS=po src

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = .build-aux/config.rpath

.PHONY: update-po
update-po:
	$(MAKE) -C $(PO_DIR) update-po

.PHONY: check-syntax
check-syntax:
	gcc -o nul -S ${CHK_SOURCES}
