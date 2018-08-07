#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<set>
#include<cstdlib>
#include "util.h"
#include "DictionaryTrie.h"
#include "DictionaryBST.h"
#include "DictionaryHashtable.h"
#define LETTERS 26
using namespace std;

int main(int argc, char** argv)
{

  //Initialize words
  vector<std::string> words;
  vector<std::string> ourWords;
  vector<string>::iterator wit;
  vector<string>::iterator wen;
  vector<string>::iterator ourIt;
  //initialize nonwords
  set<string> nope;
  set<string>::iterator nit;
  set<string>::iterator nen;

  //Initialize data structures
  DictionaryBST d_bst;
  DictionaryHashtable d_ht;
  DictionaryTrie dt;
  int t_bst, t_ht, tt;

  words.push_back("harry");
  words.push_back("sriram");
  words.push_back("cse");
  words.push_back("crucio");
  words.push_back("autocomplete");
  words.push_back("basketball");
  words.push_back("basket");
  words.push_back("all your free time");
  words.push_back("we did it");


  cout << "Inserting into Dictionaries..." << endl;

  wit = words.begin();
  wen = words.end();
  for(; wit != wen; ++wit)
    {
      cout << "Inserting: \"" << *wit << "\"... ";
      t_bst = d_bst.insert(*wit);
      t_ht = d_ht.insert(*wit);
      tt = dt.insert(*wit, 1);
      //cout << t_bst << " " << t_ht << " "<< tt << "... ";
      if(!t_bst)
	{
	  cout << "failed for DictionaryBST... ";
	}
      if(!t_ht)
	{
	  cout << "failed for DictionaryHashset... ";
	}
      if(!tt)
	{
	  cout << "failed for DictionaryTrie... ";
	}
      if(t_bst && t_ht && tt)
	{
	  cout << "PASSED! :D ";
	}
      cout << endl;
    }

  cout << endl << "Re-inserting elements that were just inserted into "
       << "Dictionaries..." << endl;

  wit = words.begin();
  wen = words.end();
  for(; wit != wen; ++wit)
    {
      cout << "Inserting: \"" << *wit << "\"... ";
      t_bst = d_bst.insert(*wit);
      t_ht = d_ht.insert(*wit);
      tt = dt.insert(*wit, 0);
      if(t_bst)
        {
          cout << "failed for DictionaryBST... ";
        }
      if(t_ht)
        {
          cout << "failed for DictionaryHashset... ";
        }
      if(tt)
        {
          cout << "failed for DictionaryTrie... ";
        }
      if(!t_bst && !t_ht && !tt)
        {
          cout << "PASSED! :D ";
        }
      cout << endl;
    }

  cout << endl;
 
  // stores return values from tests
  int dd, ddBST, ddHT;

  // To test various cases
  ourWords.push_back(" word");
  ourWords.push_back("A");
  ourWords.push_back("word ");
  ourWords.push_back("word");
  ourWords.push_back("a shot in the dark");
  ourWords.push_back("a shot");
  
  ourIt = ourWords.begin();

  /* Testing space preceeding word is invalid */
  ddBST = d_bst.insert(*ourIt);
  ddHT = d_ht.insert(*ourIt);
  dd = dt.insert(*ourIt, 1);
  if(dd || ddHT || ddBST ) {
    cout << "Invalid space at beginning of word...Failed!" << endl;
  }
  else 
    cout << "Inserting pre-space...Test Passed!" << endl;


  /* Check for upper case letter */
  ++ourIt;
  dd = dt.insert(*ourIt,1);
  ddHT = d_ht.insert(*ourIt);
  ddBST = d_bst.insert(*ourIt);
  if(dd || ddHT || ddBST) 
    cout << "Please use lower case...Failed!" << endl; 
  else 
    cout << "Inserting upper case...Test Passed!" << endl; 
 
  /* Testing space following word is invalid */
  ++ourIt;
  dd = dt.insert(*ourIt, 1);
  ddHT = d_ht.insert(*ourIt);
  ddBST = d_bst.insert(*ourIt);
  if(dd || ddHT || ddBST) {
    cout << "Invalid space at end of word...Failed!" << endl;
  }
  else
    cout << "Inserting post-space...Test Passed!" << endl;
  
  /* Check that finding a word not in the dictionary returns false */
  ourIt = ++ourIt;
  dd = (dt.find(*ourIt) != nullptr);
//  dd = dt.find(*ourIt);
  ddHT = d_ht.find(*ourIt);
  ddBST = d_bst.find(*ourIt);
  if( dd || ddHT || ddBST ) {
    cout << "Word is not contained in dictionary. Test failed!" << endl;
  }
  else 
    cout << "Testing find on non-existent element...Test Passed!" << endl;
   
  
  /* Check that insert properly stores multiple word lines */
  ++ourIt;
  dd = dt.insert(*ourIt, 1);
  ddHT = d_ht.insert(*ourIt);
  ddBST = d_bst.insert(*ourIt);
  if(!dd && !ddHT && !ddBST) {
    cout << "Inserting multiple word string failed!" << endl;
  }
  else
    cout << "Inserting multiple word string...Test Passed!" << endl;
  
  /* Ensure find returns true only on word nodes */
  ++ourIt;
  dd = (dt.find(*ourIt) != nullptr);
//  dd = dt.find(*ourIt);
  ddHT = d_ht.find(*ourIt);
  ddBST = d_bst.find(*ourIt);
  if( ddHT || ddBST) {
    cout << "Finding non-word node returned true...failed!" << endl;
  }
  else{ 
    cout << "Finding non-word node returned false...Passed!" << endl;
  }

  /* FINAL IMPLEMENTAION TESTING */
 
  // Check the auto-complete function
  DictionaryTrie dictionary;// initialize dictionary data structure
  vector<string> predictions;
  vector<string> expected_predictions, expected_predictions2, 
  expected_predictions3, expected_predictions4;
  
  // Open file and load dictionary
  std::ifstream fs;
  fs.open ("shuffled_freq_dict.txt",std::ifstream::in);
  Utils::load_dict(dictionary, fs, 500);
   
  // Check first 5 words starting with 'in' within first 500 in list
  //  are ordered
  predictions = dictionary.predictCompletions("in", 5);
  expected_predictions = { "infection and", "in writing and", 
  "international celebrity","independent trade", "in her poem"};
  if( predictions == expected_predictions) {
    cout << "Autocomplete function for prefix \"in\"...Test Passed!"  
         << endl;
  }
  else {
    cout << "Autocomplete function for prefix \"in\"...Test failed!"
         << endl;
  }

  // Check that first 12 words starting with 'the' within first 500 in 
  // list are ordered
  predictions = dictionary.predictCompletions("the", 12);
  expected_predictions2 = {"the motto", "the national academy of sciences", 
  "the foundry", "the dust on", "the center of the action",
  "the ability to walk", "the law said", "they imply that",
  "the physical characteristics of the", "the disproportionately",
  "the season we", "the auditory system"};
  
  if(predictions == expected_predictions2) { 
    cout << "Autocomplete function for prefix \"the\"...Test Passed!"<<endl;
  }
  else {
    cout << "Autocomplete function for prefix \"the\"...Test failed!"<<endl;
  }

  // Check that frequency is updated for duplicate words
  dictionary.insert("the auditory system", 1000);
  predictions = dictionary.predictCompletions("the", 12);
  expected_predictions2 = {"the auditory system", "the motto",
  "the national academy of sciences", 
  "the foundry", "the dust on", "the center of the action",
  "the ability to walk", "the law said", "they imply that",
  "the physical characteristics of the", "the disproportionately",
  "the season we"};
  
  if(predictions == expected_predictions2) { 
    cout << "Autocomplete function for prefix \"the\"...Test Passed!"<<endl;
  }
  else {
    cout << "Autocomplete function for prefix \"the\"...Test failed!"<<endl;
  }

  return 0;
}
