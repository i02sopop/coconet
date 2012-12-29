LANGUAGES=es en

.PHONY: update-pot update-po new-lang all
update-pot:
	@xgettext -L C -j $(SOURCE_DIR)/*.c $(INCLUDE_DIR)/*.h -o coconet.pot

update-po:
	@for i in ${LANGUAGES}; do \
		msgmerge -U -i ${PO_DIR}/$$i.po ${PO_DIR}/coconet.pot; \
	done

new-lang:
	@for i in ${NEW_LANG}; do \
		msginit -i ${PO_DIR}/coconet.pot -o ${PO_DIR}/$$i.po; \
	done

all:
	@make update-pot
	@make update-po
