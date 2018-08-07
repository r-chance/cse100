/*
 * DisjointSet.h
 * Authors: Robert Chance and Dorsadaf Moinzad
 * Created 3/10/17
 */

#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <chrono>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include "ActorNode.h"
#include <sstream>

using namespace std;



/* Comparator class to sort movie vector */
class VecComp {
public:
  bool operator()(const MovieNode* a, const MovieNode* b);
};



/* Timer class to record runtimes of searches */
class Timer {
private:
      std::chrono::time_point<std::chrono::high_resolution_clock> start;
    
public:    
    /*
     * Function called when starting the timer.
     */
    void begin_timer();
    
    /*
     * Function called when ending the timer. Returns duration in nanoseconds
     * PRECONDITION: begin_timer() must be called before this function
     */
    long long end_timer();
};



/* This class provides the structure for a disjoint set of actors, which can
 * be analyzed using union and find operations.  This class uses path
 * compression during finding and inserting.
 */
class DisjointSet {
protected:
  // Container that holds the ActorNode set
  unordered_map<string, ActorNode*> actorSet;
  // Container that holds the MovieNode set and backing container for
  // disjoint set
  unordered_map<string, MovieNode*> movies;
  // Movie vector to be sorted by year
  vector<MovieNode*> movieVec;
  
public:
  
  // Constructor and Destructor
  DisjointSet(void);
  ~DisjointSet();

  /* This function will take input from a .tsv file and generate a disjoint
   * set of ActorNodes.  The function will read one line in at a time from
   * the tab-delimited file and generate an ActorNode for each unique actor
   * that is read in from the file.  It will also create a MovieNode for each
   * unique movie, and insert them into a hash map.  When all the movies are 
   * entered, the MovieNodes are then sorted into an array by year.
   */
  bool loadFromFile(const char* in_filename);
  
  /* This function will read in from a file containing a list of actor pairs.
   * For each actor pair, where both actors are contained in the data set,
   * this function will call unionFind. Output is written to ofstream out.
   */
  void connectPairs(const char* in_filename, ofstream &out);


  /* This function takes in two actor names. It will call mergeSet on each
   * pair of actors and return the earliest year they became part of the same
   * set. The output if both actors never merge into the same set is 9999.
   */
  int unionFind(string&, string&);

  /* 
   * This function will take two actor nodes and merge the sets to which they
   * belong.  Using path compression, it will then determine if both actors
   * share the same sentinel node, i.e. the actors were merged into the same
   * set.  If so it will return true, if not it returns false.
   */
  void mergeSet(ActorNode*, ActorNode*);

  /* 
   * This function will take an actor node and return its sentinel node.
   * It will also use path compression as it does this.
   */
  ActorNode* find(ActorNode*);

};  

#endif
