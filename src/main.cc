#include "formula.hh"
#include "global.hh"

#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char** argv) {
  term t1 = term("x1");
  term t2 = term("x2");
  formula f = formula({t1,t2});

  cout << f << endl;

  cout << "Hello" << endl ;
}
