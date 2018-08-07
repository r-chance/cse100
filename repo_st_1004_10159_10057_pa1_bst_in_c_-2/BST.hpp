#ifndef BST_HPP
#define BST_HPP
#include "BSTNode.hpp"
#include "BSTIterator.hpp"
#include <iostream>

template<typename Data>
class BST {

public:

  /** define iterator as an aliased typename for BSTIterator<Data>. */
  typedef BSTIterator<Data> iterator;

  /** Default constructor.
      Initialize an empty BST.
      This is inlined because it is trivial.
   */
  BST() : root(nullptr), isize(0) {}


  /** Default destructor.
      Delete every node in this BST.
  */
  ~BST();

  /** Given a reference to a Data item, insert a copy of it in this BST.
   *  Return a pair where the first element is an iterator 
   *  pointing to either the newly inserted element or the element 
   *  that was already in the BST, and the second element is true if the 
   *  element was newly inserted or false if it was already in the BST.
   * 
   *  Note: This function should use only the '<' operator when comparing
   *  Data items. (should not use ==, >, <=, >=)  
   */
  std::pair<iterator, bool> insert(const Data& item);


  /** Find a Data item in the BST.
   *  Return an iterator pointing to the item, or pointing past
   *  the last node in the BST if not found.
   *  Note: This function should use only the '<' operator when comparing
   *  Data items. (should not use ==, >, <=, >=).  For the reasoning
   *  behind this, see the assignment writeup.
   */
  iterator find(const Data& item) const;

  
  /** Return the number of items currently in the BST.
   */ 
  unsigned int size() const;
  
  /** Return the height of the BST.
      height of a tree starts with root at height 0
	  height of an empty tree is -1
   */ 
  int height() const;


  /** Return true if the BST is empty, else false. */
  bool empty() const;

  /** Return an iterator pointing to the first (smallest) item in the BST.
   */ 
  iterator begin() const;

  /** Return an iterator pointing past the last item in the BST.
   */
  iterator end() const;

  /** Helper function, returns the root of the templated BST */
//  BSTNode<Data>* getRoot();

private:

  /** Pointer to the root of this BST, or 0 if the BST is empty */
  BSTNode<Data>* root;
  
  /** Number of Data items stored in this BST. */
  unsigned int isize;

  /** Find the first element of the BST
   * Helper function for the begin method above.
   */ 
  static BSTNode<Data>* first(BSTNode<Data>* root);

  /** do a postorder traversal, deleting nodes
   */
  static void deleteAll(BSTNode<Data>* n);
 
  /** Helper function for finding height of BST */
  int getHeight(BSTNode<Data>* n) const;
 };


// ********** Function definitions ****************


/** Default destructor.
    Delete every node in this BST.
*/
template <typename Data>
BST<Data>::~BST() {
  deleteAll(root);
}


/** Given a reference to a Data item, insert a copy of it in this BST.
 *  Return a pair where the first element is an iterator pointing to either the newly inserted
 *  element or the element that was already in the BST, and the second element is true if the 
 *  element was newly inserted or false if it was already in the BST.
 * 
 *  Note: This function should use only the '<' operator when comparing
 *  Data items. (should not use ==, >, <=, >=)  
 */
