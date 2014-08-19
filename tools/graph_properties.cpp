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

#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <sstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>

#include "hg_graphs_lib.h"
#include "hg_debug.h"
#include "stats.h"

#include <boost/graph/clustering_coefficient.hpp>

// accumulators
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
/// using namespace boost::accumulators;

using namespace std;


void usage (string exe_name){
  cout << "NAME: " << endl;
  cout << "\t" << "graph_properties" << " - hyperbolic graphs properties computation " << endl;
  cout << endl;
  cout << "SYNOPSIS: " << endl;
  cout << "\t" << "graph_properties" << " [options] [args] " << endl;
  cout << endl;
  cout << "DESCRIPTION:" << endl;
  cout << "\t" << "The program computes the following graph properties:" << endl;
  cout << "\t" << "radial.txt contains pairs: <node id> <radial coordinate>" << endl;
  cout << "\t" << "angular.txt contains pairs: <node id> <angular coordinate>" << endl;
  cout << "\t" << "degree.txt contains pairs: <node id> <node degree>" << endl;
  cout << "\t" << "knn.txt contains pairs: <node id> <node avg. neigh. degree>" << endl;
  cout << "\t" << "cc.txt contains pairs: <node id> <node clustering coefficient>" << endl;
  cout << endl;
  cout << "OPTIONS:"<< endl;
  cout << "\t" << "-i" << "\t" << "graph file (in hg format)" << endl;
  cout << "\t" << "\t" << "default value is ./graph.hg" << endl;
  cout << "\t" << "-o" << "\t" << "output folder" << endl;
  cout << "\t" << "\t" << "default value is current folder" << endl;
  cout << "\t" << "-h" << "\t" << "print help menu" << endl; 
  cout << endl;
  return;
}



int main (int argc, char **argv) {
  // default values
  string graph_file = "./graph.hg";
  string output_folder = "./";

  char *cvalue = NULL;
  int index;
  int c;  
  bool verbose = true;
  opterr = 0;

  hg_debug("parsing options");
 
  while ((c = getopt (argc, argv, "i:o:qh")) != -1) {
    switch (c) {
    case 'i':
      graph_file = optarg;
      break;
    case 'o':
      output_folder = optarg;
      break;
    case 'h':
      usage(argv[0]);
      return 0;
    case 'q':
      verbose = false;
      break;
    case '?':
      cout << "Unknown option provided" << endl << endl;
      usage(argv[0]);
      return 1;
    default:
      cout << "Unknown option provided" << endl << endl;
      usage(argv[0]);     
      return 1;
    }
  }

  hg_debug("reading hg graph");
  if(verbose) {
    cout << "Loading graph " << graph_file << endl;
  }
  hg_graph_t *graph = NULL;
  graph = hg_read_graph(graph_file);
  if(graph == NULL) {
    hg_enduser_warning("Empty topology - %s \n\t  Quitting.", graph_file.c_str());
    return 1;
  }
  if(verbose) {
    cout << "Graph loaded" << endl;
  }

  stringstream s;
  s << "mkdir -p " << output_folder;
  system(s.str().c_str());

  int n = num_vertices(*graph);
  double k, knn, cc;
  double r, theta;

  // maps
  map<int,double> degree_map;
  map<int,double> avg_neigh_degree;
  map<int,double> clust_coeff;
  map<int,double> radial;
  map<int,double> angular;
  
  // accumulators
  boost::accumulators::accumulator_set<double, boost::accumulators::stats<  boost::accumulators::tag::mean, boost::accumulators::tag::moment<2> > > ks;
  boost::accumulators::accumulator_set<double, boost::accumulators::stats<  boost::accumulators::tag::mean, boost::accumulators::tag::moment<2> > > knns;
  boost::accumulators::accumulator_set<double, boost::accumulators::stats<  boost::accumulators::tag::mean, boost::accumulators::tag::moment<2> > > ccs;

  // properties computation
  hg_debug("computing properties");

  if(verbose) {
    cout << "Graph properties computation" << endl;
  }

  hg_graph_t::vertex_iterator vertexIt, vertexEnd;
  hg_graph_t::adjacency_iterator neighbourIt, neighbourEnd;
  tie(vertexIt, vertexEnd) = vertices(*graph);
  for (; vertexIt != vertexEnd; ++vertexIt) { 
    // degree
    k =  degree(*vertexIt,*graph);
    if(k == 0) continue; // we do not consider 0-degree nodes
    // knn
    knn = 0;
    tie(neighbourIt, neighbourEnd) = adjacent_vertices(*vertexIt, *graph); 
    for (; neighbourIt != neighbourEnd; ++neighbourIt){ 
      knn += degree(*neighbourIt,*graph);
    }
    if(k!=0){
      knn = knn /k;
    }
    // clustering coefficient 
    cc = clustering_coefficient(*graph,*vertexIt);
    // putting data in maps and accumulators
    degree_map.insert(make_pair(*vertexIt,k));
    avg_neigh_degree.insert(make_pair(*vertexIt,knn));
    clust_coeff.insert(make_pair(*vertexIt,cc));
    ks(k);
    knns(knn);
    ccs(cc);
    // radial and angular coordinates
    radial.insert(make_pair(*vertexIt, (*graph)[*vertexIt].r));
    angular.insert(make_pair(*vertexIt, (*graph)[*vertexIt].theta));
    hg_debug("%lu: k %f \t knn %f \t cc %f \t (%f,%f)", *vertexIt, k, knn, cc, (*graph)[*vertexIt].r, (*graph)[*vertexIt].theta);
  }

  if(verbose) {
    cout << "Properties computed" << endl;
  }

  // printing all the id - value maps
  hg_debug("printing properties in %s", output_folder.c_str());
  print(output_folder + "/degree.txt", degree_map);
  print(output_folder + "/knn.txt", avg_neigh_degree);
  print(output_folder + "/cc.txt", clust_coeff);
  print(output_folder + "/radial.txt", radial);
  print(output_folder + "/angular.txt", angular);
  if(verbose) {
    cout << "Files written in " << output_folder << ":" << endl;
    cout << "\t" << "degree.txt "<< endl;
    cout << "\t" << "knn.txt"<< endl;
    cout << "\t" << "cc.txt"<< endl;
    cout << "\t" << "radial.txt"<< endl;
    cout << "\t" << "angular.txt"<< endl;
    cout << endl;
    cout << "Average values (std deviation): " << endl;
    std::cout.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed
    std::cout.precision(3);
    cout << "\t" << "degree:\t\t" << boost::accumulators::mean(ks);
    cout<< " (" << sqrt(boost::accumulators::moment<2>(ks)) << ")"<< endl;
    cout << "\t" << "knn:\t\t" << boost::accumulators::mean(knns);
    cout << " (" << sqrt(boost::accumulators::moment<2>(knns)) << ")"<< endl;
    cout << "\t" << "clustering:\t" << boost::accumulators::mean(ccs);
    cout << " (" << sqrt(boost::accumulators::moment<2>(ccs)) << ")"<< endl;
    cout << endl;
  }
  
  // destroy graph
  delete graph;


  return 0;
}

