//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <boost/config.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>


enum family
{ Jeanie, Debbie, Rick, John, Amanda, Margaret, Benjamin, N };

int
main(int argc, char* argv[] )
{
  using namespace boost;
  std::string usage = "USAGE: ./SDG <insert|delete|list> <size> <rounds>";

  if ( argc < 3 ){
   std::cout << usage << std::endl;
   return -1;
  }

  char* cmd = argv[1];
  long long size = atoll(argv[2]);



  const char *name[] = { "Jeanie", "Debbie", "Rick", "John", "Amanda",
    "Margaret", "Benjamin"
  };

  adjacency_list <> g(N);

  int rounds = 1; // run 1 time by default
  if ( argc > 3) {
    rounds = atoll(argv[3]);
  }

  std::cout << cmd << " with size " << size << " repeat " << rounds << " rounds" << std::endl;

int i;
for (i=0;i<rounds;++i){
  
  if ( strcmp(cmd, "insert") == 0 ) {
    for (long long i=0;i<size;++i){
       add_edge(family(i%7), family((i+1)%7), g);
    }
  }
  else if ( strcmp( cmd, "delete" ) == 0 ){
    for (long long i=0;i<size;++i){
       add_edge(family(i%7), family((i+1)%7), g);
    }
    for (long long i=0;i<size;++i){
       remove_edge(family(i%7), family((i+1)%7), g);
    }
  }
  else if ( strcmp( cmd, "list" ) == 0 ){
    for (long long i=0;i<size;++i){
       add_edge(family(i%7), family((i+1)%7), g);
    }

    graph_traits < adjacency_list <> >::vertex_iterator i, end;
    graph_traits < adjacency_list <> >::adjacency_iterator ai, a_end;
    property_map < adjacency_list <>, vertex_index_t >::type
      index_map = get(vertex_index, g);
  
    for (boost::tie(i, end) = vertices(g); i != end; ++i) {
      std::cout << name[get(index_map, *i)];
      boost::tie(ai, a_end) = adjacent_vertices(*i, g);
      if (ai == a_end)
        std::cout << " has no children";
      else
        std::cout << " is the parent of ";
      for (; ai != a_end; ++ai) {
        std::cout << name[get(index_map, *ai)];
        if (boost::next(ai) != a_end)
          std::cout << ", ";
      }
      std::cout << std::endl;
    }

  }
  else {
    std::cout << usage << std::endl;
    return -1;
  }

  std::cout << size << " items returned in round " << i << "." << std::endl;

}
  
  return EXIT_SUCCESS;
}
