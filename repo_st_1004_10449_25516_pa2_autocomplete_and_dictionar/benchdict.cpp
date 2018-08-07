#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <functional>

// input args
#define MIN_SIZE 1
#define STEP_SIZE 2
#define NUM_ITERATIONS 3
#define DICTFILE 4
#define FIFTEEN 15
#define A_HUNDRED 100
#include "DictionaryBST.cpp"
#include "DictionaryHashtable.cpp"
#include "DictionaryTrie.cpp"
#include "util.h"
#include "util.cpp"


using namespace std;

// This program will analyze the run time of worst case finds in a BST based
// dictionary, Hashtable based dictionary, and Multiway Trie based dictionary.
int main(int argc, char* argv[]) {

  unsigned int min_size = stoi(argv[MIN_SIZE]);// starting size of dict
  unsigned int step_size = stoi(argv[STEP_SIZE]);// value to increment
  unsigned int num_iterations = stoi(argv[NUM_ITERATIONS]);// # of tests
  string dictfile = argv[DICTFILE];// name of dictionary text file
  string s;
  int character;
  long long run_time = 0;

  // open file stream to text file
  ifstream fs;
  fs.open(dictfile,ifstream::in);

  Timer* t = new Timer();
 
  // Benchmarking DictionaryBST
  cout << "Benchmarking DictionaryBST" << endl;

  // Test run times with increasingly large dictionaries
  for( int i = 0; i < num_iterations; i++ ) {

    run_time = 0;
    
    // reset file pointer
    fs.clear();
    fs.seekg(0);
    DictionaryBST* d = new DictionaryBST();
    Utils::load_dict(*d,fs,min_size+i*step_size);
    
    // do multiple tests to average data with
    for( int j = 0; j < FIFTEEN; j++ ) {

      t->begin_timer();

      // get 100 lines from dictionary file, search dict object for lines
      for( int k = 0; k < A_HUNDRED; k++ ) {
        getline(fs,s);
        int index = 0;
        do {
          character = s[index];
          index++;
        }
        while( character != ' ');
        s = s.substr(index,s.length());
        d->find(s);
      }
    
      run_time += t->end_timer();
    }

    cout << min_size+i*step_size << "\t" << (double)run_time/FIFTEEN << endl;
    delete(d);
  }
 
  // Benchmarking DictionaryHashtable
  cout << "Benchmarking DictionaryHashtable" << endl;

  for( int i = 0; i < num_iterations; i++ ) {

    run_time = 0;
    fs.clear();
    fs.seekg(0);
    DictionaryHashtable* d = new DictionaryHashtable();
    Utils::load_dict(*d,fs,min_size+i*step_size);
    
    for( int j = 0; j < FIFTEEN; j++ ) {

      t->begin_timer();
      for( int k = 0; k < A_HUNDRED; k++ ) {
        getline(fs,s);
        int index = 0;
        do {
          character = s[index];
          index++;
        }
        while( character != ' ');
        s = s.substr(index,s.length());
        d->find(s);
      }
    
      run_time += t->end_timer();
    }

    cout << min_size+i*step_size << "\t" << (double)run_time/FIFTEEN << endl;
    delete(d);
  }

  // Benchmarking DictionaryTrie
  cout << "Benchmarking DictionaryTrie" << endl;

  for( int i = 0; i < num_iterations; i++ ) {

    run_time = 0;
    fs.clear();
    fs.seekg(0);
    DictionaryTrie* d = new DictionaryTrie();
    Utils::load_dict(*d,fs,min_size+i*step_size);
    
    for( int j = 0; j < FIFTEEN; j++ ) {

      t->begin_timer();
      for( int k = 0; k < A_HUNDRED; k++ ) {
        getline(fs,s);
        int index = 0;
        do {
          character = s[index];
          index++;
        }
        while( character != ' ');
        s = s.substr(index,s.length());
        d->find(s);
      }
    
      run_time += t->end_timer();
    }

    cout << min_size+i*step_size << "\t" << (double)run_time/FIFTEEN << endl;
    delete(d);
  }
  delete(t);
} 
