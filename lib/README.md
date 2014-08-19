# Hyperbolic Graph Generator: hggraphs library


== Installation:

Requirements:
	- boost libraries 1.46.1 or higher
	- gsl libraries 1.16 or higher
	- libtool 

In order to install the Hyperbolic Graph Generator you need to follow
the list of instructions below:

	./configure
	make
	(sudo) make install

The procedure above builds and install the hggraphs library
(public interface is described in hg_graphs_lib.h). 
For more information, please see the Installation FAQs.


== Installation FAQs:


Q: Can I install the hggraphs library in my HOME directory?
A: Yes, all you need is to run configure using the --prefix option:
   	./configure --prefix=/home/user/my_hggraphs_lib

   Library files will be available in:
	/home/user/my_hggraphs_lib/include
	/home/user/my_hggraphs_lib/lib


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
   can I install the hggraphs library?
A: If the boost libraries are installed in a custom path, e.g. 
   they are in /home/user/myboost, then the configure command
   must be run with the CPPFLAGS set:

	./configure CPPFLAGS='-I/home/user/myboost'

   Boost are header-only libraries, then no LDFLAGS are required


Q: My gsl libraries are not installed in a standard path, how
   can I install the hggraphs library?
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
   can I install the hggraphs library?
A: A combination of the previous answers can be used:

   ./configure CPPFLAGS='-I/home/user/myboost -I/home/user/mygsl/include' LDFLAGS="-L/home/user/mygsl/lib"


Q: Can I use the hggraphs library to develop new tools?
A: Yes, the public interface is described in the hg_graphs_lib.h file
   that is installed in include/hg_graphs_lib.h. In order to link the 
   library to your tool you need to provide the -lhggraphs option as well
   as the path to the lib folder containing the library at linking time.
   e.g.
	hggraphs library installed in default path:

	g++ -I/usr/local/include -c my_new_tool.o my_new_tool.cpp
	g++ -o my_new_tool my_new_tool.o -L/usr/local/lib -lhggraphs

	hggraphs library installed in custom path:

	g++ -I/home/user/my_hggraphs_lib/include -c my_new_tool.o my_new_tool.cpp
	g++ -o my_new_tool my_new_tool.o -L/home/user/my_hggraphs_lib/lib -lhggraphs


