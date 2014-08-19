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
#include <set>
#include <vector>
#include <sstream>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>

#include <boost/graph/connected_components.hpp>

#include "hg_graphs_lib.h"
#include "hg_debug.h"

using namespace std;


void usage (string exe_name){
  cout << "NAME: " << endl;
  cout << "\t" << "greedy_routing" << " - greedy routing success ratio computation " << endl;
  cout << endl;
  cout << "SYNOPSIS: " << endl;
  cout << "\t" << "greedy_routing" << " [options] [args] " << endl;
  cout << endl;
  cout << "DESCRIPTION:" << endl;
  cout << "\t" << "Greedy routing performace test: it computes the greedy routing" << endl;
  cout << "\t" << "success ratio taking into account [a] (default 10000) random pairs" << endl;
  cout << "\t" << "belonging to the same connected component." << endl;
  cout << endl;
  cout << "OPTIONS:"<< endl;
  cout << "\t" << "-i" << "\t" << "input graph file (in hg format)" << endl;
  cout << "\t" << "\t" << "default value is ./graph.hg" << endl;
  cout << "\t" << "-a" << "\t" << "number of attempts" << endl;
  cout << "\t" << "\t" << "default value is 10000" << endl;
  cout << "\t" << "-s" << "\t" << "random seed" << endl;
  cout << "\t" << "\t" << "default value is 1" << endl;
  cout << "\t" << "-h" << "\t" << "print help menu" << endl; 
  cout << endl;
  return;
}


#ifdef DEBUG_ON

string print_path(vector<int> path) {
  stringstream s;
  int i;
  for(i=0; i<path.size(); i++) {
    s << "\t" << path[i];
  }
  return s.str();
}

#endif



int main (int argc, char **argv) {

  // default values
  string graph_file = "./graph.hg";
  int seed = 1;
  int num_attempts = 10000;

  hg_debug("parsing options");

  char *cvalue = NULL;
  int index;
  int c;  
  opterr = 0;
 
  while ((c = getopt (argc, argv, "i:a:s:h")) != -1) {
    switch (c) {
    case 'i':
      graph_file = optarg;
      break;
    case 'a':
      num_attempts = atoi(optarg);
      break;
    case 's':
     seed = atoi(optarg);
      break;
    case 'h':
      usage(argv[0]);
     return 0;
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
  hg_graph_t *graph = NULL;
  graph = hg_read_graph(graph_file);
  if(graph == NULL) {
    hg_enduser_warning("Empty topology - %s \n\t  Quitting.", graph_file.c_str());
    return 1;
  }

  // computing the connected components of this graph
  std::vector<int> component(num_vertices(*graph));
  int num = connected_components(*graph, &component[0]);
  std::vector<int>::size_type i;
  hg_debug("Total number of components: %d", num);
    
  hg_init_random_generator(seed);

  hg_graph_t::adjacency_iterator neighbourIt, neighbourEnd;
  double distance = -1.0;
  vector<int> empty;
  // candidate_next map:
  // distance: list_of_nodes_at_same distance
  pair<double,vector<int> > candidate_next = make_pair(distance,empty);
  hg_coordinate_t cur_coo;
  hg_coordinate_t dst_coo;
  int candidate_index;

  int effective_attempts = 0;
  int success = 0;
  int a = 0;
  for(a = 0; a<num_attempts; a++) {   
    int src = floor(hg_rand_01_wrapper() *((*graph)[boost::graph_bundle].expected_n - 1.0));
    int dst = floor(hg_rand_01_wrapper() *((*graph)[boost::graph_bundle].expected_n - 1.0));
    // we do not consider isolated nodes
    if( degree(src,*graph) == 0 or degree(dst,*graph)==0) {
      continue;
    }
    // we do not consider nodes in different components
    if(component[src] != component[dst]) {
      continue;
    }
    effective_attempts++;
    dst_coo.r = (*graph)[dst].r;
    dst_coo.theta = (*graph)[dst].theta;
    hg_debug("\tAttempt %d: %d -> %d", effective_attempts, src, dst);
    // greedy routing @ work
    set<int> visited; // visited nodes
#ifdef DEBUG_ON
    vector<int> path; // nodes path (src - dst)
#endif
    int current = src;
    while(current != dst) {
      // add current node to visited/path
      visited.insert(current);
#ifdef DEBUG_ON
      path.push_back(current);
#endif
      // select next current node (i.e. closest to destination)
      candidate_next = make_pair(-1.0,empty);
      tie(neighbourIt, neighbourEnd) = adjacent_vertices(current, *graph); 
      int k = 0;
      for (; neighbourIt != neighbourEnd; ++neighbourIt){ 
	k++;
	cur_coo.r = (*graph)[*neighbourIt].r;
	cur_coo.theta = (*graph)[*neighbourIt].theta;
	distance = hg_hyperbolic_distance(graph, cur_coo, dst_coo);
	// hg_debug("\t\t\tNeighbor %lu @ %f", *neighbourIt, distance);
	if(candidate_next.first == -1 or candidate_next.first > distance) {
	  candidate_next = make_pair(distance,empty);
	  candidate_next.second.push_back(*neighbourIt);
	}
	else {
	  if(candidate_next.first == distance) {
	    candidate_next.second.push_back(*neighbourIt);
	  }
	}	
      }
      // now we have a vector of candidates equally distant from the dst
      // we extract a random one (this node has at least one neighbor)
      candidate_index = floor(hg_rand_01_wrapper() *(candidate_next.second.size()-1));
      current = candidate_next.second[candidate_index];
#ifdef DEBUG_ON
      hg_debug("\t\t Distance: %f, k: %d, nodes: %lu, choosen: %d ", candidate_next.first, k,
	    candidate_next.second.size(), current);
#endif
      // if the current node has been already visited then we register a fail
      if(visited.find(current) != visited.end()){
#ifdef DEBUG_ON
	hg_debug("\tFAIL (%lu) %s ", path.size(), (print_path(path)).c_str());
#endif
	break;
      }
    }
    
    if(current == dst) {
      success++;
#ifdef DEBUG_ON
      path.push_back(dst);
      hg_debug("\tSUCCESS (%lu) %s ", path.size(), (print_path(path)).c_str());
#endif
    }    
  }

  // greedy routing performance
  if(effective_attempts > 0) {
    hg_debug("Effective attempts: %d", effective_attempts);
    // cout << "Connected components: " << num << endl;
    // cout << "Success: " << success << endl; 
    cout << "Success rate: " << (double)success/(double)effective_attempts << endl; 
  }
  else {
    hg_enduser_warning("0 effective attempts");
  }
  delete graph;

  return 0;
}



