# Hyperbolic-Graph-Generator


A set of tools to generate synthetic graphs embedded into a hyperbolic space and to test the greedy routing.


## Installation:


Requirements:
- *boost libraries 1.46.1* or higher
- *gsl libraries 1.16* or higher
- *libtool* 

In order to install the Hyperbolic Graph Generator you need to follow the list of instructions below:
```sh
$ ./configure
$ make
$ (sudo) make install
```
The procedure above builds three tools:
- *hyperbolic_graph_generator*
- *graph_properties*
- *greedy_routing*

also, it installs the *hggraphs library* (public interface is described in *hg_graphs_lib.h*). 
For more information about the installation process, please see the Installation FAQs.

## Description:

### hyperbolic_graph_generator

A tool to generate synthetic graphs embedded into a hyperbolic space.


NAME: 
      hyperbolic_graph_generator - hyperbolic graphs generator 

SYNOPSIS: 
	  hg_graph_generator [options] [args] 

DESCRIPTION:
	The program generates random hyperbolic graphs according to
	http://dx.doi.org/10.1103/PhysRevE.82.036106
	A briefer description of how the hyperbolic graph generator
	code works can be found in Hyperbolic_Graph_Generation.pdf

OPTIONS:
	-n	graph size (number of nodes)
		default value is 1000
	-k	expected average degree
		default value is 10
	-g	expected power-law exponent gamma
		default value is 2 (infinite gamma = 10)
	-t	temperature
		default value is 0 (infinite temperature = 10)
	-z	square root of curvature zeta=sqrt(-K)
		default value is 1
	-s	random seed
		default value is 1
	-o	output folder
		default value is current folder
	-h	print help menu
	-v	verbose mode on


OUTPUT FORMAT:
	
	The program outputs a graph using the following order:
	the first line describes the main graph parameters:
		N <num nodes> T <tempearature> G <gamma> K <avg.degree> Z <zeta> S <seed> I <initial_node_id>
	the following <num nodes> lines associate a polar coordinate
	to each node of the graph:
		node_id		radial coordinate	angular coordinate
	Finally, the links are listed
		node_id  node_id

	Example:
	N	100	T	0	G	2	K	10	Z	2	S	1    I  1
	1	0.000301382	0.826476
	2	0.093611	2.88178
	3	0.0786131	1.37576
	... 
	99	0.10503	3.97561
	100	0.0713906	5.18173
	1	2
	1	3
	1	4
	...
	97	98
	98	99


=== graph_properties


A tool to compute some basic graph properties of the generated hyperbolic graphs

NAME: 
      graph_properties - hyperbolic graphs properties computation 

SYNOPSIS: 
	  graph_properties [options] [args] 

DESCRIPTION:
	The program computes the following graph properties:
	radial.txt contains pairs: <node id> <radial coordinate>
	angular.txt contains pairs: <node id> <angular coordinate>
	degree.txt contains pairs: <node id> <node degree>
	knn.txt contains pairs: <node id> <node avg. neigh. degree>
	cc.txt contains pairs: <node id> <node clustering coefficient>

OPTIONS:
	-i	graph file (in hg format)
		default value is ./graph.hg
	-o	output folder
		default value is current folder
	-h	print help menu



=== greedy_routing


NAME: 
	greedy_routing - greedy routing success ratio computation 

SYNOPSIS: 
	greedy_routing [options] [args] 

DESCRIPTION:
	Greedy routing performace test: it computes the greedy routing
	success ratio taking into account [a] (default 10000) random pairs
	belonging to the same connected component.

OPTIONS:
	-i	input graph file (in hg format)
		default value is ./graph.hg
	-a	number of attempts
		default value is 10000
	-s	random seed
		default value is 1
	-h	print help menu




== Installation FAQs:

Q: Where are the executables?
A: By default, `make install' installs the package's commands under
   `/usr/local/bin', include files under `/usr/local/include', etc.
   You can specify an installation prefix other than `/usr/local' by
   giving `configure' the option `--prefix=PREFIX', where PREFIX must
   be an absolute file name.


Q: Can I install the Hyperbolic Graph Generator in my HOME directory?
A: Yes, all you need is to run configure using the --prefix option:
   	./configure --prefix=/home/user/my_hg_generator

   Tools will be available in:
	/home/user/my_hg_generator/bin
   Library files will be available in:
	/home/user/my_hg_generator/include
	/home/user/my_hg_generator/lib


Q: boost libraries not found
A: if boost libraries are not installed, then
   install them using your package management system
   or:
   1) download them at http://www.boost.org/
   2) extract boost_**.tar.bz2 in a convenient location e.g. /home/user/
   Then run again the configure command in the Hyperbolic Graph Generator
   as follows:

        ./configure CPPFLAGS='-I/home/user/myboost'

   Note: boost are header-only libraries, no installation
	 process is required


Q: gsl libraries not found
A: if gsl libraries are not installed, then
   install them using your package management system
   or follow the instructions at:
	http://www.gnu.org/software/gsl/
   if gsl libraries are installed in custom location 
   then read Q "My gsl libraries are not installed in
   a standard path ..."


Q: My boost libraries are not installed in a standard path, how
   can I install the Hyperbolic Graph Generator?
A: If the boost libraries are installed in a custom path, e.g. 
   they are in /home/user/myboost, then the configure command
   must be run with the CPPFLAGS set:

	./configure CPPFLAGS='-I/home/user/myboost'

   Boost are header-only libraries, then no LDFLAGS are required


Q: My gsl libraries are not installed in a standard path, how
   can I install the Hyperbolic Graph Generator?
A: If the gsl libraries are installed in a custom path, e.g. 
   they are in /home/user/mygsl, then the configure command
   must be run with both the CPPFLAGS and LDFLAGS set:

	./configure CPPFLAGS='-I/home/user/mygsl/include' LDFLAGS="-L/home/user/mygsl/lib"
   
   If gsl are installed, the following command return the information
   be put in the CPPFLAGS and LDFLAGS:
   $ gsl-config --cflags --libs
     -I/opt/local/include
     -L/opt/local/lib -lgsl -lgslcblas


Q: Both boost and gsl libraries are installed in non standard paths, how
   can I install the Hyperbolic Graph Generator?
A: A combination of the previous answers can be used:

   ./configure CPPFLAGS='-I/home/user/myboost -I/home/user/mygsl/include' LDFLAGS="-L/home/user/mygsl/lib"



Q: Can I use the hggraphs library to develop new tools?
A: Yes, the public interface is described in the hg_graphs_lib.h file
   that is installed in include/hg_graphs_lib.h. In order to link the 
   library to your tool you need to provide the -lhggraphs option as well
   as the path to the lib folder containing the library at linking time.
   e.g.
	Hyperbolic Graph Generator installed in default path:

	g++ -I/usr/local/include -c my_new_tool.o my_new_tool.cpp
	g++ -o my_new_tool my_new_tool.o -L/usr/local/lib -lhggraphs

	Hyperbolic Graph Generator installed in custom path:

	g++ -I/home/user/my_hg_generator/include -c my_new_tool.o my_new_tool.cpp
	g++ -o my_new_tool my_new_tool.o -L/home/user/my_hg_generator/lib -lhggraphs


