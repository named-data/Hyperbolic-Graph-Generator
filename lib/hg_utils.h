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


#ifndef _HG_UTILS_H
#define _HG_UTILS_H

#include "hg_formats.h"
#include "hg_random.h"



// infer graph type
hg_graph_type hg_infer_hg_type(const double & expected_gamma, const double & temperature);
hg_graph_type hg_infer_hg_type(const hg_graph_t * g);

// get coordinates of node id
hg_coordinate_t hg_get_coordinate(const hg_graph_t * graph, const int id);



//  ============= coordinates' random extraction utilities  ============= 

/* extract a radial coordinate value uniformly from an 
 * exponential distribution (0,radius)
 */
double hg_uniform_radial_coordinate(const double radius);

/* extract a radial coordinate value quasi-uniformly from an 
 * exponential distribution (0,radius). alpha defines how
 * uniform is the extraction.
 */
double hg_quasi_uniform_radial_coordinate(const double radius,
					  const double alpha);

/* extract an angular coordinate value uniformly at random
 * from the uniform distribution [0, 2 PI] 
 */
double hg_uniform_angular_coordinate();




#endif /* _HG_UTILS_H */
