#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([percolwash], [0.0], [])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([game.cc])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
AC_CHECK_LIB(ncurses,initscr,[AC_DEFINE(LIB_NCURSES)
LDFLAGS="-lncurses $LDFLAGS"],
[AC_CHECK_LIB(curses,initscr,[AC_DEFINE(LIB_CURSES)
LDFLAGS="-lcurses $LDFLAGS"],[
                AC_MSG_ERROR(
can't continue...
this package needs curses library.)
	        ])
])
# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_ARG_WITH(color,
[ --without-color don't use color ],[],
[AC_DEFINE(USE_COLOR)])

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
