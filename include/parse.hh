#ifndef PARSE_HH
#define PARSE_HH

#include <sstream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include<iostream>
#include "global.hh"

using namespace std;

void printFormula (cnfFormula result){

  for(cnfFormula::iterator it = result.begin(); it != result.end(); ++it){
    clause c = (*it);
    for(clause::iterator it = c.begin(); it != c.end(); ++it){

      lit l = (*it);
      if(!l.sign) cout << "-";
      cout << l.lbl << ", " ;
    } // inner for

    cout << endl;
    } // outer for 

}


cnfFormula parseInput(string inFileName){
  
  std::ifstream infile(inFileName);

  std::string line;
  
  int numVars, numClauses;
  cnfFormula result;

  while (std::getline(infile, line))
    {
      std::istringstream iss(line);
      string first;
      clause newClause;

      iss >> first;

      //cout << "first: " << first << endl;

      if(first == "c") continue;
      if(first == "p"){
	  string cnfSt;
	  iss >> cnfSt;

	  iss >> numVars >> numClauses;

	  // cout << numVars << endl;
	  // cout << numClauses << endl;
	  continue;
	}// end inner if


      // cout << first << endl;

      int val = stoi(first);
      do{
       	//cout << val << endl;
	lit newLit;
	bool newSign = (val > 0);
	  
	if(!newSign) newLit.lbl = (val*-1) - 1;
	else newLit.lbl = val - 1;
	newLit.sign = newSign;
	newClause.push_back(newLit);
	
	iss >> first;
	val = stoi(first);

      } while (val != 0);


      result.push_back(newClause);

    } // end while


  
  //printFormula(result);


  return result;

} // end parseInput




#endif // PARSE_HH