template <typename Data>
std::pair<BSTIterator<Data>, bool> BST<Data>::insert(const Data& item) {

  // If BST is empty
  if( !root ) {
    root = new BSTNode<Data>(item);
    ++isize;
    return std::pair<BSTIterator<Data>,bool> (BSTIterator<Data>(root), true);
  }

  BSTNode<Data>* curr = root;

  // Traverse tree to find correct location for insertion
  while( curr->left || curr->right ) {

    // If item < curr->data, traverse left subtree
    if( item < curr->data ) {
      if( curr->left ) {
        curr = curr->left;
      }
      else break;
    }

    // If item > curr->data, traverse right subtree
    else if( curr->data < item ) {
      if( curr->right ) {
        curr = curr->right;
      }
      else break;
    }

    // If item == curr->data, return curr and false to indicate that no new
    // node was inserted.
    else {
      return std::pair<BSTIterator<Data>,bool> (BSTIterator<Data>(curr), false);
    }
  }

  // Create new node to insert
  BSTNode<Data>* newNode = new BSTNode<Data>(item);

  // Insert to left child if item < curr->data
  if( item < curr->data ) {
    curr->left = newNode;
    newNode->parent = curr;
  }

  // Insert to right child if item > curr->data
  else if( curr->data < item ) {
    curr->right = newNode;
    newNode->parent = curr;
  }

  // If item == curr->data, don't insert.  Return pointer to curr and false.
  else {
    return std::pair<BSTIterator<Data>,bool> (BSTIterator<Data>(curr), false);
  }

  // Increment BST size and return iterator and true
  ++isize;
  return std::pair<BSTIterator<Data>, bool>(BSTIterator<Data>(newNode), true);
}


/** Find a Data item in the BST.
 *  Return an iterator pointing to the item, or pointing past
 *  the last node in the BST if not found.
 *  Note: This function should use only the '<' operator when comparing
 *  Data items. (should not use ==, >, <=, >=).  For the reasoning
 *  behind this, see the assignment writeup.
 */
template <typename Data>
BSTIterator<Data> BST<Data>::find(const Data& item) const
{
  auto temp = this->begin();
  auto end = this->end();

  // Traverse tree from begin to end
  while(temp != end) {
    if(*temp < item || item < *temp) {
      ++temp;
    }
    
    // return iterator if iterator value == item
    else {
      return temp;
    }
  }
  return end;
}

  
/** Return the number of items currently in the BST.
 */ 
template <typename Data>
unsigned int BST<Data>::size() const
{
  return isize;
}

/** Return the height of the BST.
 */
template <typename Data> 
int BST<Data>::height() const
{
  return getHeight(root);
}


/** Return true if the BST is empty, else false.
 */ 
template <typename Data>
bool BST<Data>::empty() const
{
  if( isize > 0 ) return false;  
  return true;
}

/** Return an iterator pointing to the first (smallest) item in the BST.
 */ 
template <typename Data>
BSTIterator<Data> BST<Data>::begin() const
{
  return BSTIterator<Data>(first(root));
}

/** Return an iterator pointing past the last item in the BST.
 */
template <typename Data>
BSTIterator<Data> BST<Data>::end() const
{
  return BSTIterator<Data>(nullptr);
}

/** Find the first element of the BST
 * Helper function for the begin method above.
 */ 
template <typename Data>
BSTNode<Data>* BST<Data>::first(BSTNode<Data>* root)
{
  BSTNode<Data>* curr = root;

  // traverse down left side of tree until curr->left is null, then return curr
  while( curr ) {
    if( curr->left ) {
      curr = curr->left;
    }
    else break;
  }
  return curr;
}

/** Helper function for destructor, uses a postorder traversal, deleting nodes
 */
template <typename Data>
void BST<Data>::deleteAll(BSTNode<Data>* n)
{
  // base case, if n is null
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

  // delete the root of n's single node subtree
  delete(n);
  return;
}

// Helper function for height(). Searches recursively for longest path in tree
// and returns the value as height.
template <typename Data>
int BST<Data>::getHeight(BSTNode<Data>* n) const {

  int left, right;

  // base case, no nodes or only root node
  if( !n || (!(n->right) && !(n->left)) ) 
    return 0;
  
  // check height of left side of tree
  left = getHeight(n->left);
  // check height of right side of tree
  right = getHeight(n->right);

  // return larger value as height of tree (+1 for root)
  if( left < right )
    return right+1;

  else return left+1;
}

/* Helper function, returns the root node of the BST */
/*template <typename Data>
BSTNode<Data>* BST<Data>::getRoot() {

 return this->root;
} 
*/
#endif //BST_HPP
