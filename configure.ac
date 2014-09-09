#
# Hyperbolic Graph Generator
#
# Chiara Orsini, CAIDA, UC San Diego
# chiara@caida.org
#
# Copyright (C) 2014 The Regents of the University of California.
#
# This file is part of the Hyperbolic Graph Generator.
#
# The Hyperbolic Graph Generator is free software: you can redistribute
# it and/or modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version 3 of the
# License, or  (at your option) any later version.
#
# The Hyperbolic Graph Generator is distributed in the hope that it will
# be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the Hyperbolic Graph Generator.
# If not, see <http://www.gnu.org/licenses/>.
#

AC_PREREQ([2.61])

AC_INIT([Hyperbolic_Graph_Generator], [1.0.1], [chiara@caida.org])
AC_LANG_PUSH([C++])

HYPERBOLIC_GRAPH_GENERATOR_MAJOR_VERSION=0
HYPERBOLIC_GRAPH_GENERATOR_MID_VERSION=0
HYPERBOLIC_GRAPH_GENERATOR_MINOR_VERSION=0

AC_DEFINE_UNQUOTED([HYPERBOLIC_GRAPH_GENERATOR_MAJOR_VERSION],$HYPERBOLIC_GRAPH_GENERATOR_MAJOR_VERSION,
	[Hyperbolic_Graph_Generator major version])
AC_DEFINE_UNQUOTED([HYPERBOLIC_GRAPH_GENERATOR_MID_VERSION],$HYPERBOLIC_GRAPH_GENERATOR_MID_VERSION,
	[Hyperbolic_Graph_Generator mid version])
AC_DEFINE_UNQUOTED([HYPERBOLIC_GRAPH_GENERATOR_MINOR_VERSION],$HYPERBOLIC_GRAPH_GENERATOR_MINOR_VERSION,
	[Hyperbolic_Graph_Generator minor version])

LT_INIT

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_SRCDIR([lib/hg_graphs_lib.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CXX
AM_PROG_LIBTOOL
AC_PROG_CC_C99

AC_SYS_LARGEFILE

AH_VERBATIM([_GNU_SOURCE],
[/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif])

AC_CHECK_FUNCS([gettimeofday memset strdup strstr strsep strlcpy vasprintf])

# should we dump debug output to stderr and not optmize the build?

AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
        [enable debug data generation (def=no)])],
    [debugit="$enableval"],
    [debugit=no])
AC_MSG_RESULT([$debugit])

if test x"$debugit" = x"yes"; then
    AC_DEFINE([DEBUG],[],[Debug Mode])
else
    AC_DEFINE([NDEBUG],[],[No-debug Mode])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC


# Checks for header files.
AC_CHECK_HEADERS([math.h stdlib.h string.h time.h sys/time.h])
AC_CHECK_HEADERS([boost/graph/adjacency_list.hpp], , [AC_MSG_ERROR( [boost libraries not found, please see README])] )
AC_CHECK_HEADERS([gsl/gsl_math.h], , [AC_MSG_ERROR( [gsl libraries not found, please see README])] )


# Checks for libraries.
# Ref: http://www.gnu.org/software/gsl/manual/html_node/Autoconf-Macros.html
AC_CHECK_LIB([m],[cos], ,  [AC_MSG_ERROR( [libm required ])])
AC_CHECK_LIB([gslcblas],[cblas_dgemm], ,  [AC_MSG_ERROR( [gsl libraries not found, please see README ])])
AC_CHECK_LIB([gsl],[gsl_blas_dgemm], ,  [AC_MSG_ERROR( [gsl libraries not found, please see README])])



# we may want to come back later and add compile-time configuration for things
# like datastructure providers, but for now it will all get compiled


AC_SUBST([HYPERBOLIC_GRAPH_GENERATOR_MAJOR_VERSION])
AC_SUBST([HYPERBOLIC_GRAPH_GENERATOR_MID_VERSION])
AC_SUBST([HYPERBOLIC_GRAPH_GENERATOR_MINOR_VERSION])

AC_HEADER_ASSERT

AC_CONFIG_FILES([Makefile
		 tools/Makefile
		 ])

AC_CONFIG_SUBDIRS([lib])

AC_OUTPUT
