#include <iostream>

#include "trie.h"
#include "assoc_array.h"

int main (int argc, char * argv []) {

// use this space for testing. delete the code below if you wish. put yours. recompile. have fun :)

  for (int i=0; i<10000000; i++) {
    int a = 5;
    assoc_array<int> arr;
    arr("karp",a);
    arr("kot",a);
    cout << arr("karp") << endl;
    cout << arr("kot") << endl;

    assoc_array<int> arr2(arr);
    arr2("kar",6);
    arr2("kot",8);
    cout << *arr2("kar") << endl;
    cout << *arr2("kot") << endl;
    cout << arr("kar") << endl;

    assoc_array<int> arr3;
    arr3 = arr;
    arr3("word",2);
    cout << *arr3("kot") << " " << *arr3("word") << endl;
    arr3.remove("kot");
    if ( arr3("kot") ) cout << "wrong!";
    cout << "BUT THIS IS OK: " << *arr("kot") << endl;
  }
  return 0;
}
