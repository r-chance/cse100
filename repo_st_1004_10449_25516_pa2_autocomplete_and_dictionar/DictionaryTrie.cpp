#include "util.h"
#include "DictionaryTrie.h"

using namespace std;

unsigned int character; // used for parsing individual string characters

/* Create a new Dictionary that uses a Trie back end */
DictionaryTrie::DictionaryTrie(){
  
  /* Allocate new node */
  root = new Node;
}

/* Insert a word with its frequency into the dictionary.
 * Return true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string) */
bool DictionaryTrie::insert(std::string word, unsigned int freq)
{ 
  
  Node* curr = root;
  Node* wordNode;

  /* checks for empty string or already existed one */  
  if(word == "") 
    return false;

  if(!validateInput(word)) {
    
    cout << "Invalid input. Please try again." << endl;
    return false;
  }

  /* Update frequency of word if already existing in dictionary */
  if((wordNode = find(word))) {
  
    /* Update frequency */
    if( wordNode->freq < freq ) {
      wordNode->freq = freq;
    }
    return false;
  }
  
  curr = root;

  /* Insert word to dictionary */
  unsigned int i = 0;
  for( ; i < word.length(); i++ ) {
    character = word[i];

    // assign character to vector mapping
    if(character == ' ') 
      character = ALPHASIZE_;
    else 
      character -= 'a';

    if( curr -> children[character] != nullptr) {    
      curr = curr->children[character];
      continue;
    }
    else {
      curr -> children[character] = new Node;
      curr = curr->children[character];
    }
  }
  // update word node, frequency, and return true
  curr->isWord = true;
  curr->freq = freq;
  
  return true;
}

/* Return true if word is in the dictionary, and false otherwise */
DictionaryTrie::Node* DictionaryTrie::find(std::string word) const
{
  /* Check for validitity of the word */
  string s = word;

  if(!validateInput(s)) {
    return nullptr;
  }

  /* Holds the current node */	
  Node* curr = root;

  /* Check if word is duplicate entry */
  for( unsigned int i = 0; i < word.length(); i++ ) {
    // setting index of dictionary vector to mapped element
    character = word[i];
    if(character == ' ') 
      character = ALPHASIZE_;
    else 
      character -= 'a';

    // Check if character exists in trie
    if( curr->children[character] != nullptr ) {
      // if loop is at end of word
      if( i == word.length()-1 ) {
        // if curr is a word node
        if( curr->children[character]->isWord ) {
          return curr->children[character];
        }
        // if end of word and not a word node
        else return nullptr;
      }
      // if loop is not at end of word, update and continue
      else {
        curr = curr->children[character];
        continue;
      }
    }
    // If no edge to next letter, not found
    else return nullptr;
  }
  return nullptr;
}
/* Search through the subtree, rooted at the end of the prefix
 * performing a depth first search algorithm. starting from the given
 * node, construct all possible strings
 * when a word node is found, insert it to the map. Add the freq
 */
void DictionaryTrie::findAllCompletions(Node* node, set<pair<unsigned int, 
                                        string>>& result, string s ){

  // allocate mem for pair on the stack
  std::pair<unsigned int, string> p;

  // Search through the subtree, performing DFS
  for(int character = 0; character <= ALPHASIZE_; ++character) {
    // Validity checks
    if(node->children[character] != nullptr) {
      if(character == ALPHASIZE_ )
        s += ' ';
      else
	s += (character + 'a');
	  
      // If this is the word node 
      if(node->children[character]->isWord) {

      // add this word to the list
      p.first = node->children[character]->freq;
      p.second = s;
      result.insert(p);
      }
      
      // Recursive call to the findAllComplesions using DFS
      findAllCompletions(node->children[character], result, s);
 
      // Remove the last character after the completion of the recursive
      // call
      s.pop_back();
    }
  }
}

/* Return up to num_completions of the most frequent completions
 * of the prefix, such that the completions are words in the dictionary.
 * These completions should be listed from most frequent to least.
 * If there are fewer than num_completions legal completions, this
 * function returns a vector with as many completions as possible.
 * If no completions exist, then the function returns a vector of size 0.
 * The prefix itself might be included in the returned words if the prefix
 * is a word (and is among the num_completions most frequent completions
 * of the prefix)
 */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                   unsigned int num_completions) 
{
  // counts the number of completions
  unsigned int count = 0;

  /* Define a set that contains all the possible strings that can be
     constructed from the given prefix, ordered by their freq */
  std::set<pair<unsigned int, string>> myset;

  // create a new pair for inserting data to set
  std::pair<unsigned int, string> p;
  
  // Result  
  std::vector<std::string> words;

  // The empty prefix must return and empty vector
  if(prefix.empty())
    return words;
  if(!validateInput(prefix)) {
    return words;
  }

  // Holds the current node
  Node* curr = root;

  // Check for empty trie, if empty, return empty vector
  if( !curr ) return words;

  // Iterate through prefix in trie
  for(unsigned int i = 0; i < prefix.length(); i++ ) {
	
    // Setting index of dic vector to mapped elts
    character = prefix[i];

    if(character == ' ')
      character = ALPHASIZE_;
    else
      character -= 'a';

    // Check if character exists in trie
    if(curr->children[character] != nullptr ) {
      curr = curr->children[character];
    }
    // If no edge to next letter, not found, return empty vector
    else 
      return words;
  }

  // If the prefix is a word, put it into the set
  if(curr->isWord){
    p.first = curr->freq;
    p.second = prefix;
    myset.insert(p);   
  }

  /* Construct the set with all possible strings that can be constructed
   * with the given prefix */
  findAllCompletions(curr, myset, prefix);


  /* Go through the set in reverse order, to have the string listed from
   * most frequent to least. Then return up to num_completions strings */
  for(auto it = myset.rbegin(); it != myset.rend() &&
                count < num_completions; ++it ) {
 
    // insert words sorted by highest frequency to words vector 
    words.push_back(it->second);
    count++;
  } 

  // return words vector, will contain number of words equal to 
  // num_completions, sorted in order of highest frequency to lowest
  return words;
}

/* Destructor */
DictionaryTrie::~DictionaryTrie(){
  deleteAll(root);
}

/* Helper function for destructor */
void DictionaryTrie::deleteAll(Node* n) {
 
  for (Node* item : n->children) {
    if(item != nullptr) {
      deleteAll(item);
    }
  }
  delete n;
}

/* Helper function for checking input validity */
bool DictionaryTrie::validateInput(string s) const {

  /* Validate prefix: check for chars outside of a-z and space */
  for(unsigned int i = 0; i < s.length(); i++) {
    if( (s[i] < 'a' || s[i] > 'z') && s[i] != ' ') {
      return false;
    }
    if(s[i] == ' ' && (i == 0 || i == s.length()-1) ) {
      return false;
    }
  }
  return true;
}
