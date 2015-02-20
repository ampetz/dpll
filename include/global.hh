#ifndef GLOBAL_HH
#define GLOBAL_HH

#include <string>
#include <vector>
#include <algorithm>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/tuple/tuple.hpp>
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

  bool operator==(const lit& a) const{
  return (a.lbl == a.lbl && a.sign == a.sign);

  }
  
};

typedef vector<lit> clause;         // Disjunctive clause
typedef vector<clause> cnfFormula;  // Conjunction of disjunctive 
                                    // clauses(i.e. in CNF)


typedef boost::adjacency_list<  
  boost::vecS,              
  boost::vecS,               
  boost::bidirectionalS > graphType;


struct globalState {

  cnfFormula inputFormula;

  graphType g;

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

  vector<int> lastAssigned;



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
  bool alreadyAssigned = isAssigned(i);
  if(!alreadyAssigned){
    globalStruct.assigned[i] = true;
    globalStruct.assignments[i] = val;
    globalStruct.decision[globalStruct.decisionLevel].push_back(i);
    globalStruct.lastAssigned.push_back(i);
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


vector<int> pred (int tailVar){

  graphType graph = globalStruct.g;
  graphType::inv_adjacency_iterator ei, ei_end;
  vector<int> predecessors;
  for (boost::tie(ei, ei_end) = inv_adjacent_vertices(tailVar, graph); ei != ei_end; ++ei) predecessors.push_back((*ei));

  return predecessors;


}
vector<int> backTrace(int tailVar){

  graphType graph = globalStruct.g;
  //graphType::inv_adjacency_iterator ei, ei_end;
  vector<int> predecessors;
  
  
  //for (boost::tie(ei, ei_end) = inv_adjacent_vertices(tailVar, graph); ei != ei_end; ++ei) predecessors.push_back((*ei));  
  predecessors = pred(tailVar);

  if((predecessors.size()) == 1 && (predecessors[0] == (globalStruct.assigned.size()))) {

    vector<int> emptyVec;
    //cout << "HERE" << endl;
    return emptyVec;
    } 

  vector<int> all;
  vector<int> temp;

  cout << "SIZE of pred:  " << predecessors.size();

  for(vector<int>::iterator it = predecessors.begin(); it != predecessors.end(); ++it){
    bool exists = std::find(all.begin(), all.end(), ((*it))) != all.end();
    if(!exists) all.push_back((*it));
    temp = backTrace((*it));
    for(vector<int>::iterator it = temp.begin(); it != temp.end(); ++it){
      bool exists = std::find(all.begin(), all.end(), ((*it))) != all.end();
      
      if(!exists) all.push_back((*it));

    }
  }


  return all;
    

    }


pair<bool, int> distanceBetween(int tail, int goal){

  int rootId = globalStruct.assigned.size();
  vector<int> preds = pred(tail);
  
  bool rootReached = (preds.size() == 1) && (std::find(preds.begin(), preds.end(), rootId) != preds.end());

  if(rootReached) return make_pair(false, -1);

  bool goalFound = (std::find(preds.begin(), preds.end(), goal) != preds.end());
  
  if(goalFound) return make_pair(true, 1);

  
  vector<int> distances;
  for(vector<int>::iterator it = preds.begin(); it != preds.end(); ++it){

    int val = (*it);
    int dist = (distanceBetween(val, goal).first);
    distances.push_back(dist);
  }

  int min = (*min_element(distances.begin(), distances.end()));
  return make_pair(true, 1+min);

}


int countToUip(int tail, int uip){
  
  int dist = (distanceBetween(tail, uip)).second;

  


}

int firstUIP (vector<int> conVars, vector<int> allUips){

  //int min;
  int dist;
  vector<int> dists;
  
  int conVar = conVars[0];  //TODO:  loop through all conVars
  int min = (distanceBetween(conVar, allUips[0])).second;
  int id = allUips[0];

  for(vector<int>::iterator it = allUips.begin() + 1; it != allUips.end(); ++it){
    int uip = (*it);
    dist = countToUip(conVar, uip);
    if(dist < min) id = uip;
    
  }
  
  return id;
  

}


int findUIP (){

  int conClauseIndex = globalStruct.conflictClauseIndex;
  
  vector<lit> conClause = globalStruct.inputFormula[conClauseIndex];

  vector<int> conVars2;
  vector<int> conVars;
  

  for(vector<lit>::iterator it = conClause.begin(); it != conClause.end(); ++it){
    conVars2.push_back((*it).lbl);
  }

  for(vector<int>::iterator it = conVars2.begin(); it != conVars2.end(); ++it){
    int var = (*it);

    bool found = std::find(conVars.begin(), conVars.end(), var) != conVars.end();
    if(!found) conVars.push_back(var);

  }

  vector<vector<int> > allVec;
  vector<int> temp, temp2;
  bool inAll = true;
  vector<int> result;

  for(vector<int>::iterator it = conVars.begin(); it != conVars.end(); ++it){

    temp = backTrace((*it));
    allVec.push_back(temp);
  }

  temp = allVec[0];

  for(vector<int>::iterator oit = temp.begin(); oit != temp.end(); ++oit){
    int val = (*oit);

    for(vector<vector<int> >::iterator init = allVec.begin() + 1; init != allVec.end(); ++init) {
      temp2 = (*init);
    
      bool found = std::find(temp2.begin(),temp2.end(), val) != temp2.end();
    if (!found) { inAll = false; break; }

    } // inner for

    if(inAll) result.push_back(val);

  } // outer for

  //Now result contains all UIPs.  Find firstUip

  return firstUIP(conVars, result);
   
}

vector<pair<int, int> > freq(vector<int> vars){

  vector<pair<int,int> > result;
  vector<int> l = globalStruct.lastAssigned;

  for(vector<int>::iterator it = vars.begin(); it != vars.end(); ++it){
    int var = (*it);

    vector<int>::iterator it2 = std::find(l.begin(), l.end(), var);
    int diff = (l.end() - it2);
    cout << "DIFF: " << diff << endl;
    
    result.push_back(make_pair(var, diff));

  }

  return result;

}

int lastAssignedLit (clause cl){

  vector<int> l; // = globalStruct.lastAssigned;

  for(vector<lit>::iterator it = cl.begin(); it != cl.end(); ++it){
    lit aLit = (*it);
    int val = aLit.lbl;
    bool exists =  std::find(l.begin(), l.end(), val) != l.end();
    if(!exists) l.push_back(val);
  }

  vector<pair<int, int> > result = freq(l);

  int min = (*(result.begin())).second;
  int id = (*(result.begin())).first;
  for(vector<pair<int,int> >::iterator it = result.begin() + 1; it != result.end(); ++it){

      int newval = (*it).second;
      int newid = (*it).first;
      if(newval < min) {min = newval; id = newid;}
  }

  return id;
  
}


bool stopCriterionMet (clause c) {

  vector<int> varsAtDl = globalStruct.decision[globalStruct.decisionLevel];

  vector<int> varsInCl;
  int uip = findUIP();
  bool result = true;

  for(vector<lit>::iterator it = c.begin(); it != c.end(); ++it){
    int var = (*it).lbl;

    bool found = std::find(varsInCl.begin(), varsInCl.end(), var) != varsInCl.end();
    if(!found) varsInCl.push_back(var);

  }


  int count = 0;
  int varAt;
  for(vector<int>::iterator it = varsAtDl.begin(); it != varsAtDl.end(); ++it){
    int varDl = (*it);
    bool inClause = std::find(varsInCl.begin(), varsInCl.end(), varDl) != varsInCl.end();
    if(inClause) {count++; varAt = varDl;}

  }

  if(count != 1) return false;

  for(vector<lit>::iterator it = c.begin(); it != c.end(); ++it){
    lit aLit = (*it);
    if(aLit.lbl != varAt) continue;
    
    if(aLit.sign == true) return false;
    else return true;

  }

  
}

clause resolve (clause c, clause ante, int var){

  c.insert(c.end(), ante.begin(), ante.end());
  lit removeLit;
  removeLit.lbl = var;
  removeLit.sign = true;
  lit nRemoveLit;
  nRemoveLit.lbl = var;
  nRemoveLit.sign = false;
  c.erase(std::remove(c.begin(), c.end(), removeLit), c.end());
  c.erase(std::remove(c.begin(), c.end(), nRemoveLit), c.end());

  return c;

}

clause intsToClause (vector<int> intAntes){

  vector<lit> result;

  for(vector<int>::iterator it = intAntes.begin(); it != intAntes.end(); ++it){
    int var = (*it);
    bool assignment = globalStruct.assignments[var];

    bool sign = !assignment;
    lit aLit;
    aLit.lbl = var;
    aLit.sign = sign;

    result.push_back(aLit);
  }

  return result;


}


int decLevel (int var){
  int result;

  //int outerIndex;
  for(vector<vector<int> >::iterator it = globalStruct.decision.begin(); it != globalStruct.decision.end(); ++it){

    int outerIndex = (it - globalStruct.decision.begin());
    vector<int> inner = (*it);

    for(vector<int>::iterator init = inner.begin(); init != inner.end(); ++init){

      int val = (*init);
      if(val == var) return outerIndex;

    }

  }

}
int analyzeConflict (){
  if(globalStruct.decisionLevel == 0) return -1;

  
  clause currentConflict = globalStruct.inputFormula[globalStruct.conflictClauseIndex];

  while(!stopCriterionMet(currentConflict)){

  

    int var = lastAssignedLit(currentConflict);
    vector<int> ante = pred(var);
    clause clAnte = intsToClause(ante);


    currentConflict = resolve(currentConflict, clAnte, var);


    }

  globalStruct.inputFormula.insert(globalStruct.inputFormula.begin(), currentConflict);

  
  vector<int> conInts;
  for(vector<lit>::iterator it = currentConflict.begin(); it != currentConflict.end(); ++it){

    lit aLit = (*it);
    int val = aLit.lbl;

    bool found = std::find(conInts.begin(), conInts.end(), val) != conInts.end();
    if(!found) conInts.push_back(val);
  }

  vector<int> decLevels;
  for(vector<int>::iterator it = conInts.begin(); it != conInts.end(); ++it){
    int val = (*it);
    decLevels.push_back(decLevel(val));

  }

  
  std::sort(decLevels.begin(), decLevels.end());

  int max = decLevels.back();

  for(vector<int>::reverse_iterator it = (decLevels.rbegin() + 1); it != decLevels.rend(); ++it){
    int val = (*it);
    if(max != val){max = val; break;}

  }
  
  return max;
  //c.insert(c.end(), ante.begin(), ante.begin());
  


  return 0;
}

void backTrack (int level) {

  //TODO:  forEach decision level d > level, 
           //forEach vertex v at decision level d, clear_vertex(v, g)

  vector<int> vars;

  for(vector<vector<int> >::iterator it = globalStruct.decision.begin(); it != (globalStruct.decision.begin() + level + 1); ++it){
      vector<int> newVars = (*it);
      for(vector<int>::iterator it = newVars.begin(); it != newVars.end(); ++it){
	int val = (*it);
	vars.push_back(val);
      }


    }

	for(int i = level+1; i < globalStruct.decision.size(); i++){
	  globalStruct.decision[i].clear();
	}


	for(int i = 0; i < globalStruct.assigned.size(); i++){
	  bool found = std::find(vars.begin(), vars.end(), i) != vars.end();
	  if(!found) globalStruct.assigned[i] = false;


	}
	


	for(vector<int>::iterator it = vars.begin(); it != vars.end(); ++it){

	  int val = (*it);
	  clear_vertex(val, globalStruct.g);
	}

	
	globalStruct.lastAssigned.clear();

	globalStruct.decisionLevel = level;

	return;



	
  // TODO:  Collect vector vec of variables decided at decision levels < level. 
  //        Zero out assigned[], then repopulate assigned[] and assignments[] with ONLY variables in vec.  

  // TODO:  decisionLevel = level

}


#endif // GLOBAL_HH
