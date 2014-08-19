/*
 * Hyperbolic Graph Generator
 *
 * Rodrigo Aldecoa, Northeastern University
 * raldecoa@neu.edu
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

#ifndef _HG_MATH_H
#define _HG_MATH_H

#include "hg_formats.h"




/* Numerical integration */

// Heaviside function O(R-x)

// Get R from numerical integration
double hg_get_R(hg_graph_t * graph, hg_algorithm_parameters_t & p);

// Get lambda from hypergeometric function
double hg_get_lambda(hg_graph_t * graph, hg_algorithm_parameters_t & p);



#endif /* _HG_MATH_H */
