#ifndef GLOBAL_HH
#define GLOBAL_HH

#include <string>
#include <vector>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
using namespace std;
using namespace boost;

/*
struct node {
  int lbl;
  bool val;

  bool operator==(const node& a) const{
    return (lbl == a.lbl);
  }

  };  */

//typedef int node;

struct lit {
  int lbl;   // Variable label(0=x0, 1=x1, etc.)
  bool sign; // Variable sign(true=positive, false=negated)

};

typedef vector<lit> clause;         // Disjunctive clause
typedef vector<clause> cnfFormula;  // Conjunction of disjunctive 
                                    // clauses(i.e. in CNF)





struct globalState {

  cnfFormula inputFormula;

  boost::adjacency_list <> g;

  vector<bool> assignments;
  /*[true, false, true] represents:  {x0->1, x1->0, x2->1} */
 
  vector<bool> assigned;    
  /*  [false, false, true] represents: x2 is the only variable that has been 
      assigned a boolean value.  x0 and x1 are candidates for a decision.

      if ( and(assigned) == true ) then all variables have been assigned 
      ( Decide() is done ).
  */

  vector<vector<int> > decision;
  /*  Outer vector indexed by decision level.  Each inner vector contains all 
      variables(represented by their index) that were assigned at that 
      decision level. (i.e.  [ [0], [1,3], [2] ] represents: x0 was assigned 
      a boolean value at decision level 0, x1 and x3 at level 1, and x2 at 
      level 2)
  */

  //vector<vector <bool> > formula;  //TODO:  Should this be of type cnfFormula?
  /*  Assumes CNF.  Outer index is conjuncts, inner is disjunctive clause.
      This represents the current state of the formula after each assignment
      (this is where we will check for satisfiability).  
      
      
  */

  int decisionLevel;

  int conflictClauseIndex;

}; //globalStruct;




globalState globalStruct;

bool isAssigned(int var){
  return globalStruct.assigned[var];


}

int numUniqueVars (cnfFormula inFormula){

  cnfFormula v = inFormula; //globalStruct.inputFormula
  cnfFormula::iterator outerIt;
  clause::iterator innerIt;
  
  vector<int> uniqueVars;
  int count = 0;


  for (outerIt = v.begin(); outerIt != v.end(); ++outerIt) {
    clause c = (*outerIt);
    //int outerIndex = (outerIt - v.begin());
    
    for(innerIt = c.begin(); innerIt != c.end(); ++innerIt) {

      lit l = (*innerIt);
      int var = l.lbl;
      bool exists = std::find(uniqueVars.begin(), uniqueVars.end(), var) != uniqueVars.end();
      //count++;

      if(!exists) uniqueVars.push_back(var);
    } // inner for

  } // outer for

  return uniqueVars.size();

}

  //TODO:  add tree updates here??
void assign (int i, bool val)
{
  //cout << "in assign()!!!!";
  globalStruct.assignments[i] = val;
  bool alreadyAssigned = isAssigned(i);
  if(!alreadyAssigned){
    globalStruct.assigned[i] = true;
  globalStruct.decision[globalStruct.decisionLevel].push_back(i);
  }
  //globalStruct.decisionLevel += 1; // Dont do this here, since implied vars stay on same decision level
}




bool getLitTruthValue (lit daLit) {

  int label = daLit.lbl;
  bool truthValue;
  bool assignment = globalStruct.assignments[label];
  if(daLit.sign == false) truthValue = !(assignment) ;
  else truthValue = assignment;

  return truthValue;
}

void addEdge (int a, int b){

  add_edge(a, b, globalStruct.g);


}

void addRootNode (int label){

  int masterRootNode = globalStruct.assigned.size();
  addEdge(masterRootNode, label);

}

bool decide ()
{
  cout << "in DECIDE()" << endl;
  //bool allAssigned = true;
  vector<bool> v = globalStruct.assigned;
  

    // TODO:  Is there a way to do more efficient AND of entire array?
    for (vector<bool>::iterator it = v.begin(); it != v.end(); ++it) {
      if ( (*it) == false ) {
	//TODO:  Heuristic for choosing index and truth value here...
	int index = it - v.begin();
	//cout << endl << index << endl;
	bool truthValue = true;
	//cout << "past past";
	assign(index, truthValue);
	//cout << "Made it here!";
	addRootNode(index);
	return true; // Successful assignment
      }
    }

    return false; // All variables already assigned
}

void updateGraph (vector<int> adjacentVars, int impliedVar){
  
  //cout << "IN updateGraph()" << endl;
  if(adjacentVars.size() == 0) addRootNode(impliedVar);
  else{
    for(vector<int>::iterator it = adjacentVars.begin(); 
	it != adjacentVars.end(); ++it){
    
      int incomingVertex = (*it);
      //cout << incomingVertex << ", " << endl;
      addEdge(incomingVertex, impliedVar);

    }
  } 

  } 

