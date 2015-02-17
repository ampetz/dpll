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
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/graph_utility.hpp>
#include <stdlib.h>
#include <cstdlib>

#include "global.hh"

using namespace std;
using namespace boost;
enum family
{ Jeanie, Debbie, Rick, John, Amanda, Margaret, Benjamin, N };



void initializeStruct (cnfFormula inFormula){
  
  int numVars = numUniqueVars(inFormula);
  cout << numVars << endl;
  globalStruct.inputFormula = inFormula;
  globalStruct.assignments = vector<bool>(numVars);
  globalStruct.assigned = vector<bool>(numVars, false);
  globalStruct.decision = vector<vector<int>>(numVars);
  globalStruct.decisionLevel = 0;
  globalStruct.g = boost::adjacency_list <>(numVars + 1);

}

void printGlobal (){

  vector<bool>::iterator it;
  vector< vector<int> >::iterator outerit;
  vector<int>::iterator innerit;
  vector<bool> a = globalStruct.assignments;
  vector<bool> b = globalStruct.assigned;
  vector<vector<int>> c = globalStruct.decision;
  

  
  cout << "Decisions: " << endl;

  for(outerit = c.begin(); outerit != c.end(); ++outerit) { 
    int outerIndex = outerit - c.begin();
    vector<int> level = (*outerit);

    cout << "Level " << outerIndex << ": ";

    for(innerit = level.begin(); innerit != level.end(); ++innerit) { 
      int innerIndex = innerit - level.begin();

      cout << (*innerit) << ", ";
    } // inner for



    cout << endl;


  } // outer for

  cout << "Assignments: " << endl;

  for(it = a.begin(); it != a.end(); ++it) { 
    int index = it - a.begin();
   
    cout << index << ": " << (*it) << endl;

  }

  cout << "Assigned: " << endl;

  for(it = b.begin(); it != b.end(); ++it) { 
    int index = it - b.begin();
   
    cout << index << ": " << (*it) << endl;

  }

}
  
  

int main(){


  lit lit0 = {0, true};
  lit lit0a = {0, false};
  lit lit1 = {1, false};
  lit lit2 = {2, true};

  cnfFormula testF = {{lit0}, {lit0a, lit1} };


  initializeStruct(testF);
  bool val = bcp ();

  bool decVal = decide();
  
  cout << endl << "BCP result: " << val << endl;
  cout << endl << "Decide result: " << decVal << endl;

  printGlobal();

  print_graph(globalStruct.g, "01234");
  
  
  
  /*
  adjacency_list <> h(2);
  add_edge(3, 0, h);
  add_edge(0,1,h);
  add_edge(1,2,h);
  clear_vertex(0, h);


  graph_traits < adjacency_list <> >::vertex_iterator i, end;
  //graph_traits < adjacency_list <> >::adjacency_iterator ai, a_end;
  property_map < adjacency_list <>, vertex_index_t >::type
    index_map = get(vertex_index, h);

  vector<int> vertVals;
  for (boost::tie(i, end) = vertices(h); i != end; ++i) {
    vertVals.push_back(get(index_map, *i));
  }

  /*for(vector<int>::iterator it = vertVals.begin(); it != vertVals.end(); ++it)
    cout << (*it) << "," << endl; */

  //print_graph(h, "012"); */
  cout << endl << endl;






  /*
  using namespace boost;
  const char *name[] = { "Jeanie", "Debbie", "Rick", "John", "Amanda",
    "Margaret", "Benjamin"
  };

  adjacency_list <> g(N);
  //add_vertex(g);
  //std::pair<g.edge_descriptor, bool> result;
  //result = add_edge(Jeanie, Debbie, g);
  add_edge(Jeanie, Debbie, g);
  add_edge(Jeanie, Jeanie, g);
  add_edge(6,3,g);
  add_edge(Jeanie, Rick, g);
  add_edge(Jeanie, John, g);
  add_edge(Debbie, Amanda, g);
  add_edge(Rick, Margaret, g);
  add_edge(John, Benjamin, g); 
  //add_edge(-1, Amanda, g);

  print_graph(g, name);

    //std:: cout << endl << endl << result << endl << endl;

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
  return EXIT_SUCCESS; */
  
}



/*   
graph print extras


int *names = &vertVals[0];
  string varString;
  char ch;
  char *chNames;

  for (int i = 0; i < vertVals.size(); i++) {

    ch = names[i];

    varString += itoa(ch, chNames, 10); //ch;

  }

  

  


  for(int i = 0; i < vertVals.size(); i++)
    cout << varString << names[i] << ", " << endl;

*/


