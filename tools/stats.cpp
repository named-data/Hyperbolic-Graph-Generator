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

#include "stats.h"


map<double,int> frequencies(const map<int,double>& id_property) {

    map<double,int> frequencies_map;
    map<int,double> :: const_iterator it;
    map<double,int> :: iterator f;
    for(it = id_property.begin(); it != id_property.end(); it++) {
      f = frequencies_map.find(it->second);
      if(f == frequencies_map.end()) {
	frequencies_map.insert(make_pair(it->second, 1));
      }
      else{
	f->second++;
      }
    }
    return frequencies_map;
}


map<double,double> pdf(const map<int,double>& id_property) {

  map<double,double> pdf;  
  int instances = id_property.size();
  if(instances != 0) {
    map<double,int> frequencies_map = frequencies(id_property);
    map<double,int> :: const_iterator it;
    for(it = frequencies_map.begin(); it != frequencies_map.end(); it++) {
      pdf.insert(make_pair(it->first, (double)it->second/(double)instances));
    }
  }
  return pdf;
}


map<double,double> ccdf(const map<int,double>& id_property) {

  map<double,double> ccdf_map;  
  map<double,double> :: iterator c;  
  int instances = id_property.size();
  int ccdf_val = instances;
  if(instances != 0) {
    map<double,int> frequencies_map = frequencies(id_property);
    map<double,int> :: const_iterator it;
    for(it = frequencies_map.begin(); it != frequencies_map.end(); it++) {
      ccdf_val = ccdf_val - it->second;
      ccdf_map.insert(make_pair(it->first, (double)ccdf_val));
    }
    for(c = ccdf_map.begin(); c!=ccdf_map.end(); c++) {
      c->second = (double) c->second / (double) instances;
    }
  }
  return ccdf_map;
}



map<double,double> avg_over_degree(const map<int,double> & id_degree,
				   const map<int,double>& id_property) {
  map<double,double> res;
  map<double,int> frequencies_map = frequencies(id_degree);
  map<int,double> :: const_iterator it;
  map<int,double> :: const_iterator p;
  map<double,double> :: iterator r;
  for(it = id_degree.begin(); it != id_degree.end(); it++) {
    p = id_property.find(it->first);
    if( p != id_property.end()) {
      r = res.find(it->second);
      if(r == res.end()) {
	// insert degree - other property
	res.insert(make_pair(it->second, p->second));
      }
      else {
	// add other property
	r->second += p->second;
      }
    }
  }
  // then normalize res
  map<double,int> :: const_iterator f;
  for(r = res.begin(); r != res.end(); r++) {
    f = frequencies_map.find(r->first);
    if(f != frequencies_map.end()) {
      r->second = r->second / (double)f->second;
    }
  }
  return res;
}


void print(const string filename, const map<int,double>& id_property) {
  ofstream output_stream;
  // opening file
  output_stream.open(filename.c_str(), ios::out);
  // FILE IS OPEN CORRECTLY
  if (output_stream.is_open() && !output_stream.fail()) { 
    // printing content
    map<int,double> :: const_iterator it;
    for(it = id_property.begin(); it!= id_property.end(); it++) {
      output_stream << it->first << "\t" << it->second << endl;
    }
  }
  // SOME PROBLEM AROSE
  else{
    cerr << "ERROR opening " << filename << "!" << endl;		
  }
  // closing file
  output_stream.close();
}


void print(const string filename, const map<double,double>& id_property) {
  ofstream output_stream;
  // opening file
  output_stream.open(filename.c_str(), ios::out);
  // FILE IS OPEN CORRECTLY
  if (output_stream.is_open() && !output_stream.fail()) { 
    // printing content
    map<double,double> :: const_iterator it;
    for(it = id_property.begin(); it!= id_property.end(); it++) {
      output_stream << it->first << "\t" << it->second << endl;
    }
  }
  // SOME PROBLEM AROSE
  else{
    cerr << "ERROR opening " << filename << "!" << endl;		
  }
  // closing file
  output_stream.close();
}

