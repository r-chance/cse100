#include "util.h"
#include "DictionaryBST.h"


/* Create a new Dictionary that uses a BST back end */
DictionaryBST::DictionaryBST(){}

/* Insert a word into the dictionary. */
bool DictionaryBST::insert(std::string word){
   
  // check for empty string 
  if(word != "") {

    // check for space at beginning or end of word
    if( word[0] == ' ' || word[word.length()-1] == ' ' ) {
      return false;
    }

    // check for uppercase
    for (int i = 0; i < (int) word.length(); i++ ) {
      if( (word[i] < 'a' || word[i] > 'z') && word[i] != ' ') 
        return false;
    }

    if(dictBST.find(word) == dictBST.end()) {
      dictBST.insert(word);
      return true;
    }
  }
  return false;
}
	
/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryBST::find(std::string word) const
{
  if(word != "") {
    if(dictBST.find(word) != dictBST.end()) {
      return true;
    }
  }
  return false; 
}

/* Destructor */
DictionaryBST::~DictionaryBST(){
}


