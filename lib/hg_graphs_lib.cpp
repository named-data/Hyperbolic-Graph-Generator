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


#include "hg_graphs_lib.h"
#include "hg_gen_algorithms.h"
#include "hg_utils.h"
#include "hg_random.h"
#include "hg_debug.h"
#include <iomanip> 



hg_graph_t * hg_read_graph(const string filename) {
  // graph_t pointer
  hg_graph_t *g = NULL;
  // graph generation parameters
  string par; // name of parameter
  // node id
  int i;
  int node1,node2;
  // coordinates
  double radial;
  double angular;
  // opening file
  ifstream file;
  int expected_n;
  unsigned int starting_id;
  file.open(filename.c_str(), ios::in);
  if(!file.fail() and file.is_open()) {
    // reading first line
    file >> par >> expected_n;
    g = new hg_graph_t(expected_n);
    (*g)[boost::graph_bundle].expected_n = expected_n;
    file >> par >> (*g)[boost::graph_bundle].temperature;
    file >> par >> (*g)[boost::graph_bundle].expected_gamma;
    file >> par >> (*g)[boost::graph_bundle].expected_degree;
    file >> par >> (*g)[boost::graph_bundle].zeta_eta;
    file >> par >> (*g)[boost::graph_bundle].seed;
    file >> par >> starting_id;
    // reading coordinates
    for(i = 0; i < expected_n; i++) {
      file >> node1 >> radial >> angular;
      (*g)[i].r = radial;
      (*g)[i].theta = angular;
    }
    // reading links
    while(file >> node1 >> node2) {
      add_edge(node1-starting_id, node2-starting_id, *g);
    }
  }
  else {
    // warning
    hg_log_err("File %s cannot be opened", filename.c_str());
    return NULL;
  }
  file.close();
  // infer graph type from parameters 
  (*g)[boost::graph_bundle].type  = hg_infer_hg_type(g);
  return g; 
}



void hg_print_graph(const hg_graph_t *g, const string filename, unsigned int starting_id) {

  if(g == NULL) {
    hg_log_err("Warning: empty data structure, no file written");
    return;
  }
  ofstream file;
 
  file.open(filename.c_str(), ios::out);
  if(!file.fail() and file.is_open()) {
    file << std::setprecision(5) << std::fixed;
    // hg_graph parameters
    file << "N" << "\t" << (*g)[boost::graph_bundle].expected_n << "\t";
    file << "T" << "\t" << (*g)[boost::graph_bundle].temperature << "\t";
    file << "G" << "\t" << (*g)[boost::graph_bundle].expected_gamma << "\t";
    file << "K" << "\t" << (*g)[boost::graph_bundle].expected_degree << "\t";
    if((*g)[boost::graph_bundle].temperature >= HG_INF_TEMPERATURE &&
       (*g)[boost::graph_bundle].expected_gamma < HG_INF_GAMMA) {
      file << "eta" << "\t" << (*g)[boost::graph_bundle].zeta_eta << "\t";
    }
    else {
      file << "Z" << "\t" << (*g)[boost::graph_bundle].zeta_eta << "\t";
    }
    file << "S" << "\t" << (*g)[boost::graph_bundle].seed << "\t";
    file << "I" << "\t" << starting_id << endl;
    // hg_graph vertex coordinates
    hg_graph_t::vertex_iterator vertexIt, vertexEnd;
    tie(vertexIt, vertexEnd) = vertices(*g);
    for (; vertexIt != vertexEnd; ++vertexIt) { 
      file << *vertexIt + starting_id << "\t";
      file << (*g)[*vertexIt].r << "\t";
      file << (*g)[*vertexIt].theta << endl;
    }
    // hg_graph edgelist
    hg_graph_t::edge_iterator edgeIt, edgeEnd;
    tie(edgeIt, edgeEnd) = edges(*g);
    for (; edgeIt != edgeEnd; ++edgeIt) { 
      file << source(*edgeIt, *g) + starting_id << "\t";
      file << target(*edgeIt, *g) + starting_id << endl;
    }
  }
  file.close();
  return;
}


void hg_init_random_generator(const unsigned int seed) {
  HG_Random::init(seed);
}


double hg_rand_01_wrapper() {
  // return =  ((double) rand() / (RAND_MAX));
  return  HG_Random::get_random_01_value();
}


hg_graph_t * hg_graph_generator(const int n, const double k_bar, 
				const double exp_gamma, const double t, 
				const double zeta, const int seed, 
				bool verbose) {
  hg_graph_type gt = hg_infer_hg_type(exp_gamma,t);
  hg_graph_t * graph = NULL;

  switch(gt){
  case HYPERBOLIC_RGG: 
    graph = hg_hyperbolic_rgg(n, k_bar, exp_gamma, zeta, seed, verbose);
    break;
  case HYPERBOLIC_STANDARD:
    graph = hg_hyperbolic_standard(n, k_bar, exp_gamma, t, zeta, seed, verbose);
    break;
  case SOFT_CONFIGURATION_MODEL:
    graph = hg_soft_configuration_model(n, k_bar, exp_gamma, zeta, seed, verbose);
    break ;
  case ANGULAR_RGG:
    graph = hg_angular_rgg(n, k_bar, zeta, seed, verbose);
    break;
  case SOFT_RGG:
    graph = hg_soft_rgg(n, k_bar, t, zeta, seed, verbose);
    break;
  case ERDOS_RENYI:
    graph = hg_erdos_renyi(n, k_bar, zeta, seed, verbose);
    break;
  default:
    // case not implemented
    hg_log_err("Case not implemented yet");
  }
  return graph;
}
