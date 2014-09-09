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

#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <sstream>

#include "hg_graphs_lib.h"
#include "hg_debug.h"

     
using namespace std;


void usage (string exe_name){
  cout << "NAME: " << endl;
  cout << "\t" << "hg_graph_generator" << " - hyperbolic graphs generator " << endl;
  cout << endl;
  cout << "SYNOPSIS: " << endl;
  cout << "\t" << "hg_graph_generator" << " [options] [args] " << endl;
  cout << endl;
  cout << "DESCRIPTION:" << endl;
  cout << "\t" << "generates random hyperbolic graphs according to" << endl;
  cout << "\t" << "http://dx.doi.org/10.1103/PhysRevE.82.036106" << endl;
  cout << "\t" << "A brief description of how the hyperbolic graph generator" << endl;
  cout << "\t" << "code works can be found in Hyperbolic_Graph_Generation.pdf" << endl;
  cout << endl;
  cout << "OPTIONS:"<< endl;
  cout << "\t" << "-n" << "\t" << "graph size (number of nodes)" << endl;
  cout << "\t" << "\t" << "default value is 1000" << endl;
  cout << "\t" << "-k" << "\t" << "expected average degree" << endl;
  cout << "\t" << "\t" << "default value is 10" << endl;
  cout << "\t" << "-g" << "\t" << "expected power-law exponent gamma" << endl;
  cout << "\t" << "\t" << "default value is 2 (infinite gamma = ";
  cout << HG_INF_GAMMA << ")" << endl;
  cout << "\t" << "-t" << "\t" << "temperature" << endl;
  cout << "\t" << "\t" << "default value is 0 (infinite temperature = ";
  cout << HG_INF_TEMPERATURE << ")" << endl;
  cout << "\t" << "-z" << "\t" << "square root of curvature zeta=sqrt(-K)" << endl;
  cout << "\t" << "\t" << "or ratio eta=zeta/T in Soft Configuration Model" << endl;
  cout << "\t" << "\t" << "default value is 1" << endl;
  cout << "\t" << "-s" << "\t" << "random seed" << endl;
  cout << "\t" << "\t" << "default value is 1" << endl;
  cout << "\t" << "-o" << "\t" << "output folder" << endl; 
  cout << "\t" << "\t" << "default value is current folder" << endl;
  cout << "\t" << "-f" << "\t" << "graph file name" << endl; 
  cout << "\t" << "\t" << "default value is graph (.hg extension is always added)" << endl;
  cout << "\t" << "-h" << "\t" << "print help menu" << endl; 
  cout << "\t" << "-q" << "\t" << "quiet (no output)" << endl; 
  cout << endl;
  return;
}


