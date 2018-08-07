#include "util.h"
#include "DictionaryHashtable.h"

/* Create a new Dictionary that uses a Hashset back end */
DictionaryHashtable::DictionaryHashtable(){}

/* Insert a word into the dictionary. */
bool DictionaryHashtable::insert(std::string word)
{
  // check for empty string
  if (word != "") {

    // check for spaces at beginning or end of word
    if( word[0] == ' ' || word[word.length()-1] == ' ' ) {
      return false;
    }    

    for ( int i = 0; i < (int) word.length() ; i++ ) {
      if( (word[i] < 'a' || word[i] > 'z') && word[i] != ' ' ) {
        return false;
      }
    }

    if (dicHash.find(word) == dicHash.end() ) {
      dicHash.insert(word);
      return true;
    } 
  }
  
  return false;
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryHashtable::find(std::string word) const
{  
 /* checks if the word is valid */ 
 if (word != "") {
   if (dicHash.find(word) != dicHash.end())
     return true;
 }
 return false;	
}
/* Destructor */
DictionaryHashtable::~DictionaryHashtable(){}
