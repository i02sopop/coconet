AC_INIT([coconet],[1.0.2],[palvarez@ritho.net])
AC_PREREQ([2.69])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_AUX_DIR([.build-aux])
dnl AC_CONFIG_SUBDIRS([subdir])
AM_INIT_AUTOMAKE([foreign -Wall -Werror dist-bzip2])

dnl Gettext
AM_GNU_GETTEXT_VERSION([0.18.1])

dnl Maintainer mode
AM_MAINTAINER_MODE([enable])

dnl Check programs
dnl Find and test de C compiler
AC_PROG_CC
AC_LANG_C
AM_PROG_CC_C_O

AC_PROG_INSTALL
AC_PROG_MAKE_SET

dnl Check libraries
AC_CHECK_LIB([m], [sqrt], [LIBMATH=-lm], [AC_MSG_ERROR([Math library not found])])
AC_SUBST([LIBMATH])

dnl Checking headers 
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADER_STDBOOL
AC_HEADER_STDC

dnl Check typedefs, structs and compilers characteristics

dnl Checking library functions
AC_CHECK_FUNCS([strerror])
dnl AC_CHECK_FUNC(atol,,AC_MSG_ERROR(Oops! no atol ?!?))

AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC

dnl Variables for Makefiles.
AC_SUBST([PROJ_DIR], [`pwd`])

AC_SUBST([INCLUDE_DIR], [${PROJ_DIR}/include])
AC_SUBST([SOURCE_DIR], [${PROJ_DIR}/src])
AC_SUBST([PO_DIR], [${PROJ_DIR}/po])
dnl AC_SUBST([MAKE], [make])

dnl Output files
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile po/Makefile tests/Makefile])

dnl Generate output
AC_OUTPUT
