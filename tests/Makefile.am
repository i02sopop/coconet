TESTS_OBJ  = check_utils.o
#TESTS_OBJ +=

TESTS = coconet_check
check_PROGRAMS = coconet_check
coconet_check_SOURCES = main.c utils.c
coconet_check_CFLAGS = -I$(INCLUDE_DIR) -DLOCALEDIR=\"$(localedir)\" @CHECK_CFLAGS@
coconet_check_LDADD = $(TESTS_OBJ) $(LIBMATH) $(LIBINTL) @CHECK_LIBS@ 

check_%.o: $(SOURCE_DIR)/%.c
	$(COMPILE) -I$(INCLUDE_DIR) -DLOCALEDIR=\"$(localedir)\" -DCHECKING -c -o $@ $^
