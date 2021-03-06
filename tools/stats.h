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

#ifndef _STATS_H
#define _STATS_H

#include <iostream>
#include <fstream>
#include <map>

using namespace std;


map<double,double> pdf(const map<int,double>& id_property);
map<double,double> ccdf(const map<int,double>& id_property);

map<double,double> avg_over_degree(const map<int,double> & id_degree,
				   const map<int,double>& id_property);

void print(const string filename, const map<int,double>& id_property);
void print(const string filename, const map<double,double>& id_property);


#endif /* _STATS_H */
