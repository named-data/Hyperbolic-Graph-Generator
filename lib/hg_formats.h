/*
 * Hyperbolic Graph Generator
 *
 * Chiara Orsini, CAIDA, UC San Diego
 * chiara@caida.org
 *
 * Copyright (C) 2014 The Regents of the University of California.
 *
 * This file is part of the Hyperbolic Graph Generator.
 *
 * The Hyperbolic Graph Generator is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or  (at your option) any later version.
 *
 * The Hyperbolic Graph Generator is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Hyperbolic Graph Generator.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef _HG_FORMATS_H
#define _HG_FORMATS_H

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/exterior_property.hpp>

using namespace std;

// constants definition

#define HG_PI M_PI // 3.14159265359
#define HG_INF_TEMPERATURE 10
#define HG_INF_GAMMA 10         
#define HG_INF_RADIUS 1000


/* Polar coordinates of a node in a
 * hyperbolic space */
typedef struct struct_hg_coordinate {
  double r;     // radial coordinate, distance from the origin
  double theta; // angular coordinate, angular distance from a reference point
} hg_coordinate_t;

typedef enum {
  HYPERBOLIC_RGG,
  HYPERBOLIC_STANDARD,
  SOFT_CONFIGURATION_MODEL,
  ANGULAR_RGG, 
  SOFT_RGG,
  ERDOS_RENYI
} hg_graph_type;


/* Parameters describing a graph
 * generated in a hyperbolic space */
typedef struct struct_hg_parameters {
  hg_graph_type type;
  int expected_n;
  double temperature;
  double expected_gamma;
  double expected_degree;
  int seed;
  double zeta_eta;
} hg_parameters_t;

/* Graph generation internal parameters */
typedef struct struct_hg_algorithm_parameters_t {
  double radius;
  double alpha;
  double eta;
  double c;
} hg_algorithm_parameters_t;


/* Structures used for numerical *
 * integration. Not all fields   *
 * are used every time           */
typedef struct struct_f_params{
  double R;
  double alpha; 
  double zeta; 
  double eta;
  double beta;
} hg_f_params;
  

/* customized boost graph */
typedef boost::adjacency_list<boost::setS, /* container for the edgelist
					    * for each of the vertices
					    * - no duplicate edges */
                              boost::vecS, /* container for vertexlist
					    * - no duplicate nodes */
                              boost::undirectedS, /* undirected graph selector */
                              hg_coordinate_t,    /* vertex coordinate */
                              boost::no_property, /* edge properties */
                              hg_parameters_t     /* graph parameters */
                              > hg_graph_t;


#endif /* _HG_FORMATS_H */
