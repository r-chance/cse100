#include "BSTInt.h"
#include "BST.hpp"
#include "BSTIterator.hpp"

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <set>

using namespace std;

/**
 * A test driver for the BST int class and class template.
 * PA1 CSE 100 2016
 * Authors:Christine Alvarado
 *         Robert Chance
 *         A13088857
 */
int main() {

    /* Create an STL vector of some ints */
    /*test*/
    vector<int> v;
    v.push_back(3);
    v.push_back(4);
    v.push_back(1);
    v.push_back(100);
    v.push_back(-33);
    v.push_back(-31);

    /* Create a second vector of ints */
    vector<int> v2;
    v2.push_back(1);
    v2.push_back(0);
    v2.push_back(1);
    v2.push_back(0);

    /* Create an instance of BST holding int */
    BSTInt b;
    BSTInt b2;
    BSTInt emptyBST;    

    // Could use: for(auto item : v) { instead of the line below
    for(int item : v) { 
        if(!b.insert(item)) {
            cout << "Incorrect bool return value when inserting " << item 
                 << endl;
            return -1;
        }
    }

    /* Test size. */
    cout << "Size is: " << b.size() << endl;
    if(b.size() != v.size()) {
        cout << "... which is incorrect." << endl;
        return -1;
    }

    /* Test find return value. */
    // Test the items that are already in the tree
    for(int item : v) {
        if(!b.find(item)) {
            cout << "Incorrect return value when finding " << item << endl;
            return -1;
        }
    }

    /* Test find smallest value. */
    /* This test will loop until b has no left child node, at which point b 
       contains the minumum value of the BST. */
/*    BSTNodeInt* curr = b.getRoot();
    while( curr->left ) {
        curr = curr->left;
    }
    if( curr->data != -33 ) {
        cout << "Incorrect value in minimum value location" << endl;
        return -1;
    }
 
*/
    /* Test find largest value. Loops until b has no r child node at which
     * point b contains the maximum value of the BST. */
/*    curr = b.getRoot();
    while( curr->right ) {
        curr = curr->right;
    }
    if( curr->data != 100 ) {
        cout << "Incorrect value in maximum value location" << endl;
        return -1;
    }
*/
    
    /* Test find node in empty list, should return false. */
    if( emptyBST.find(0) ) {
        cout << "The find function shouldn't have returned true" << endl;
        return -1;
    }   


    /* Test insert duplicate. Any duplicates should return false. */ 
    if( b.insert(-33) ) {
        cout << "Duplicate entry should have returned false" << endl;
        return -1;
    }


    /* Test inserting multiple duplicates. v2 contains 2 sets of duplicates.
     */    
    for( int item : v2 ) {
        if(b2.find(item)) {
            if(b2.insert(item)) {
                cout << "Duplicate entries not allowed." << endl;
                return -1;
            }
        }
        else {
            b2.insert(item);
        }
    } 


    /* Testing height of BST */
    if( b.height() != 3 ) {
        cout << "Height of " << b.height() << " for BST is wrong" << endl;
        return -1;
    }
    if( b2.height() != 1 ) {
        cout << "Height of " << b2.height() << " for BST is wrong"<< endl;
        return -1;
    }
    if( emptyBST.height() != 0 ) {
        cout << "Height of " << emptyBST.height()<<" for empty BST is wrong\n";
        return -1;
    }


    /* Testing if BST is empty */
    if(!emptyBST.empty()) {
        cout << "emptyBST should return empty!" << endl;
        return -1;
    }
    if(b.empty()) {
        cout << "b should not return empty!" << endl;
        return -1;
    }
    if(b2.empty()) {
        cout << "b2 should not be empty!" << endl;
        return -1;
    }


    /* Testing that smaller elements go to left child */
/*    curr = b.getRoot();
    while(curr->left) {
        if(curr->left->data < curr->data) {
            curr = curr->left;
        }
        else {
            cout << "Values are incorrectly placed in BST" << endl;
            return -1;
        }
    }

*/
    /* Testing that larger elements go to right child */
/*    curr = b2.getRoot();
    while(curr->right) {
        if(curr->data < curr->right->data) {
            curr = curr->right;
        }
        else {
            cout << "Values are incorrectly placed in BST" << endl;
            return -1;
        }
    }    

*/
    /* TEMPLATED BST TESTS */

    
    // Test the template version of the BST  with ints 
    BST<int> btemp;
    for (int item : v) {
        // The auto type here is the pair of BSTIterator<int>, bool
        cout << "Inserting " << item << " into the int template-based BST...";
        auto p = btemp.insert(item);
        if (*(p.first) != item) {
            cout << "Wrong iterator returned.  "
                 << "Expected " << item << " but got " << *(p.first) << endl;
            return -1;
        }
        if (!p.second) {
            cout << "Wrong boolean returned.  Expected true but got " 
                 << p.second << endl;
            return -1;
        }
        cout << "success!" << endl;              
    }

    // Now test finding the elements we just put in
    for (int item: v) {
        cout << "Finding " << item << "...." << endl;
        BSTIterator<int> foundIt = btemp.find(item);
        if (*(foundIt) != item) {
            cout << "incorrect value returned.  Expected iterator pointing to "
                 << item << " but found iterator pointing to " << *(foundIt) 
                 << endl;
            return -1;
        }
        cout << "success!" << endl;
    }

    // Test the iterator: The iterator should give an in-order traversal
  
    // Sort the vector, to compare with inorder iteration on the BST
    sort(v.begin(),v.end());

    cout << "traversal using iterator..." << endl;
    auto vit = v.begin();
    auto ven = v.end();

    // This is equivalent to BSTIterator<int> en = btemp.end();
    auto en = btemp.end();

    //This is equivalent to BST<int>::iterator it = btemp.begin();
    auto it = btemp.begin();
    for(; vit != ven; ++vit) {
        if(! (it != en) ) {
            cout << *it << "," << *vit 
                 << ": Early termination of BST iteration." << endl;
            return -1;
        }

        cout << *it << endl;
        if(*it != *vit) {
            cout << *it << "," << *vit 
                 << ": Incorrect inorder iteration of BST." << endl;
            return -1;
        }
        ++it;
    }
    cout << "success!" << endl;

    BST<int> btest;
    // test that duplicates aren't inserted
    auto returned = btemp.insert(-33);
    if(returned.second) {
      cout << "Duplicate elements are not allowed" << endl;
      return -1;
    }

/* 
    // Testing that every left child is less than parent node
    BSTNode<int>* curr2 = btemp.getRoot();
 
    // iterate down left side of tree
    while(curr2->left) {
      if(curr2->left->data < curr2->data) {
        curr2 = curr2->left;
      }
      else {
        cout << "Ordering of BST is incorrect!" << endl;
        return -1;
      }
    }


    // Testing that every right child is greater than parent node
    curr2 = btemp.getRoot();
    
    // iterate down right side of tree
    while(curr2->right) {
      if(curr2->right->data > curr2->data) {
        curr2 = curr2->right;
      }
      else {
        cout << "Ordering of BST is incorrect!" << endl;
        return -1;
      }
    }
*/
    // Test that nothing is returned from empty BST
    BST<int> emptyTemplatedBST;
    auto emptyReturn = emptyTemplatedBST.find(0);    

    if(emptyReturn != NULL) {
      cout << "Should have returned a null value" << endl;
      return -1;
    }

    // Test that emptyTemplatedBST.empty() returns true
    if(!emptyTemplatedBST.empty()) {
      cout << "Should have returned true" << endl;
      return -1;
    }
 
    
    cout << "All tests passed!" << endl;
    return 0;
}
