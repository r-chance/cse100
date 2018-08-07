/*
 * DisjointSet.cpp
 * Author: Robert Chance & Dorsadaf Moinzad
 * Date:   Mar 10, 2017
 *
 * This file provides implementation for a disjoint set to be used in a 
 * union-find search in the actorconnections file.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "DisjointSet.h"
#include "ActorNode.h"
#define NNNN 9999

using namespace std;


/* Constructor */
DisjointSet::DisjointSet(void) {}



/* 
 * This function takes only a file name containing the actor/movie
 * data.  The function will create a forest of single ActorNode* trees and a
 * vector containing all of the movies. Used for actorconnections.
 */ 
bool DisjointSet::loadFromFile(const char* in_filename) {

  // Initialize the file stream
  ifstream infile(in_filename);
  bool have_header = false;// to skip reading the header

  // keep reading lines until the end of file is reached
  while(infile) {
    
    string s;

    if(!getline(infile,s)) break;// break upon EOF

    // skip reading the header for first iteration
    if(!have_header) {
      have_header = true;
      continue;
    }
    
    istringstream ss(s);// implement stream object for string
    vector <string> record;// store actor/movie/year info

    // Load a single actor/movie/year line from file
    while(ss) {
      string next;
      // get the next string before hitting a tab character and put it in 'next'
      if(!getline( ss, next, '\t' )) break;

      record.push_back(next);
    }
    
    if(record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year(stoi(record[2]));

    ActorNode* actorA = new ActorNode(actor_name);
    MovieNode* movieA = new MovieNode(movie_title, movie_year);

    ActorNode* tempA;
    MovieNode* tempM;

    // Check if an actor Already exists
    if( (tempA = actorSet[actor_name]) ) {
      // Delete actorA node if already in map
      delete(actorA);
    }
    else {
      // Insert actor if not already in map
      actorSet[actor_name] = actorA;
      tempA = actorA;
    }

    // Check if movie already exists
    if( (tempM = movies[(movieA->getTitle())+to_string(movieA->getYear())]) ) {
      delete(movieA);
      movieA = nullptr;
    }

    else {
      movies[(movieA->getTitle())+to_string(movieA->getYear())] = movieA;
      tempM = movieA;
    }

    tempM->actors.push_back(tempA);
  }
 
  // If file ends loop and is NOT at EOF.
  if(!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  // Close input file
  infile.close();

  // Sorts the movies vector by year
  for(auto it = movies.begin(); it != movies.end(); it++) {
    movieVec.push_back(it->second);
  }
  sort(movieVec.begin(), movieVec.end(), VecComp());

  // Return true to indicate file successfully loaded
  return true;
}



/*
 * This function will read in from a file containing a list of actor pairs.
 * For each actor pair, where both actors are contained in the data set,
 * this function will call unionFind, and write the output to a file.
 */
void DisjointSet::connectPairs(const char* in_filename, ofstream &out) {
  // Initialize the file stream
  ifstream infile(in_filename);
  // Check if the file is successfully opened
  if (!infile.is_open()) {
    cerr << "Problem opening the file" << endl;
    return;
  }

  // Read the file
  bool have_header = false;  // Check if the header is processed

  // Print header to output file
  out << "Actor1\tActor2\tYear\n"; // print output file header

  // Keep reading lines of actor pairs until the eof is reached
  while(infile) {

    string s; // Line of input
    if(!getline(infile, s))  break;

    if(!have_header) {
      // Check header and skip the header
      have_header = true;
      continue;
    }

    istringstream ss(s);
    vector<string> record;  // Line information

    // Read in actor names from this line
    while(ss) {

      string next;
      // Get the next string before hitting a tab charac and put
      // it into next
      if(!getline(ss, next, '\t')) break;
		 
      record.push_back(next);  // Obtain a field of data
    }
	
    // Check the column count, there should be 2
    if(record.size() != 2) continue;

    // processing the line of input
    string actorA(record[0]);
    string actorB(record[1]);

    // Get new pair of actors if one or both actors are not found in the data
    if( !(actorSet[actorA] && actorSet[actorB]) ) continue;

    // Output actors and year they are connected to file
    out << actorA << "\t" << actorB << "\t" 
        << to_string(unionFind(actorA, actorB));
     
    if(!infile.eof()) out << "\n";
  }
  infile.close();
}



/* This function takes in two actor names. It will call merge set after each
 * movie is added, and after each year of movies, it will search if the pair
 * of actors belong to the same set. If so, the year they merged is returned.
 * The output if both actors never merge into the same set is 9999.
 */
int DisjointSet::unionFind(string& actorAName, string& actorBName) {

  // Create pointers to ActorNodes
  ActorNode* actorA = actorSet[actorAName];
  ActorNode* actorB = actorSet[actorBName];

  // Keep track of movie iterators, it2 progresses until next movie year,
  // it1 iterates over all movies in this year.
  auto it1 = movieVec.begin(); 
  auto it2 = movieVec.begin();

  // Variable to store movie year that is being analyzed 
  int earliestYear = (*it1)->getYear();

  MovieNode* temp;
 
  // Loop while it2 hasn't reached the end of the vector
  while(it2 != movieVec.end()) {

    // Advance it2 to next movie year
    while(earliestYear == (*it2)->getYear()) {
      it2++;
      // Check if the it2 hasn't reached the end
      if (it2 == movieVec.end()) {
        break;
      }
    }

    // Loop until year of it1 is equal to year of it2, adding movies with each
    // iteration
    while(it1 != it2) {

      temp = *it1; // Set temp to MovieNode pointed to by it1

      // Merge sets for all actors that played in temp movie
      for(unsigned int i = 1; i < temp->actors.size(); i++) {
        mergeSet(temp->actors[i],temp->actors[i-1]);
      }

      // Advance it1 to next movie
      it1++;
    }

    // Check if actors A and B are connected after this movie year insertion
    // Return year if connected, add next year of movies if not
    if(find(actorA)->getName() == find(actorB)->getName()) {
 
      // Reset parent pointers in all Actor nodes
      for(auto it = actorSet.begin(); it != actorSet.end(); it++) {

        (it->second)->setParent(nullptr);
      }
      return temp->getYear();
    }
	
    // Set the earliest year
    if(it2 != movieVec.end()) 
      earliestYear = (*it2)->getYear(); 
  } 	  
  
  return NNNN;
}



/* This function takes two actorNodes and merges the sets to which they 
 * belong.  Using path compression, it will optimize the disjoint set for
 * finding sentinel nodes. Actor A's sentinel node will be pointing to actor 
 * B's sentinel node.
 */
void DisjointSet::mergeSet(ActorNode* actorA, ActorNode* actorB) {

  // Create ActorNode to store sentinel nodes
  ActorNode* sentinelA = nullptr;
  ActorNode* sentinelB = nullptr;

  // Find sentinel node of actor A
  sentinelA = find(actorA);
  // Find sentinel node of actor B
  sentinelB = find(actorB);

  // Assign sentinelA to point to sentinelB, as long as both aren't already
  // in same set
  if(!(sentinelA->getName() == sentinelB->getName())) {
    sentinelA->setParent(sentinelB);
  }
  
}




/* This function takes in an actorNode and returns its sentinel node,
 * will also use path compression during finding.
 */
ActorNode* DisjointSet::find(ActorNode* actor) {

  // Create temp variable for ActorNode
  ActorNode* temp = actor;

  // Create a vector to store parent pointers of actorNodes.  This will be
  // used for path compression. Each pointer will be reassigned to the
  // sentinel node's pointer
  vector<ActorNode*> parentPointers;

  // Iterate through links of parent nodes until parent is null, at which
  // point, that node containing that null parent is the sentinel
  while(temp->getParent()) {
    // add temp to vector of parent points
    parentPointers.push_back(temp);

    // set temp equal to parent
    temp = temp->getParent();
  }

  // Assign all pointers in parentPointers to point to temp
  for(ActorNode* a: parentPointers) {
    a->setParent(temp);
  }

  // return sentinel node
  return temp;
}



/* This is a destructor for the DisjointSet class */
DisjointSet::~DisjointSet() {

  // Delete all ActorNodes
  for(auto it = actorSet.begin(); it != actorSet.end(); it++) {
    delete it->second;
  }

  // Delete all MovieNodes
  for(auto it = movieVec.begin(); it != movieVec.end(); it++) {
    delete *it;
  }
}



/* This is a comparator for sorting movie years in a vector 
 */
bool VecComp::operator()(const MovieNode* a, const MovieNode* b) {
  return (a->getYear() < b->getYear());
}


/*
 * Starts the timer. Saves the current time.
 */
void Timer::begin_timer()
{
    
  start = std::chrono::high_resolution_clock::now();
}

/*
 * Ends the timer. Compares end time with the start time and returns number of
 * nanoseconds
 */
long long Timer::end_timer()
{
    
  std::chrono::time_point<std::chrono::high_resolution_clock> end;
  end = std::chrono::high_resolution_clock::now();
    
  return (long long)std::chrono::duration_cast<std::chrono::
                                 nanoseconds>(end - start).count();
}

