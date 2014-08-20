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


AM_CPPFLAGS = -I$(top_srcdir)  -I$(top_srcdir)/lib 


bin_PROGRAMS = hyperbolic_graph_generator greedy_routing graph_properties


hyperbolic_graph_generator_SOURCES = hyperbolic_graph_generator.cpp 
hyperbolic_graph_generator_LDADD = -lhggraphs
hyperbolic_graph_generator_LDFLAGS = -L$(top_builddir)/lib


greedy_routing_SOURCES = greedy_routing.cpp
greedy_routing_LDADD = -lhggraphs
greedy_routing_LDFLAGS = -L$(top_builddir)/lib


graph_properties_SOURCES = graph_properties.cpp \
			   stats.h \
			   stats.cpp 
graph_properties_LDADD = -lhggraphs
graph_properties_LDFLAGS = -L$(top_builddir)/lib



ACLOCAL_AMFLAGS = -I m4

CLEANFILES = *~
