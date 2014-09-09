## Hyperbolic-Graph-Generator


The Hyperbolic-Graph-Generator is a package of tools to generate
graphs embedded in the hyperbolic plane, and to compute the efficiency
of greedy forwarding in these graphs.

This standalone package is also a core component of the hyperbolic
routing package that is going to be integrated into the NDNSim (http://ndnsim.net).
Hyperbolic routing is greedy geometric routing in a
hyperbolically embedded network. Hyperbolic routing has attractive
scalability and efficiency characteristics. In particular, applied to
the AS Internet, hyperbolic routing is nearly optimal, i.e., maximally
scalable (http://dx.doi.org/10.1038/ncomms1063/). The NDN testbed has
been recently mapped to the hyperbolic plane yielding similar routing
efficiency (http://www.caida.org/research/routing/greedy_forwarding_ndn/).
Because of its exceptional scalability and efficiency characteristics,
hyperbolic routing is one of the routing strategies in the NDN
architecture.


### Installation:

Requirements:
- *boost libraries 1.46.1* or higher
- *gsl libraries 1.16* or higher
- *libtool* 

In order to install the Hyperbolic Graph Generator you need to follow the list of instructions below:
```sh
$ sh ./bootstrap.sh  # non-release versions
$ ./configure
$ make
$ (sudo) make install
```
The procedure above builds three tools:
- *hyperbolic_graph_generator*
- *graph_properties*
- *greedy_routing*


Also, it installs the *hggraphs library* (public interface is described in *hg_graphs_lib.h*). 
For more information about the installation process, please see the Installation FAQs.

### Description:

#### **hyperbolic_graph_generator**

A tool to generate synthetic graphs embedded into a hyperbolic space.

```
NAME: 
      hyperbolic_graph_generator - hyperbolic graphs generator 

SYNOPSIS: 
	  hg_graph_generator [options] [args] 

DESCRIPTION:
	The program generates random hyperbolic graphs according to
	http://dx.doi.org/10.1103/PhysRevE.82.036106
	A brief description of how the hyperbolic graph generator
	code works can be found in Hyperbolic_Graph_Generation.pdf.

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
	    or ratio eta=zeta/T in Soft Configuration Model
		default value is 1
	-s	random seed
		default value is 1
	-o	output folder
		default value is current folder
	-f	graph file name
		default value is graph (.hg extension is always added)
	-h	print help menu
	-q	quiet (no output)

OUTPUT FORMAT:
	The program outputs a graph using the following format.
	The first line describes the main graph parameters:
	N <num nodes> T <tempearature> G <gamma> K <avg.degree> Z <zeta> S <seed> I <initial_node_id>
	The following <num nodes> lines associate a polar coordinate
	to each node of the graph:
		node_id		radial coordinate	angular coordinate
	Finally, the links are listed:
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
```


#### **graph_properties**
A tool to compute some basic graph properties of the generated hyperbolic graphs.

```
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
    -q	quiet (no output)
```


### greedy_routing
A tool to test the greedy routing on hyperbolic graphs.

```
NAME: 
	greedy_routing - greedy routing success ratio computation. 

SYNOPSIS: 
	greedy_routing [options] [args] 

DESCRIPTION:
	Greedy routing performance test: it computes the greedy routing
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
```

###  Installation FAQs:

**Where are the executables?**
By default, `make install` installs the package's commands under  `/usr/local/bin`, copies the header files under `/usr/local/include`, while the compiled libraries are available at `/usr/local/lib`. 
You can specify an installation prefix other than `/usr/local` by giving `configure` the option `--prefix=PREFIX`, where PREFIX must be an absolute path.


**Can I install the Hyperbolic Graph Generator in my HOME directory?**
Yes, all you need is to run `configure` using the `--prefix` option:
	```
   	./configure --prefix=/home/user/my_hg_generator
	```

Tools will be available in:

	`/home/user/my_hg_generator/bin`

Library files will be available in:

	`/home/user/my_hg_generator/include`

	`/home/user/my_hg_generator/lib`


**The boost libraries are not found, how can I fix this problem?**
If the boost libraries are not installed, then install them using your package management system, or:
   1) download them at http://www.boost.org/
   2) extract boost_**.tar.bz2 in a convenient location e.g. /home/user/
   Then run again the configure command in the Hyperbolic Graph Generator
   as follows:

        ./configure CPPFLAGS='-I/home/user/myboost'

   Note: boost are header-only libraries, no installation process is required


**The gsl libraries are not found, how can I fix this problem**
If the gsl libraries are not installed, then install them using your package management system or follow the instructions at http://www.gnu.org/software/gsl/.


**My boost libraries are not installed in a standard path, how can I build the Hyperbolic Graph Generator?**
If the boost libraries are installed in a custom path, e.g. they are in `/home/user/myboost`, then the configure command must be run with the CPPFLAGS set:

	./configure CPPFLAGS='-I/home/user/myboost'

Boost are header-only libraries, then no LDFLAGS are required.


**My gsl libraries are not installed in a standard path, how can I install the Hyperbolic Graph Generator?**
If the gsl libraries are installed in a custom path, e.g.  they are in `/home/user/mygsl`, then the configure command must be run with both the CPPFLAGS and LDFLAGS set:

	./configure CPPFLAGS='-I/home/user/mygsl/include' LDFLAGS="-L/home/user/mygsl/lib"
   
If gsl are installed, the following command return the information to be put in the CPPFLAGS and LDFLAGS:
    ```
   $ gsl-config --cflags --libs
      -I/opt/local/include
      -L/opt/local/lib -lgsl -lgslcblas
    ```

**Both boost and gsl libraries are installed in non standard paths, how can I install the Hyperbolic Graph Generator?**
A combination of the previous answers can be used.

   ./configure CPPFLAGS='-I/home/user/myboost -I/home/user/mygsl/include' LDFLAGS="-L/home/user/mygsl/lib"



**Can I use the hggraphs library to develop new tools?**
The libhggraphs public interface is described in the hg_graphs_lib.h file that is installed in `include/hg_graphs_lib.h`. In order to link the library to your tool you need to provide the -lhggraphs option as well as the path to the lib folder containing the library at linking time.
Let's suppose that the Hyperbolic Graph Generator installed in the default path `/usr/local`, in order to build a new tool the following operations have to be performed:

	g++ -I/usr/local/include -c my_new_tool.o my_new_tool.cpp
	g++ -o my_new_tool my_new_tool.o -L/usr/local/lib -lhggraphs

