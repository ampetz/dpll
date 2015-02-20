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
  cout << "Num Vars:  " << numVars << endl;
  globalStruct.inputFormula = inFormula;
  globalStruct.assignments = vector<bool>(numVars);
  globalStruct.assigned = vector<bool>(numVars, false);
  globalStruct.decision = vector<vector<int>>(numVars);
  globalStruct.decisionLevel = 0;
  globalStruct.g = graphType(numVars + 1);

}
  

// true = sat, false = unsat
bool dpll (cnfFormula inForm){

    initializeStruct(inForm);
    bool bcpResult = bcp();
    if(bcpResult) return false;
    while(true){
      bool decideResult = decide();
      if(!decideResult) return true;

      bcpResult = bcp();

      
      if(bcpResult) cout << "Conflict" << endl;
      else cout << "Non-conflict" << endl;


      while(bcpResult){
	cout << "Trying to backtrack....." << endl;
	int backtrackLevel = analyzeConflict();
	if(backtrackLevel < 0) return false;
	bcpResult = backTrack(backtrackLevel);
      } // end inner while
    } // end outer while
} // end dpll
  

int main(){

  lit lit0 = {0, true};
  lit nlit0 = {0, false};
  lit lit1 = {1, true};
  lit nlit1 = {1, false};
  lit lit2 = {2, true};
  lit nlit2 = {2, false};

  lit lit3 = {3, true};
  lit nlit3 = {3, false};

  lit lit4 = {4, true};
  lit nlit4 = {4, false};

  lit lit5 = {5, true};
  lit nlit5 = {5, false};

  lit lit6 = {6, true};
  lit nlit6 = {6, false};

  cnfFormula trivial = {{lit0}};
  cnfFormula testSat = {{lit0}, {nlit0, lit1}};
  cnfFormula testSat2 = {{lit0},{nlit0,lit1}, {nlit1,lit2}};
  cnfFormula testSat3 = {{lit0},{nlit0,lit1}, {nlit1,lit2}, {nlit2, lit3}};
  cnfFormula testSat4 = {{lit0}, {lit1},{nlit0,nlit1,lit1}};
  cnfFormula testSat5 = {{lit0, lit1, nlit1, nlit2}, {lit2, nlit0}};
  cnfFormula testSat6 = {{lit0}, {lit1, nlit0}};
  cnfFormula testSatt = {{lit0}, {lit1}, {nlit0, nlit1, lit2}};
  cnfFormula testSattt = {{nlit0, nlit1, lit2}, {lit0}, {lit1}};

  cnfFormula testSatttt = {{nlit0, nlit1, lit2}, {lit0, lit1}, {lit1, lit0}};
  
  cnfFormula testSattttt = {{nlit0, lit1},{lit0, lit1}, {nlit1, lit2}, {nlit0, nlit1}, {nlit0, lit1, lit2}, {nlit0, lit2}, {lit0, lit2}};;

  cnfFormula testUnsat1 = {{lit0}, {nlit0}};
  cnfFormula testUnsat2 = {{lit0}, {lit1},{nlit0,nlit1}};
  
  cnfFormula testUnsat3 = {{lit0}, {lit1}, {lit2}, 
			   {nlit0,nlit1,nlit2}};

  
  cnfFormula testUnsat4 = { {lit0, nlit0}, {lit1, nlit1} };
  
  bool result = dpll(testSattttt);
  //bool result = dpll(testUnsat3);


  if(result) cout << "SAT" << endl;
  else cout << "UNSAT" << endl;

  printGlobal();
  // print_graph(globalStruct.g, "01234");

}// end main








