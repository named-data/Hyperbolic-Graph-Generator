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


#include <assert.h>     /* assert */
#include <limits>

#include "hg_gen_algorithms.h"
#include "hg_utils.h"
#include "hg_random.h"
#include "hg_math.h"
#include "hg_debug.h"


/* ================= graph construction utilities ================= */


/* r_precomputedsinhcosh is a structure that contains the precomputed values
 * of sinh(zeta * r) and cosh(zeta * r) for each value of r. It is an
 * STL map:
 *         <r, < sinh(zeta*r), cosh(zeta*r) > >  
 * this structure does not need to be exported outside this coding unit
 */
typedef map<double, pair<double,double> > r_precomputedsinhcosh;


static void hg_assign_coordinates(hg_graph_t * graph, const hg_algorithm_parameters_t & in_par,
				  r_precomputedsinhcosh * r_psc = NULL) {
  hg_debug("\tAssigning coordinates");
  int id; // node identifier [0,n-1]
  switch((*graph)[boost::graph_bundle].type){
  case HYPERBOLIC_RGG: 
  case HYPERBOLIC_STANDARD:
  case SOFT_CONFIGURATION_MODEL:
    for(id = 0; id < (*graph)[boost::graph_bundle].expected_n; id++) {
      double zeta = (*graph)[boost::graph_bundle].zeta_eta;
      double r = hg_quasi_uniform_radial_coordinate(in_par.radius, in_par.alpha);
      (*graph)[id].r = r;
      if(r_psc != NULL) {
	(*r_psc).insert(make_pair(r, make_pair(sinh(zeta * r), cosh(zeta * r))));
      }
      (*graph)[id].theta = hg_uniform_angular_coordinate();
    }
    break;
  case ANGULAR_RGG:
  case SOFT_RGG:
  case ERDOS_RENYI:
    for(id = 0; id < (*graph)[boost::graph_bundle].expected_n; id++) {
      (*graph)[id].r = in_par.radius; // HG_INF_RADIUS
      (*graph)[id].theta = hg_uniform_angular_coordinate();
    }
    break;
  default:
    hg_log_err("Case not implemented.");
  }
}


static void hg_init_graph(hg_graph_t * graph, const int & n, const double & k_bar, 
			  const double & exp_gamma, const double & t, 
			  const double & zeta_eta, const int & seed, 
			  const hg_graph_type & gt) {
  hg_debug("\tGraph initialization");
  /* initialize the graph structure with the 
   * parameters provided in input by the user */
  (*graph)[boost::graph_bundle].type = gt;
  (*graph)[boost::graph_bundle].expected_n = n;
  (*graph)[boost::graph_bundle].temperature = t;
  (*graph)[boost::graph_bundle].expected_gamma = exp_gamma;
  (*graph)[boost::graph_bundle].expected_degree = k_bar;
  (*graph)[boost::graph_bundle].zeta_eta = zeta_eta;
  (*graph)[boost::graph_bundle].seed = seed;  
  // Init random generator
  HG_Random::init(seed);  
}


/* ================= useful mathematical functions  ================= */



static double hg_get_R_from_numerical_integration(hg_graph_t * graph, hg_algorithm_parameters_t & p) {
  return hg_get_R(graph, p);  
}


static double hg_get_lambda_from_Gauss_hypergeometric_function(hg_graph_t * graph, hg_algorithm_parameters_t & p) {
  return hg_get_lambda(graph, p);
}


/* ================= single model graph generators  ================= */


inline double hg_hyperbolic_distance_hyperbolic_rgg_standard(const hg_graph_t * graph,
							     const hg_coordinate_t & node1, 
							     const hg_coordinate_t & node2,
							     r_precomputedsinhcosh * r_psc = NULL) {
  // check if it is the same node
  if(node1.r == node2.r && node1.theta == node2.theta) {
    return 0;
  }
  // if the nodes have the same angular coordinates
  // then we return the euclidean distance
  if(node1.theta == node2.theta) {
    return abs(node1.r-node2.r);
  }
  // equation 13
  double zeta = (*graph)[boost::graph_bundle].zeta_eta;
  double delta_theta = HG_PI - abs(HG_PI - abs(node1.theta - node2.theta));
  double part1, part2;
  if(r_psc != NULL) {
    part1 = (*r_psc)[node1.r].second * (*r_psc)[node2.r].second;
    part2 = (*r_psc)[node1.r].first  * (*r_psc)[node2.r].first  * cos(delta_theta);
  }
  else {    
    part1 = cosh(zeta * node1.r) * cosh(zeta * node2.r);
    part2 = sinh(zeta * node1.r) * sinh(zeta * node2.r) * cos(delta_theta);
  }
  return  acosh(part1 - part2) / zeta;
}


