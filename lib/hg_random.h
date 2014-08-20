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


#ifndef _HG_RANDOM_H
#define _HG_RANDOM_H

#include "boost/random.hpp" 
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_real.hpp>
#include <iostream> 


/* "Mersenne Twister: A 623-dimensionally equidistributed uniform pseudo-random
 * number generator", Makoto Matsumoto and Takuji Nishimura, 
 * ACM Transactions on Modeling and Computer Simulation: 
 * Special Issue on Uniform Random Number Generation, Vol. 8, No. 1, January 1998, pp. 3-30.
 *
 * http://www.boost.org/doc/libs/1_42_0/libs/random/random-generators.html#mt19937
 */
typedef boost::mt19937 HG_RandomGenerator_t;


class HG_Random {
private:
    static HG_RandomGenerator_t random_generator;
    static int seed;
public:
  static void init(const unsigned int & s);
  static double get_random_01_value();
};

#endif /* _HG_RANDOM_H */
