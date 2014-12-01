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

include $(top_srcdir)/Common.am

lib_LIBRARIES = liblua.a

liblua_a_CFLAGS = $(AM_CFLAGS)
liblua_a_CXXFLAGS = $(AM_CXXFLAGS)

liblua_a_SOURCES = lapi.c \
lauxlib.c \
lbaselib.c \
lcode.c \
ldblib.c \
ldebug.c \
ldo.c \
ldump.c \
lfunc.c \
lgc.c \
linit.c \
liolib.c \
llex.c \
lmathlib.c \
lmem.c \
loadlib.c \
lobject.c \
lopcodes.c \
loslib.c \
lparser.c \
lstate.c \
lstring.c \
lstrlib.c \
ltable.c \
ltablib.c \
ltm.c \
lua.c \
luac.c \
lundump.c \
lvm.c \
lzio.c \
print.c

