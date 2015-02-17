#ifndef FORMULA_HH
#define FORMULA_HH

#include "term.hh"

#include <ostream>
#include <initializer_list>
#include <vector>
using std::initializer_list;
using std::vector;
using std::ostream;

enum class format { CNF, NNF, DNF };

class formula {
  public:
    formula()   = default;
    ~formula()  = default;

    formula(initializer_list<term> init) 
      : _terms(init), _format(format::CNF) 
    {
    }

    friend ostream& operator<<(ostream& o, formula f);

  private:
    vector<term> _terms;
    format       _format;
};


ostream& operator<<(ostream& o, formula f) {
  for(const auto& t : f._terms) {
    o << " " << t.ident << " ";
  }

  return o;
}
#endif //FORMULA_HH
