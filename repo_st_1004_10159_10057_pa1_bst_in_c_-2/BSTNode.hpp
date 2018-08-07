#ifndef BSTNODE_HPP
#define BSTNODE_HPP
#include <iostream>
#include <iomanip>

/** Starter code for PA1, CSE 100 2016
 * Authors: Christine Alvarado, based on code by Paul Kube 
 *          Robert Chance
 *          A13088857
 */

template<typename Data>
class BSTNode {

public:

  Data const data; // The data for BSTNode object
  BSTNode<Data>* left;
  BSTNode<Data>* right;
  BSTNode<Data>* parent;
 
  /** Constructor.  Initialize a BSTNode with the given Data item,
   *  no parent, and no children.
   */
  BSTNode(const Data & d);


  /** Return the successor of this BSTNode in a BST, or 0 if none.
   ** PRECONDITION: this BSTNode is a node in a BST.
   ** POSTCONDITION:  the BST is unchanged.
   ** RETURNS: the BSTNode that is the successor of this BSTNode,
   ** or 0 if there is none.
   */
  BSTNode<Data>* successor(); 

}; 


// Function definitions
// For a templated class it's easiest to just put them in the same file as the
// class declaration

template <typename Data>
BSTNode<Data>::BSTNode(const Data & d) : data(d), left(0), right(0), parent(0) {}

/* Return a pointer to the BSTNode that contains the item that is sequentially
   next in the tree */
template <typename Data>
BSTNode<Data>* BSTNode<Data>::successor()
{
  
  BSTNode<Data>* curr = this;

  // Check for right child node and, if it exists, check for left child(ren)
  // of that node
  if( curr->right ) { 
    curr = curr->right;
 
    // iterate to bottom of left branch
    while( curr->left ) {
      curr = curr->left;
    }

    return curr;
  }
  
  // If no right child node exists
  check_parent:  
    // Check for parent node(s).  If curr ISN'T right child of that parent
    // node, return parent
    if( curr->parent && curr != curr->parent->right )
      return curr->parent;
    
    // If curr IS a right child node, check for parent(s) until one isn't a
    // right child, return the parent of that node, if it exists. 
    else if( curr->parent && curr == curr->parent->right ) {
      curr = curr->parent;
      goto check_parent; 

    }
      
  // If no parent exists to which curr is a left child, return 0
  else {
    return 0;
  }
}

/** Overload operator<< to print a BSTNode's fields to an ostream. */
template <typename Data>
std::ostream & operator<<(std::ostream& stm, const BSTNode<Data> & n) {
  stm << '[';
  stm << std::setw(10) << &n;                 // address of the BSTNode
  stm << "; p:" << std::setw(10) << n.parent; // address of its parent
  stm << "; l:" << std::setw(10) << n.left;   // address of its left child
  stm << "; r:" << std::setw(10) << n.right;  // address of its right child
  stm << "; d:" << n.data;                    // its data field
  stm << ']';
  return stm;
}

#endif // BSTNODE_HPP
