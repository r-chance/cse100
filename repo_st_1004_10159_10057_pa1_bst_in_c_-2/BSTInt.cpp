
#include "BSTInt.h"
#include <iostream>

/* Function definitions for a int-based BST class */


/** Default destructor.
    Delete every node in this BST.
*/
BSTInt::~BSTInt() {
  deleteAll(root);
}

/** Given a reference to a Data item, insert a copy of it in this BST.
 *  Return  true if the item was added to this BST
 *  as a result of this call to insert,
 *  false if an item equal to this one was already in this BST.
 *  Note: This function should use only the '<' operator when comparing
 *  Data items. (should not use ==, >, <=, >=)  For the reasoning
 *  behind this, see the assignment writeup.
 */

bool BSTInt::insert(int item)
{
  // If BSTInt is empty
  if (!root) {
    root = new BSTNodeInt(item);
    ++isize;
    return true;
  }

  BSTNodeInt* curr = root;
  
  // Traverse BST to find insertion location
  while (curr->left || curr->right) {
    if (item < curr->data) {
      if(curr->left) {
        curr = curr->left;
      }
      else break; 
    } 
    else if (curr->data < item) {
      if(curr->right) {
        curr = curr->right;
      }
      else break;
    }
    else {
      return false;
    }
  }

  // Ready to insert
  BSTNodeInt* newNode = new BSTNodeInt(item);
 
  // Insert to left child if item < curr->data 
  if (item < curr->data) {
    curr->left = newNode;
    newNode->parent = curr;
  }
  // Insert to right child if item > curr->data
  else if(curr->data < item) {
    curr->right = newNode;
    newNode->parent = curr;
  }

  else {
    delete(newNode);
    return false;
  }
  ++isize;
  return true;
}


/** Find a Data item in the BST.
 *  Return true if the item is in the BST or false otherwise
 *  Note: This function should use only the '<' operator when comparing
 *  Data items. (should not use ==, >, <=, >=).  For the reasoning
 *  behind this, see the assignment writeup.
 */
bool BSTInt::find(int item) const
{

  BSTNodeInt* curr = root;
  while (curr) {
    if (curr->data < item) {
      curr = curr->right;
    }
    else if (item < curr->data) {
      curr = curr->left;
    }
    else {
      return true;
    }
  }
  return false;
}

  
/** Return the number of items currently in the BST.
 */
unsigned int BSTInt::size() const 
{
  return isize;
}

  
/** Return the height of the BST.
    Height of tree with just root node is 0
 */
int BSTInt::height() const
{
    int height = getHeight(root);
    return height;
}


/** This is a helper function for BSTInt::height().  It finds the distance
 *  between the parameter node and the most distant child node. */
int BSTInt::getHeight(BSTNodeInt* n) const {

    int left, right;

    // base case, no nodes or only root node
    if(!n || (!(n->right) && !(n->left) ) )
        return 0;

    // check height of left side of tree
    left = getHeight(n->left);
    // check height of right side of tree  
    right = getHeight(n->right);
   
    if( left < right )
        return right+1;

    else return left+1;
}


/** Return true if the BST is empty, else false. 
 */
bool BSTInt::empty() const 
{
    if(isize==0) return true;
    else return false;
}


/** do a postorder traversal, deleting nodes
 * This is a helper for the destructor
 * This method is static because it acts on BSTNodeInts
 *   and not on the BSTInt object directly.
 */
void BSTInt::deleteAll(BSTNodeInt* n)
{
    // base case, n is null
    if( !n ) return;

    // base case, no child nodes, delete n and return
    if( !(n->left) && !(n->right) ) {
        delete(n);
        return;
    }

    // case of left child, call deleteAll recursively on that child node.
    if( n->left ) {
        deleteAll(n->left);
    }

    // case of right child, call deleteAll recursively on that child node.
    if( n->right ) {
        deleteAll(n->right);
    }

    delete(n);
    return;

    
}

/** Helper function, returns the root node of the BST */
//BSTNodeInt* BSTInt::getRoot() {

//    return root;
//}