static double hg_connection_probability_hyperbolic_rgg(const hg_graph_t * graph,
						       const hg_algorithm_parameters_t & p,
						       const hg_coordinate_t & node1, 
						       const hg_coordinate_t & node2,
						       r_precomputedsinhcosh * r_psc = NULL) {
  // equation 32: Heaviside function
  if(hg_hyperbolic_distance_hyperbolic_rgg_standard(graph, node1, node2, r_psc) <= p.radius) {
    return 1;
  }
  return 0;
}


hg_graph_t * hg_hyperbolic_rgg(const int n, const double k_bar, 
			       const double exp_gamma,
			       const double zeta, const int seed, 
			       bool verbose) {
  if(verbose) {
    cout << "-> Hyperbolic Random Geometric Graph" << endl << endl;
  }
  hg_graph_t * graph = new hg_graph_t(n);
  if(graph == NULL) {
    hg_log_warn("Unable to allocate memory for graph object");
    return NULL;
  }
  // init graph
  hg_init_graph(graph, n, k_bar, exp_gamma, 0 /* t = 0 */,
		zeta, seed, HYPERBOLIC_RGG);
  // computing internal parameters
  hg_debug("\tInternal parameters computation");
  hg_algorithm_parameters_t p;
  p.alpha = 0.5 * zeta * (exp_gamma-1.0);
  p.eta = -1; // not relevant for current model
  p.c = -1;  // not relevant for current model
  p.radius = hg_get_R_from_numerical_integration(graph, p);
  r_precomputedsinhcosh r_psc; 
  hg_assign_coordinates(graph, p, &r_psc);
  hg_coordinate_t c1, c2;
  int id, other_id;
  hg_debug("\tInternal parameters:");
  hg_debug("\t\tAlpha: %f", p.alpha);
  hg_debug("\t\tRadius: %f", p.radius);
  hg_debug("\tCreating links");
  for(id = 0; id < (*graph)[boost::graph_bundle].expected_n; id++) {
    c1 = hg_get_coordinate(graph, id);
    for(other_id = id+1; other_id < (*graph)[boost::graph_bundle].expected_n; other_id++) {
      c2 = hg_get_coordinate(graph, other_id);
      if(HG_Random::get_random_01_value() < hg_connection_probability_hyperbolic_rgg(graph, p, c1, c2, &r_psc)){
	add_edge(id, other_id, *graph);
	//hg_debug("\t\tNew link: %d - %d ", id, other_id);
      }
    }
  }
  return graph;
}


inline double hg_connection_probability_hyperbolic_standard(const hg_graph_t * graph,
							    const hg_algorithm_parameters_t & p,
							    const hg_coordinate_t & node1, 
							    const hg_coordinate_t & node2,
							    r_precomputedsinhcosh * r_psc = NULL) {
  // check if it is the same node
  if(node1.r == node2.r && node1.theta == node2.theta) {
    return 0;
  }
  // equation 12: Fermi-Dirac function
  double zeta = (*graph)[boost::graph_bundle].zeta_eta;
  double t =  (*graph)[boost::graph_bundle].temperature;
  double x = hg_hyperbolic_distance_hyperbolic_rgg_standard(graph, node1, node2, r_psc);
  double exponent = (double)1.0/t * zeta/2.0  * (x - p.radius);
  return 1.0 / (exp(exponent) + 1.0); 
}


