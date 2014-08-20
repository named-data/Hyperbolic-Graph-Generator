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

#ifndef _HG_GEN_ALGORITHMS_H
#define _HG_GEN_ALGORITHMS_H

#include "hg_formats.h"
#include "hg_utils.h"
#include "hg_random.h"


double hg_hyperbolic_distance(const hg_graph_t * g,
			      const hg_coordinate_t & node1, 
			      const hg_coordinate_t & node2);



hg_graph_t * hg_hyperbolic_rgg(const int n, const double k_bar, 
			       const double exp_gamma,
			       const double zeta, const int seed, 
			       bool verbose = false);


hg_graph_t * hg_hyperbolic_standard(const int n, const double k_bar, 
				    const double exp_gamma,
				    const double temperature,			 			       
				    const double zeta, const int seed, 
				    bool verbose = false);


hg_graph_t * hg_soft_configuration_model(const int n, const double k_bar, 
					 const double exp_gamma,
					 const double zeta, const int seed, 
					 bool verbose = false);


hg_graph_t * hg_angular_rgg(const int n, const double k_bar, 			     
			    const double zeta, const int seed, 
			    bool verbose = false);


hg_graph_t * hg_soft_rgg(const int n, const double k_bar, 
			 const double temperature,			 
			 const double zeta, const int seed, 
			 bool verbose = false);


hg_graph_t * hg_erdos_renyi(const int n, const double k_bar, 			     
			    const double zeta, const int seed, 
			    bool verbose = false);






#endif /* _HG_GEN_ALGORITHMS_H */
