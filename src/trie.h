
#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED    1

#include <iostream>
#include <cstring>

using namespace std;

template <typename T>
class trie {

  private:
    // node of trie. we do not want this to be visible to user
    struct trie_node {
      T* v;
      char c;
      trie_node * next;
      trie_node * left;
      trie_node * right;
      trie_node * parent;

      // node constructors
      trie_node(const char& c) : c(c), v(0), next(0), left(0), right(0), parent(0) {}
      trie_node(const char& c, T& v) : c(c), v(new T(*v)), next(0), left(0), right(0), parent(0) {}
      trie_node(const trie_node& n) : c(n.c), v(n.v ? new T(*n.v) : 0), next(0), left(0), right(0), parent(0) {}

      // node destructor
      ~trie_node ( ) {
        delete v;
      }
    };

    // pointer to the root of the trie. Direct manipulation is forbidden
    trie_node * root;

    // lookup. If word exists in trie, then function will return pointer to
    // node representing that word. This function is nessesery for searching and removing elements
    trie_node * lookup( const char word [] );

    // printing. Executed by 'show' function
    void print ( ostream& stream, trie_node * node ) const;

    // recursive deletion of trie
    void delete_trie ( trie_node * node );

    // auxilary method for cloning
    void recursive_clone( trie_node * n1, trie_node * n2 ) const;

  public:

    //
    // user interface
    //

    // constructors
    trie ( ) : root(new trie_node(' ')) {}

    trie ( const trie<T>& t ) : root(new trie_node(' ')) {
      recursive_clone(t.root,root);
    }

    // assignment operator
    void operator = ( const trie<T>& t ) {
      delete_trie( root );
      root = new trie_node(' ');
      recursive_clone(t.root,root);
    }

    // clone method. return pointer to exact copy of this trie
    trie<T>* clone( ) const {
      trie<T> * t = new trie<T>();
      recursive_clone(root,t->root);
      return t;
    }

    // inserting
    T* insert( const char word [], T value );

    // removing
    bool remove( const char word [] );

    // searching
    bool contains( const char word [] ) {
      return lookup ( word );
    }

    // retriving value. Will return 0 if trie does not contain word
    T* get( const char word [] ) {
      trie_node * node = lookup ( word );
      return node ? node->v : 0;
    }

    bool isEmpty ( ) {
      return root->next;
    }

    // show
    void show ( ostream& stream ) const {
      print ( stream, root );
    }

    //
    // clean-up operations
    //

    // making the trie empty
    void clean ( ) {
      delete_trie( root );
      // reinitialization
      root = new trie_node(' ');
    }

    ~trie ( ) {
      delete_trie ( root );
    }
};

// insertion
template <typename T>
T* trie<T>::insert ( const char word [], T value ) {
  trie_node * act = root;
  int word_len = strlen(word);
  int i = 0;

  while ( act->next ) {

    act = act->next;

    while ( act->c != word[i] && act->right != 0) {
      act = act->right;
    }

    if ( act->c != word[i] ) {
      // node does not have child labeled 'c'
      // we are at the end of brothers list
      trie_node * node = new trie_node ( word[i] );
      act->right = node;
      node->left = act;
      node->parent = act->parent;
      act = act->right;
    }

    // we will consider next character of word in next iteration
    i++;

    // but if the word is finished ..
    if ( i == word_len ) {
      // .. we check if there is another string like word already in a trie
      if ( act->v ) {
        return act->v;
      }
      else {
        act->v = new T(value);
        return act->v;
      }
    }
  }

  // after leaving the loop we are in leaf node that represents the prefix of word.
  // word[1..i] is already inside the tire. we need to create more nodes and complete insertion
  while ( i < word_len ) {
    trie_node * node = new trie_node ( word[i] );
    act->next = node;
    node->parent = act;
    act = act->next;
    i++;
  }

  act->v = new T(value);

  return act->v;
}


// auxilary method for cloning. n1 is being copied to n2. from left to right
template <typename T>
void trie<T>::recursive_clone( trie_node * n1, trie_node * n2 ) const {

  if ( n1->next ) {
    n1 = n1->next;
    // creating new node
    trie_node * nt1 = new trie_node(*n1);
    nt1->parent = n2;
    n2->next = nt1;

    n2 = n2->next;
    // recursive call to new node
    recursive_clone( n1, n2 );

    // time to copy brothers
    while ( n1->right ) {
      n1 = n1->right;
      // creating new node
      trie_node * nt2 = new trie_node(*n1);
      nt2->parent = n2->parent;
      nt2->left = n2;
      n2->right = nt2;

      n2 = n2->right;
      // recursive call to new node
      recursive_clone( n1, n2 );
    }
  }
}

// searching
template <typename T>
typename trie<T>::trie_node * trie<T>::lookup( const char word [] ) {
  trie_node * act = root;
  int word_len = strlen(word);
  int i = 0;

  while( act->next ) {
    act = act->next;

    while ( act->c != word[i] && act->right != 0) {
      act = act->right;
    }

    if ( act->c != word[i] ) {
      // we cannot continue
      return 0;
    }
    i++;

    if ( i == word_len ) {
      if ( act->v )
        return act;
      else
        return 0;
    }
  }
  return 0;
}

// removing element
template <typename T>
bool trie<T>::remove( const char word [] ){
  trie_node * del = lookup ( word );
  trie_node * tmp;
  if ( del ) {
    // delete from bottom to first spliting node
    while( del->parent && !(del->right || del->left) ) {
      tmp = del->parent;
      delete del;
      del = tmp;
    }

    if (del == root) {
      // if we removed every node we should be at the root now
      root->next = 0;
      return true;
    }

    if(del->left) {
      // we have left-brother
      tmp = del->left;
      tmp->right = del->right;
    } else {
      // or 'del' is the first node on the current child-list
      tmp = del->parent;
      tmp->next = del->right;
      tmp->next->left = 0;
    }

    if(del->right) {
      // we have right-brother
      tmp = del->right;
      tmp->left = del->left;
    }
    delete del;
    return true;
  } else {
    return false;
  }
}

// printing. for debuging purposes
template <typename T>
void trie<T>::print ( ostream& stream, trie_node * node ) const {
  if (node->v) stream << node->c << "("<< node->v << "," <<*(node->v) <<")";
  else stream << node->c;
  if ( node->next ) {
    node = node->next;
    print ( stream, node );

    while ( node->right ) {
      node = node->right;
      print ( stream, node );
    }
  } else {
    stream << "\n";
  }
}

// cleanup operation
template <typename T>
void trie<T>::delete_trie ( trie_node * node ) {
  if ( node->next ) {
    delete_trie (  node->next );
  }
  if ( node->right ) {
    delete_trie ( node->right );
  }
  delete node;
}

// overloaded output stream operator
template <typename T>
ostream& operator << ( ostream& stream, const trie<T> &t ) {
   t.show( stream );
   return stream;
}

#endif