//returns -1 if none found.  -2 on conflict.
//  Otherwise, variable that is implied by unit clause
pair<int,int> locateUnitClause () {
  cout << "IN LOCATE LOOP" << endl << endl;

  cnfFormula v = globalStruct.inputFormula; 
  cnfFormula::iterator outerIt; // reverse??
  clause::iterator innerIt;

  int numLitsSatisfied;
  int numLitsAssigned;
  int clauseSize;

  lit returnLit;
  lit daLit;
  int outerIndex, innerIndex;

  for (outerIt = v.begin(); outerIt != v.end(); ++outerIt) {
    clause c = (*outerIt);

    outerIndex = (outerIt - v.begin());
    
    numLitsSatisfied = 0;
    numLitsAssigned = 0;
    clauseSize = c.size();
    cout << endl << "Clause size: " << clauseSize << endl;
    

    for(innerIt = c.begin(); innerIt != c.end(); ++innerIt) {  
      daLit = (*innerIt);
      innerIndex = (innerIt - c.begin());
      int var = daLit.lbl;
      bool isItAssigned = isAssigned(daLit.lbl);
      bool truthValue = getLitTruthValue (daLit);
      
      
      if (isItAssigned) numLitsAssigned++ ;
      else continue;

      if (truthValue) numLitsSatisfied++ ; 
      
    } //inner for

   
    if(numLitsAssigned == clauseSize && numLitsSatisfied == 0){
      //returnLit.lbl = -2;
      cout << "Conflict Clause: " << endl;
      for(innerIt = c.begin(); innerIt != c.end(); ++innerIt) {  
	daLit = (*innerIt);
	if(!daLit.sign) cout << "not-" ;
	cout << daLit.lbl << ",";
      }
      cout << endl;
      return make_pair(-2, outerIndex);
    }
    if(numLitsAssigned == (clauseSize - 1) && numLitsSatisfied == 0){

      vector<int> uv;
      clause::iterator it;

      lit unitLit;
      lit temp;
      for(it = c.begin(); it != c.end(); ++it){
	temp = (*it);
	if(!isAssigned(temp.lbl)){
	  unitLit = temp;
	  break;
	}
      }

      for(it = c.begin(); it != c.end(); ++it)	
	if(std::find(uv.begin(), uv.end(), (*it).lbl) == uv.end()) 
	  if((*it).lbl != unitLit.lbl) uv.push_back((*it).lbl);

      //cout << "SIZE: " << uv.size();
      updateGraph(uv, unitLit.lbl);

      //globalStruct.inputFormula.erase(outerIt);
      return make_pair(outerIndex, innerIndex);

    }

  } // outer for

  //returnLit.lbl = -1;
  return make_pair(-1,-1);

}


// Returns true on conflict, else false
bool bcp () {
  cout << "IN BCP LOOP" << endl;
  int result = 0;

  while(result != -1 && result != -2){
    
    //lit unitLit = locateUnitClause();
    pair<int, int> p = locateUnitClause();
    lit unitLit = globalStruct.inputFormula[p.first][p.second];
    
    if(p.first == -1){ // no more unit clauses(no more implications)
      globalStruct.decisionLevel += 1;
      return false;

    }

    if(p.first == -2) {// conflict     
      globalStruct.conflictClauseIndex = p.second;
      return true;

    }

    cout << "UNIT CLAUSE REACHED" << endl;
    cout << "Implied Var:  " << unitLit.lbl << "," << unitLit.sign << endl;
    
    
    if(isAssigned(unitLit.lbl) && 
       !(globalStruct.assignments[unitLit.lbl] == unitLit.sign))
      return true;


    if(!isAssigned(unitLit.lbl)){
      int var = unitLit.lbl;
      bool sign = unitLit.sign;
      bool assignment;
      if(sign) assignment = true;
      else assignment = false;

      assign(var, assignment);
      }
    
    //globalStruct.inputFormula.erase(globalStruct.inputFormula.begin() + p.first);

    bool result = bcp ();

    } // end while


} // end bcp()

int findUIP (){


}

int analyzeConflict (){


  return 0;
}

void backTrack (int level) {

  //TODO:  forEach decision level d > level, 
           //forEach vertex v at decision level d, clear_vertex(v, g)

  // TODO:  Collect vector vec of variables decided at decision levels < level. 
  //        Zero out assigned[], then repopulate assigned[] and assignments[] with ONLY variables in vec.  

  // TODO:  decisionLevel = level

}


#endif // GLOBAL_HH
