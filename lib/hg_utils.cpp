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

#include "hg_utils.h"
#include "hg_random.h"
#include "hg_debug.h"


hg_graph_type hg_infer_hg_type(const double & expected_gamma, const double & temperature) {
  if(expected_gamma < HG_INF_GAMMA) { // finite gamma
    if(temperature == 0) {      
      return HYPERBOLIC_RGG;
    }
    else{
      if(temperature < HG_INF_TEMPERATURE) {
	return HYPERBOLIC_STANDARD;
      }
      else {
	return SOFT_CONFIGURATION_MODEL;
      }      
    }
  }
  else { // gamma = infinite
    if(temperature == 0) {
      return ANGULAR_RGG;
    }
    else{
      if(temperature < HG_INF_TEMPERATURE) {
	return SOFT_RGG;
      }
      else {
	return ERDOS_RENYI;
      }      
    }
  }
  hg_log_warn("Case not implemented.");
  return ERDOS_RENYI;
}


hg_graph_type hg_infer_hg_type(const hg_graph_t * graph) {
  if(graph ==  NULL) {
    hg_log_err("Null graph provided.");
    return ERDOS_RENYI;  
  }
  return hg_infer_hg_type((*graph)[boost::graph_bundle].expected_gamma, (*graph)[boost::graph_bundle].temperature);
}



hg_coordinate_t hg_get_coordinate(const hg_graph_t * graph, const int id) {
  assert(graph != NULL);
  hg_coordinate_t c;
  c.r = (*graph)[id].r;
  c.theta = (*graph)[id].theta;
  return c;
}


/* How to extract from any probability distribution:
 * The probability integral transform states that if X is
 * a continuous random variable with cumulative distribution
 * function F_X, then the random variable Y=F_X(X) has a
 * uniform distribution on [0, 1].
 * The inverse probability integral transform is just the
 * inverse of this: specifically, if Y has a uniform distribution
 * on [0, 1] and if X has a cumulative distribution F_X, then
 * the cumulative distribution function of the random variable
 * F_X^{-1}(Y) is F_X .
 * REF:  http://en.wikipedia.org/wiki/Inverse_transform_sampling
 */

double hg_uniform_radial_coordinate(const double radius) {
  // pag.4, equation (7)
  // distribution: rho(r) = [ sinh(r) / (cosh(radius)-1) ]
  double r = 0; 
  if(radius == 0) {
    hg_enduser_warning("Radius = 0.");
    return r;
  }
  // uniformly extracted variable (y)
  double y = HG_Random::get_random_01_value();;
  /* CDF of rho(r) is:
   * [(cosh(r)-1) / (cosh(radius)-1)]
   * then: 
   * r = arcosh[ 1 + y * [cosh(radius)-1] ] */
  r = acosh(1.0 + y * (cosh(radius) - 1.0));
  return r;
}


double hg_quasi_uniform_radial_coordinate(const double radius, const double alpha) {
  // pag.6, equation (17)
  // distribution:  rho(r) = alpha * { [alpha * sinh(r)] / cosh(alpha *radius-1) }
  double r = 0; 
  if(radius == 0 or alpha == 0) {
    hg_enduser_warning("Radius = 0 or alpha = 0: discontinuity.");
    return r; // alpha = 0 -> limit does not exist (discontinuity)
  }
  // uniformly extracted variable (y)
  double y = HG_Random::get_random_01_value();
  /* CDF of rho(r) is:
   * [(cosh(alpha *r)-1) / (alpha * cosh(alpha*radius)-1)]
   * then: 
   * r = 1/alpha * arcosh[ 1 + alpha * y * [cosh(alpha*radius)-1] ] */
  r = 1.0/alpha * acosh(1.0 +  y * (cosh(alpha * radius) - 1.0));
  return r;
}


double hg_uniform_angular_coordinate() {
  // pag. 4 - subsection A (in text)
  double theta;
  // [0,1] -> [0,2pi]
  theta = HG_Random::get_random_01_value() * 2.0 * HG_PI;
  return theta;
}

