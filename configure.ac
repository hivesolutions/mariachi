# Hive Mariachi Engine
# Copyright (C) 2008 Hive Solutions Lda.
#
# This file is part of Hive Mariachi Engine.
#
# Hive Mariachi Engine is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Hive Mariachi Engine is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Hive Mariachi Engine. If not, see <http:#www.gnu.org/licenses/>.

# __author__    = João Magalhães <joamag@hive.pt>
# __version__   = 1.0.0
# __revision__  = $LastChangedRevision: 2390 $
# __date__      = $LastChangedDate: 2009-04-02 08:36:50 +0100 (qui, 02 Abr 2009) $
# __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
# __license__   = GNU General Public License (GPL), Version 3

AC_PREREQ(2.61)
AC_INIT(hive_mariachi, 1.0, development@hive.pt)
AM_INIT_AUTOMAKE(hive_mariachi, 1.0)
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])
LT_INIT
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL

# checks for programs
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_MAKE_SET

# checks for libraries
AC_CHECK_LIB([pthread], [main], [], [AC_MSG_ERROR(can't build without POSIX threads (libpthread) libraries)])
AC_CHECK_LIB([lua], [main], [], [AC_MSG_WARN(building without Lua (liblua) libraries support)])
AC_CHECK_LIB([mariachi], [main])

case "$host_os" in
*linux*)
    AC_CHECK_LIB([GL], [main], [], [AC_MSG_ERROR(can't build without OpenGL (libGL) libraries)])
    AC_CHECK_LIB([X11], [main], [], [AC_MSG_ERROR(can't build without X11 (libX11) libraries)])
    AC_CHECK_LIB([glut], [main], [], [AC_MSG_WARN(building without GLUT (libglut) libraries support)])
    ;;    
*darwin*)
    ;;
*)
    ;;
esac

# checks for header files
AC_CHECK_HEADERS([stdlib.h string.h GL/gl.h GL/glu.h GL/glut.h])

# checks for typedefs, structures, and compiler characteristics
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# checks for library functions
AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor memset])

AC_ARG_ENABLE([lua], AS_HELP_STRING([--enable-lua], [enable linking with lua @<:@default=no@:>@]), [link_lua=$enableval], [link_lua=yes])

AM_CONDITIONAL([LINK_LUA], [test "$link_lua" = "yes"])

if test "$link_lua" = "no"; then
    AC_MSG_WARN(building without Lua (liblua) libraries support)
fi

# sets the config files to be used in automake
AC_CONFIG_FILES([src/hive_mariachi/Makefile src/hive_mariachi_runner/Makefile lib/liblua/Makefile lib/liblua/src/Makefile lib/libbullet/Makefile lib/libbullet/src/Makefile])

# sets the entry point make files to be used by automake
AC_OUTPUT([Makefile src/Makefile lib/Makefile doc/Makefile man/Makefile examples/Makefile scripts/Makefile])

