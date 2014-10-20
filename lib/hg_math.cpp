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


#include <stdlib.h>
#include <limits>
#include <gsl/gsl_math.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_sf_hyperg.h>

#include "hg_math.h"

static double rho(double alpha, double R, double r){
  return alpha * exp(alpha * (r-R));
}

static double hg_heaviside(double zeta, double R, 
			   double r1, double r2, double theta){
  double x = (1/zeta)
    * acosh(cosh(zeta*r1)*cosh(zeta*r2)
	    -sinh(zeta*r1)*sinh(zeta*r2)*cos(theta));
  if(x >= R)
    return 0.0;
  else
    return 1.0;
}

static double hg_fermi_dirac_std(double beta, double zeta, double R, 
				 double r1, double r2, double theta){
  double x = (1/zeta) 
    * acosh(cosh(zeta*r1)*cosh(zeta*r2)
	    -sinh(zeta*r1)*sinh(zeta*r2)*cos(theta));
  
  return 1 / (1 + exp(beta * zeta / 2 * (x-R)));
}

static double hg_fermi_dirac_scm(double eta, double R, 
				 double r1, double r2){  
  return 1 / (1 + exp((eta / 2.0) * (r1 + r2 - R)));
}


static double hg_integral_heaviside(double *x, size_t dim, void *p){

  hg_f_params * fp = (hg_f_params *)p;
  return 1/HG_PI
    * rho(fp->alpha, fp->R, x[0])
    * rho(fp->alpha, fp->R, x[1])
    * hg_heaviside(fp->zeta, fp->R, x[0], x[1], x[2]);
}

static double hg_integral_standard(double *x, size_t dim, void *p){

  hg_f_params * fp = (hg_f_params *)p;
  return 1/M_PI 
    * rho(fp->alpha, fp->R, x[0])
    * rho(fp->alpha, fp->R, x[1])
    * hg_fermi_dirac_std(fp->beta, fp->zeta, fp->R, x[0], x[1], x[2]);
}

static double hg_integral_scm(double *x, size_t dim, void *p){

  hg_f_params * fp = (hg_f_params *)p;
  return rho(fp->alpha, fp->R, x[0])
    * rho(fp->alpha, fp->R, x[1])
    * hg_fermi_dirac_scm(fp->eta, fp->R, x[0], x[1]);
}

double hg_get_R(hg_graph_t * graph, hg_algorithm_parameters_t & p){

  int n_dimensions;
  double res, err; // result and error
  double *xl, *xu; // integral's lower and upper bounds  
  
  // construct function
  gsl_monte_function F;
  size_t calls = 100000; // number of integral iterations
  hg_f_params params = {-1,-1,-1,-1,-1};

  // hyperbolic_rgg and hyperbolic_standard integrals are 3D
  // soft_configuration_model only 2 dimensions
  if((*graph)[boost::graph_bundle].type == SOFT_CONFIGURATION_MODEL){
    n_dimensions = 2;
    xl = new double[n_dimensions];
    xu = new double[n_dimensions];
    xl[0] = xl[1] = 0;

    F.f = &hg_integral_scm;
    params.R = 0;
    params.alpha = p.alpha;
    params.eta = p.eta;
  }
  else{
    n_dimensions = 3;
    xl = new double[n_dimensions];
    xu = new double[n_dimensions];
    xl[0] = xl[1] = xl[2] = 0;
    xu[2] = HG_PI;

    if((*graph)[boost::graph_bundle].type == HYPERBOLIC_STANDARD){
      F.f = &hg_integral_standard;
      params.R = 0;
      params.alpha = p.alpha;
      params.beta = 1.0 / (*graph)[boost::graph_bundle].temperature;
      params.zeta = (*graph)[boost::graph_bundle].zeta_eta;
    }
    else{
      F.f = &hg_integral_heaviside;
      params.R = 0;
      params.alpha = p.alpha;
      params.zeta = (*graph)[boost::graph_bundle].zeta_eta;
   
    }
  }


  F.dim = n_dimensions;
  F.params = &params;

  const gsl_rng_type *T;
  gsl_rng *r;
  gsl_rng_env_setup ();
  T = gsl_rng_default;
  r = gsl_rng_alloc (T);
  gsl_monte_miser_state *s = gsl_monte_miser_alloc(n_dimensions); 

  double n = (*graph)[boost::graph_bundle].expected_n;
  double k_bar = (*graph)[boost::graph_bundle].expected_degree;
  
  double eps = .01; // maximum error for the avg degree
  double low = 0.0;
  double high = std::max(50., (pow(log(n), 2)));
  if(params.beta < 1)
    high = std::max(50., (pow(log(n), 2.5)));
  double mid;
  const int max_iterations = 5000;
  int it = 0;
  do{

    // set midpoint
    mid = (high + low) / 2.0;
    xu[0] = xu[1] = mid;
    params.R = mid; // R = mid

    // integrate
    gsl_monte_miser_integrate (&F, xl, xu, n_dimensions, calls, r, s, &res, &err);

    if(isnan(res))
      {
	mid *= 1.00001;	
      }
    // bisection
    else{
      if(n*res < k_bar)
	high = mid;
      else
	low = mid;
    }
    ++it;
    //cout << it << " - " << n*res << " - " << params.R << endl;
  }while((abs(n*res-k_bar) > eps || isnan(res)) && it < max_iterations 
	 && high > std::numeric_limits< double >::min());

  if(isnan(res) || abs(n*res-k_bar) > eps 
     || high < std::numeric_limits< double >::min()){
    cerr << "Network cannot be generated. Try different parameters." << endl;
    exit(0);
  }


  gsl_monte_miser_free (s);
  delete[] xl;
  delete[] xu;

  return mid;
}

// Given that |z|>1, we need some transformations
inline double hypergeometric_f(double a, double b,
			       double c, double z){
  
  if(b == 1.0){
    return - log(1-z) / z;
  }
  else{

    double w = 1 / (1-z);
    
    // If b is an integer, the function diverges
    // We just add a small epsilon to make it work
    if(static_cast<int>(b) == b){
      double eps = .000001;
      b += eps;
    }

    double f = w * b / (b - 1.0) * gsl_sf_hyperg_2F1(1.0, 1.0, 2-b, w)
      + b * HG_PI * pow(1-w,-b) * pow(w, b) * pow(sin(b*HG_PI),-1);

    return f;
  }
}

double hg_get_lambda(hg_graph_t * graph, hg_algorithm_parameters_t & p){

  double beta = 1.0 / (*graph)[boost::graph_bundle].temperature;
  double n = (*graph)[boost::graph_bundle].expected_n;
  double k_bar = (*graph)[boost::graph_bundle].expected_degree;
  
  double res; // hypergeometric function result
  double eps = .001; // maximum error for the avg degree
  double low = 1;
  double high = DBL_MAX;
  double mid;

  do{
    // set midpoint
    mid = (high + low) / 2.0;

    res = hypergeometric_f(1.0, 1.0/beta, 1.0 + 1.0/beta, -mid);

    if(n*res < k_bar)
      high = mid;
    else
      low = mid;
  
    //cout << n*res << " - " << mid << endl;
  }while(abs(n*res-k_bar) > eps || isnan(res));
  
  return mid;
}
