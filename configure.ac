#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([GTK Centre Pointer], [0.0.4], [nobody], [gcptr], [])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_MKDIR_P

# Checks for libraries.
PKG_CHECK_MODULES([glib], [glib-2.0])
AC_SUBST(glib_LIBS)
AC_SUBST(glib_CFLAGS)

PKG_CHECK_MODULES([gtk], [gtk+-3.0])
AC_SUBST(gtk_LIBS)
AC_SUBST(gtk_CFLAGS)

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([
	Makefile
	man/Makefile
	src/Makefile
])
AC_OUTPUT