hg_graph_t * hg_hyperbolic_standard(const int n, const double k_bar, 
				    const double exp_gamma,
				    const double temperature,			 			       
				    const double zeta, const int seed, 
				    bool verbose) {
  if(verbose) {
    cout << "-> Hyperbolic Standard Graph" << endl << endl;
  }
  hg_graph_t * graph = new hg_graph_t(n);
  if(graph == NULL) {
    hg_log_warn("Unable to allocate memory for graph object");
    return NULL;
  }
  // init graph
  hg_init_graph(graph, n, k_bar, exp_gamma, temperature,
		zeta, seed, HYPERBOLIC_STANDARD);
  // computing internal parameters
  hg_debug("\tInternal parameters computation");
  hg_algorithm_parameters_t p;
  // alpha calculation. different for cold and hot regimes
  if(temperature <= 1) {
    p.alpha = 0.5 * zeta * (exp_gamma-1.0);
  }
  else {
    p.alpha = 0.5 * zeta/temperature * (exp_gamma-1.0);
  }
  p.eta = -1; // not relevant for current model
  p.c = -1;  // not relevant for current model
  p.radius = hg_get_R_from_numerical_integration(graph, p);
  hg_debug("\tInternal parameters:");
  hg_debug("\t\tAlpha: %f", p.alpha);
  hg_debug("\t\tRadius: %f", p.radius);
  r_precomputedsinhcosh r_psc; 
  hg_assign_coordinates(graph, p, &r_psc);
  hg_coordinate_t c1, c2;
  int id, other_id;
  hg_debug("\tCreating links");
  for(id = 0; id < (*graph)[boost::graph_bundle].expected_n; id++) {
    c1 = hg_get_coordinate(graph, id);
    for(other_id = id+1; other_id < (*graph)[boost::graph_bundle].expected_n; other_id++) {
      c2 = hg_get_coordinate(graph, other_id);
      if(HG_Random::get_random_01_value() < hg_connection_probability_hyperbolic_standard(graph, p, c1, c2, &r_psc)) {
	add_edge(id, other_id, *graph);
	//hg_debug("\t\tNew link: %d - %d ", id, other_id);
      }
    }
  }
  return graph;
}


static double hg_hyperbolic_distance_scm(const hg_graph_t * graph,
					 const hg_coordinate_t & node1, 
					 const hg_coordinate_t & node2) {
  // check if it is the same node
  if(node1.r == node2.r && node1.theta == node2.theta) {
    return 0;
  }
  // curvature is infinite, so 1/zeta goes to zero  
  return node1.r + node2.r;
}


static double hg_connection_probability_scm(const hg_graph_t * graph,
					    const hg_algorithm_parameters_t & p,
					    const hg_coordinate_t & node1, 
					    const hg_coordinate_t & node2) {
  // equation (39)
  double x = hg_hyperbolic_distance_scm(graph, node1, node2);
  double exponent = (double) p.eta/2.0  * (x - p.radius);
  return 1.0 / (exp(exponent) + 1.0); 
}


hg_graph_t * hg_soft_configuration_model(const int n, const double k_bar, 
					 const double exp_gamma,
					 const double eta, const int seed, 
					 bool verbose) {
  if(verbose) {
    cout << "-> Soft Configuration Model Graph" << endl << endl;
  }
  hg_graph_t * graph = new hg_graph_t(n);
  if(graph == NULL) {
    hg_log_warn("Unable to allocate memory for graph object");
    return NULL;
  }
  // init graph
  hg_init_graph(graph, n, k_bar, exp_gamma, HG_INF_TEMPERATURE /* t = inf */,
		eta, seed, SOFT_CONFIGURATION_MODEL);
  // computing internal parameters
  hg_debug("\tInternal parameters computation");
  // zeta goes to infinity
  //(*graph)[boost::graph_bundle].zeta_eta = numeric_limits<double>::max( );
  hg_algorithm_parameters_t p;
  p.eta = (*graph)[boost::graph_bundle].zeta_eta;
  p.alpha = 0.5 * p.eta * (exp_gamma-1.0);
  p.c = -1;  // not relevant for current model
  p.radius = hg_get_R_from_numerical_integration(graph, p);
  hg_debug("\t\talpha: %f", p.alpha);
  hg_debug("\t\teta: %f", p.eta);
  hg_debug("\t\tradius: %f", p.radius);
  hg_assign_coordinates(graph, p);
  hg_coordinate_t c1, c2;
  int id, other_id;
  hg_debug("\tCreating links");
  for(id = 0; id < (*graph)[boost::graph_bundle].expected_n; id++) {
    c1 = hg_get_coordinate(graph, id);
    for(other_id = id+1; other_id < (*graph)[boost::graph_bundle].expected_n; other_id++) {
      c2 = hg_get_coordinate(graph, other_id);
      if(HG_Random::get_random_01_value() < hg_connection_probability_scm(graph, p, c1, c2)) {
	add_edge(id, other_id, *graph);
	//hg_debug("\t\tNew link: %d - %d ", id, other_id);
      }
    }
  }
  return graph;
}


