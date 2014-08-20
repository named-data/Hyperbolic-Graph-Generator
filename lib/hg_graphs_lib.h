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

#ifndef _HG_GRAPHS_LIB_H
#define _HG_GRAPHS_LIB_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
#include <utility>    
#include <algorithm>  
#include <cmath>
#include <math.h> 

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/exterior_property.hpp>


#include "hg_formats.h"

using namespace std;


//  ============= hg_graph I/O functions  ============= 

/* load a graph from file in .hg format
 * see the README file for more details on
 * the hg format
 */
hg_graph_t * hg_read_graph(const string filename);


/* print a graph to file in .hg format
 * see the README file for more details on
 * the hg format
 */
void hg_print_graph(const hg_graph_t *g, const string filename, 
		    unsigned int starting_id = 1);


//  =============  Random generator functions  ============= 

// Initialize random generator
void hg_init_random_generator(const unsigned int seed);

/* this function embeds the random generator we use
 * and returns a number between 0 and 1 */
double hg_rand_01_wrapper();



//  ============= graph's generation utilities  ============= 

/* generate a hyperbolic geometry graph having n nodes, an average degree
 * equal to k_bar, and a degree distribution having a gamma exponent equal to
 * exp_gamma. The pair (exp_gamma, temperature) defines the model to be used
 * for the graph generation as well as the geometry of the space in which the
 * graph is embedded
 */
hg_graph_t * hg_graph_generator(const int n, const double k_bar, const double exp_gamma,
				const double t, const double zeta, const int seed, 
				bool verbose = false);



//  ============= hyperbolic distance function  ============= 

/* hyperbolic distance function
 * returns the distance between the two coordinates according to
 * the graph provided. 
 */

double hg_hyperbolic_distance(const hg_graph_t * g,
			      const hg_coordinate_t & node1, 
			      const hg_coordinate_t & node2);



#endif /* _HG_GRAPHS_LIB_H */
