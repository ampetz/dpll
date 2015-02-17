#ifndef TERM_HH
#define TERM_HH

#include <string>
using std::string;

class term {
  public:
    term()  = default;
    ~term() = default;
    
    term(const string& s) 
      : ident(s) 
    { 
    }

    string ident;
};

#endif // TERM_HH