static double hg_hyperbolic_distance_angular_soft_rgg(const hg_graph_t * graph,
						      const hg_coordinate_t & node1, 
						      const hg_coordinate_t & node2) {
  // check if it is the same node
  if(node1.r == node2.r && node1.theta == node2.theta) {
    return 0;
  }
  // delta theta
  return HG_PI - abs(HG_PI - abs(node1.theta - node2.theta));
}


static double hg_connection_probability_angular_rgg(const hg_graph_t * graph,
						    const hg_coordinate_t & node1, 
						    const hg_coordinate_t & node2) {
  // equation 55: Heaviside function
  if(hg_hyperbolic_distance_angular_soft_rgg(graph, node1, node2) <=
     HG_PI * (*graph)[boost::graph_bundle].expected_degree / (double) (*graph)[boost::graph_bundle].expected_n) {
    return 1;
  }
  return 0;
}


hg_graph_t * hg_angular_rgg(const int n, const double k_bar, 			     
			    const double zeta, const int seed, 
			    bool verbose) {
  if(verbose) {
    cout << "-> Angular Random Geometric Graph" << endl << endl;
  }
  hg_graph_t * graph = new hg_graph_t(n);
  if(graph == NULL) {
    hg_log_warn("Unable to allocate memory for graph object");
    return NULL;
  }
  // init graph
  hg_init_graph(graph, n, k_bar, HG_INF_GAMMA /* exp_gamma = inf */, 0 /* t = 0 */,
		zeta, seed, ANGULAR_RGG);
  // computing internal parameters
  hg_debug("\tInternal parameters computation");
  hg_algorithm_parameters_t p;
  p.radius = HG_INF_RADIUS; 
  p.alpha = -1; // not relevant for current model
  p.eta = -1; // not relevant for current model
  p.c = -1;  // not relevant for current model
  hg_assign_coordinates(graph, p);
  hg_coordinate_t c1, c2;
  int id, other_id;
  hg_debug("\tCreating links");
  for(id = 0; id < (*graph)[boost::graph_bundle].expected_n; id++) {
    c1 = hg_get_coordinate(graph, id);
    for(other_id = id+1; other_id < (*graph)[boost::graph_bundle].expected_n; other_id++) {
      c2 = hg_get_coordinate(graph, other_id);
      if(HG_Random::get_random_01_value() < hg_connection_probability_angular_rgg(graph, c1, c2)) {
	add_edge(id, other_id, *graph);
	hg_debug("\t\tNew link: %d - %d ", id, other_id);
      }
    }
  }
  return graph;
}



static double hg_connection_probability_soft_rgg(const hg_graph_t * graph,
						 const hg_algorithm_parameters_t & p,
						 const hg_coordinate_t & node1, 
						 const hg_coordinate_t & node2) {
  double x = hg_hyperbolic_distance_angular_soft_rgg(graph, node1, node2);
  // equation 46
  double beta = 1.0 / (*graph)[boost::graph_bundle].temperature;  
  return 1.0 / (1.0 + p.c * pow((x / HG_PI), beta) );
}



hg_graph_t * hg_soft_rgg(const int n, const double k_bar, 
			 const double temperature,			 
			 const double zeta, const int seed, 
			 bool verbose) {
  if(verbose) {
    cout << "-> Soft Random Geometric Graph" << endl << endl;
  }
  hg_graph_t * graph = new hg_graph_t(n);
  if(graph == NULL) {
    hg_log_warn("Unable to allocate memory for graph object");
    return NULL;
  }
  // init graph
  hg_init_graph(graph, n, k_bar, HG_INF_GAMMA /* exp_gamma = inf */, temperature /* t = 0 */,
		zeta, seed, SOFT_RGG);
  // computing internal parameters
  hg_debug("\tInternal parameters computation");
  hg_algorithm_parameters_t p;
  p.radius = HG_INF_RADIUS; 
  p.alpha = -1; // not relevant for current model
  p.eta = -1; // not relevant for current model
  p.c = hg_get_lambda_from_Gauss_hypergeometric_function(graph,p); 
  hg_assign_coordinates(graph, p);
  hg_coordinate_t c1, c2;
  int id, other_id;
  hg_debug("\tCreating links");
  for(id = 0; id < (*graph)[boost::graph_bundle].expected_n; id++) {
    c1 = hg_get_coordinate(graph, id);
    for(other_id = id+1; other_id < (*graph)[boost::graph_bundle].expected_n; other_id++) {
      c2 = hg_get_coordinate(graph, other_id);
      if(HG_Random::get_random_01_value() < hg_connection_probability_soft_rgg(graph, p, c1, c2)) {
	add_edge(id, other_id, *graph);
	hg_debug("\t\tNew link: %d - %d ", id, other_id);
      }
    }
  }
  return graph;
}


