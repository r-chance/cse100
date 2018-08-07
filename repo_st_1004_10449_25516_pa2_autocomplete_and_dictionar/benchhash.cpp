#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <unordered_set>
#include <vector>

#define A 54059 // a prime
#define SIZE_PARAM 2 // param arg for table size
#define FILENAME_PARAM 1 // param arg for filename
#define TOTAL_PARAM 3
#define TABLE_CONST 2

using namespace std;


// Hash function #1
// SOURCE: http://stackoverflow.com/questions/8317508/hash-function-for-a-string
unsigned int benchhash1(string& word, unsigned int tableSize)
{ 
  unsigned long hash = 0;
  for(int i = 0; i < word.length(); i++)
  {
    hash = (hash * A ) + word[i];
  }
  return hash % tableSize;
} 


// Hash function #2
// SOURCE: https://www.daniweb.com/programming/software-development/
//         threads/231987/string-hash-function
unsigned int benchhash2(string & key, unsigned int tableSize) {

  unsigned long hashVal = 0;
	
  for (int x = 0; x < key.length(); ++x) {
    hashVal ^= (hashVal << 5) + (hashVal >> 2) + key[x];
  }

  return hashVal % tableSize;
}


// This function prints the number of collisions for each bucket from using
// a given hash function.
void printTable(std::vector<int> & v, int hashNum) {
 
  int collisions;// stores number of collisions each bucket received
  int bucketsCounted = 0;// tracks how many elements have been counted, should
                         // be equal to size of table upon completion
  int value = 0;
  int avg = 0; // stores value*collisions, used for finding avg steps to search
  int dataSize = v.size()/TABLE_CONST;

  // Table header
  cout << "Printing the statistics for hashFunction1 with hash table size "
       << dataSize << endl;
  cout << "#hits    #slots receiving that #hits" << endl;
  while(bucketsCounted != v.size()) {
  
    collisions = 0;
    for(int i = 0; i < v.size(); i++) {

      if(v[i] == value) {
        collisions++;
        bucketsCounted++;
      }
    }
    // print a new line to table if there are collisions
    if(collisions != 0) {
      cout << value << "        " << collisions << endl;
    }
   
    // Add up num of steps to find element
    for( int j = value; j > 0; j-- ) {

      avg += j*collisions;
    }
      value++;
  }
  cout << "The average number of steps for a successful search for hash "
       << "function " << hashNum << " would be " << (double)avg/dataSize
       << endl;
  cout << "The worst case steps that would be needed to find a word is "
       << value-1 << endl << endl;
}



// Main function, inputs a file, reads and hashes lines, stores the number of
// collisions to a vector, representing a chained hash table
int main (int argc, char* argv[]) {

  // validate parameters
  if(argc != TOTAL_PARAM) {
    cout << "Incorrect parameters.  Usage is ./benchhash <inputfile.txt> "
         << "num_param" << endl;
    return -1;
  }

  int size_param = stoi(argv[SIZE_PARAM]);
  int tableSize = TABLE_CONST*size_param;
  string fileName = argv[FILENAME_PARAM];
  unsigned int h1;// Hash value from hash function 1
  unsigned int h2;// Hash value from hash function 2
   
  vector<int> hits1(tableSize,0);// 2 int vectors, each element initialized to
                                 // 0, used to store the number of hits each
  vector<int> hits2(tableSize,0);// hash table would have received

  // Open the dictionary file
  std::ifstream fs;
  fs.open(fileName,ifstream::in);
   
  string s;// string variable to store lines from file
  int character = 0;// stores character from string s

  // get lines from file, hash and record hits for each container
  for( int i = 0; i < size_param; i++ ) {
   
    getline(fs, s);// get a line from file, save it to string, s
  
    // eof check
    if(fs.eof()) {
      cout << "Only read in " << i+1 << " lines from file" << endl;
      break;
    }

    // remove freq from beginning of string
    int j = 0;// index of string
    do {
      character = s[j];
      j++;}
    while( character != ' ' );
    s = s.substr(j,s.length());// get substring of s without freq preceeding
     
    // get hash values of s, using 2 different hash functions
    h1 = benchhash1(s, tableSize);
    h2 = benchhash2(s, tableSize);
 
    // increment hits at hash value location in vectors
    hits1[h1]++;
    hits2[h2]++;
  }

  // Print data regarding hash collisions for both hash functions
  printTable(hits1, 1);
  printTable(hits2, 2);

return 0;
}
