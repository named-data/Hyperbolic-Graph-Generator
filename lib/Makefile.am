#
# The HG-graphs package
#
# Chiara Orsini, CAIDA, UC San Diego
# chiara@caida.org
#
# Copyright (C) 2014 The Regents of the University of California.
#
# This file is part of the HG-graphs package.
#
# The HG-graphs package is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# The HG-graphs package is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with theHG-graphs package.
# If not, see <http://www.gnu.org/licenses/>.
#
#

# AM_CPPFLAGS - The contents of this variable are passed to every compilation that invokes the C preprocessor; it is a list of arguments to the preprocessor. For instance, -I and -D options should be listed here.
AM_CPPFLAGS = 

EXTRA_DIST = README.md

# Each _LTLIBRARIES variable is a list of libtool libraries to build. 
lib_LTLIBRARIES = libhggraphs.la

# Public header files that need to be installed in order for people to use the library.
include_HEADERS = hg_graphs_lib.h hg_formats.h hg_gen_algorithms.h hg_utils.h hg_debug.h

libhggraphs_la_SOURCES = 	\
	hg_graphs_lib.h		\
	hg_graphs_lib.cpp	\
	hg_formats.h		\
	hg_gen_algorithms.h	\
	hg_gen_algorithms.cpp	\
	hg_random.h		\
	hg_random.cpp		\
	hg_utils.h		\
	hg_utils.cpp		\
	hg_debug.h		\
	hg_math.cpp		\
	hg_math.h

libhggraphs_la_LIBADD = 

libhggraphs_la_LDFLAGS = -version-info @LIBHGGRAPHS_MAJOR_VERSION@:@LIBHGGRAPHS_MINOR_VERSION@:@LIBHGGRAPHS_MID_VERSION@

ACLOCAL_AMFLAGS = -I m4

CLEANFILES = *~
