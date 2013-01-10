
#ifndef ASSOC_INCLUDED
#define ASSOC_INCLUDED    1

#include <vector>
#include <iostream>

#include "trie.h"

using namespace std;

template <typename T>
class assoc_array {
  private:
    trie<T> t;

  public:

    // constructors
    assoc_array ( ) {}

    assoc_array ( assoc_array& arr ) : t(arr.t) {} // something bad??

    //
    // operators
    //

    // getting value
    T* operator () ( const char word [] ) {
      return t.get(word);
    }

    // setting value
    void operator () ( const char word [], const T& val) {
      t.insert(word, val);
    }

    // assignment
    void operator = ( const assoc_array<T>& arr ) {
      t = arr.t;
    }

    bool remove ( const char word [] ) {
      return t.remove(word);
    }

    void clear ( ) {
      t.clean();
    }

    // destructor
    ~assoc_array ( ) { }
};

#endif