int main (int argc, char **argv) {

  /* HG Graph Generator: default parameters */
  int n = 1000;         // number of nodes in the graph
  double k_bar = 10;    // expected average degree
  double exp_gamma = 2; // expected gamma or gamma out
  double t = 0;         // temperature
  bool zeta_eta_provided = false; 
  double zeta_eta = 1;  // parameter associated with curvature
  int seed = 1;         // random seed
  string folder = ".";
  string graph_filename = "graph";
  const string graph_ext = ".hg";
  bool verbose = true; 

  char *cvalue = NULL;
  int index;
  int c;
  
  hg_debug("parsing option start");

  opterr = 0;
 
  while ((c = getopt (argc, argv, "n:k:g:t:z:s:o:f:hq")) != -1) {
    switch (c) {
    case 'n':
      n = atoi(optarg);
      break;
    case 'k':
      k_bar = atof(optarg);
      break;
    case 'g':
      exp_gamma = atof(optarg);
      break;
    case 't':
      t = atof(optarg);
      break;
    case 'z':
      zeta_eta = atof(optarg);
      zeta_eta_provided = true;
      break;
    case 's':
      seed = atoi(optarg);
      break;
    case 'o':
      folder = optarg;
      break;
    case 'f':
      graph_filename = optarg;
      break;
    case 'q':
      verbose = false;
      break;
    case 'h':
      usage(argv[0]);
      return 0;
    case '?':
      cout << "Unknown option provided" << endl << endl;
      usage(argv[0]);
      return 0;
    default:
      cout << "Unknown option provided" << endl << endl;
      usage(argv[0]);     
      return 1;
    }
  }

  // creating output folder
  folder = folder + "/";
  stringstream s;
  s << "mkdir -p " << folder;
  system((s.str()).c_str());
  
  //Limit cases that we do not take into account
  if(n < 3){
    hg_enduser_warning("Number of nodes must be n>=3. \n\t  Quitting.");   
    return 1;
  }
  if(k_bar < 1 or k_bar > n-1){
    hg_enduser_warning("Avg degree must be greater than 0 and less than n-1. \n\t  Quitting.");   
    return 1;
  }
  if(t<0){
    hg_enduser_warning("Temperature must be positive (t >= 0). \n\t  Quitting.");
    return 1;
  }  
  if(exp_gamma<2){
    hg_enduser_warning("Gamma must be greater or equal 2 (Gamma >= 2). \n\t  Quitting.");
    return 1;
  }  

  // Warnings 
  //if(zeta_provided and (t>=HG_INF_TEMPERATURE or exp_gamma>=HG_INF_GAMMA)) {
  if(zeta_eta_provided and exp_gamma>=HG_INF_GAMMA) {
    hg_enduser_warning("zeta or eta make sense only at finite values of gamma. \n\t  The provided value of zeta (or eta) will be ignored.");
    zeta_eta = 1;
  }
  if(seed < 1){
    hg_enduser_warning("Seed has to be greater than 0. \n\t  Assuming seed = 1.");
    seed = 1;
  }

  hg_debug("parsing options end");

  if(verbose) {
    cout << endl;
    cout << "Parameters:" << endl;
    cout << "\t" << "Number of nodes [n]:" << "\t\t\t" << n;
    if(n == 1000) { cout << " (default)";}
    cout << endl;
    cout << "\t" << "Expected average degree [k]:" << "\t\t" << k_bar;
    if(k_bar == 10) { cout << "   (default)";}
    cout << endl;
    cout << "\t" << "Expected power-law exponent [g]:" << "\t";
    if(exp_gamma >=HG_INF_GAMMA) {
      cout << "INF" << endl;
    }
    else {
      cout << exp_gamma;
      if(exp_gamma == 2) { cout << "    (default)";}
      cout << endl;
    }
    if(exp_gamma < HG_INF_GAMMA && t >=HG_INF_TEMPERATURE) {
      cout << "\t" << "Ratio zeta/T [eta]:" << "\t\t\t" << zeta_eta;
    }
    else {
      cout << "\t" << "Square root of curvature [z]:" << "\t\t" << zeta_eta;
    }
    if(zeta_eta == 1) { cout << "    (default)";}
    cout << endl;
    cout << "\t" << "Temperature [t]:" << "\t\t\t";
    if(t >=HG_INF_TEMPERATURE) {
      cout << "INF" << endl;
    }
    else {
      cout << t;
      if(t == 0) { cout << "    (default)";}
      cout << endl;
    }
    cout << "\t" << "Seed [s]:" << "\t\t\t\t" << seed;
    if(seed == 1) { cout << "    (default)";}
    cout << endl;
    cout << "\t" << "Output folder [o]:"<< "\t\t\t"  << folder;
    if(folder == "./") { cout << "   (default)";}
    cout << endl;
    cout << "\t" << "Graph file name [f]:"<< "\t\t\t"  << graph_filename << graph_ext;
    if(graph_filename == "graph") { cout << "   (default)";}
    cout << endl;
    cout << endl;
  }

  hg_debug("generating graph");

  hg_graph_t *graph = hg_graph_generator(n, k_bar, exp_gamma, t, zeta_eta, seed, verbose);

  hg_debug("printing graph");

  if(graph == NULL) {
    hg_enduser_warning( "No graph generated.");
    return 1;
  }

  // if the number of edges is 0, then no graph has been constructed
  if(num_edges(*graph) == 0) {
    hg_enduser_warning( "All nodes have zero degree (no edges in the graph)");
  }
  else{
    if(verbose) {
      cout << "Generated: " << "\t" << num_edges(*graph) << " links" << endl;
    }
    hg_print_graph(graph, folder + graph_filename + graph_ext);
    if(verbose) {
      cout << "Written: " << "\t" << folder << graph_filename << graph_ext << endl;
      cout << endl;
    }
  }

  // deallocating memory for hg_graph
  delete graph;

  hg_debug("graph generator end");

  return 0;
}