static double hg_hyperbolic_distance_er(const hg_graph_t * graph,
					const hg_coordinate_t & node1, 
					const hg_coordinate_t & node2) {
  // check if it is the same node
  if(node1.r == node2.r && node1.theta == node2.theta) {
    return 0;
  }
  // there is no "real distance", indeed!
  return 1; 
}

static double hg_connection_probability_er(const hg_graph_t * graph,
					   const hg_coordinate_t & node1, 
					   const hg_coordinate_t & node2) {
  // connection probability is given
  // by equation 61
  return 1.0 / (1.0 + (double)((double)(*graph)[boost::graph_bundle].expected_n/(*graph)[boost::graph_bundle].expected_degree)); 
}


hg_graph_t * hg_erdos_renyi(const int n, const double k_bar, 			     
			    const double zeta, const int seed, 
			    bool verbose) {
  if(verbose) {
    cout << "-> Erdos-Renyi Graph" << endl << endl;
  }
  hg_graph_t * graph = new hg_graph_t(n);
  if(graph == NULL) {
    hg_log_warn("Unable to allocate memory for graph object");
    return NULL;
  }
  // init graph
  hg_init_graph(graph, n, k_bar, HG_INF_GAMMA /* exp_gamma = inf */, HG_INF_TEMPERATURE /* t = inf */,
		zeta, seed, ERDOS_RENYI);
  // computing internal parameters
  hg_debug("\tInternal parameters computation");
  hg_algorithm_parameters_t p;
  p.radius = HG_INF_RADIUS; 
  p.alpha = -1; // not relevant for current model
  p.eta = -1; // not relevant for current model
  hg_debug("\t\tradius: %d (INF)", HG_INF_RADIUS);
  hg_assign_coordinates(graph, p);
  hg_coordinate_t c1, c2;
  int id, other_id;
  hg_debug("\tCreating links");
  for(id = 0; id < (*graph)[boost::graph_bundle].expected_n; id++) {
    c1 = hg_get_coordinate(graph, id);
    for(other_id = id+1; other_id < (*graph)[boost::graph_bundle].expected_n; other_id++) {
      c2 = hg_get_coordinate(graph, other_id);      
      if(HG_Random::get_random_01_value() < hg_connection_probability_er(graph, c1, c2)) {
	add_edge(id, other_id, *graph);
	hg_debug("\t\tNew link: %d - %d ", id, other_id);
      }
    }
  }
  return graph;
}


/* ================= hyperbolic distance function  ================= */


double hg_hyperbolic_distance(const hg_graph_t * g,
			      const hg_coordinate_t & node1, 
			      const hg_coordinate_t & node2){
  if(node1.r == node2.r && node1.theta == node2.theta) {
    return 0;
  }
  switch((*g)[boost::graph_bundle].type){
  case HYPERBOLIC_RGG: 
  case HYPERBOLIC_STANDARD:
    return hg_hyperbolic_distance_hyperbolic_rgg_standard(g, node1, node2);
  case SOFT_CONFIGURATION_MODEL:
    return hg_hyperbolic_distance_scm(g, node1, node2);
  case ANGULAR_RGG:
  case SOFT_RGG:
    return hg_hyperbolic_distance_angular_soft_rgg(g, node1, node2);
  case ERDOS_RENYI:
    return hg_hyperbolic_distance_er(g, node1, node2);
  default:
    // case not implemented
    hg_log_warn("Case not implemented yet");
  }
  return -1;
}
