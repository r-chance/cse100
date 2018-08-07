/*
 * ConnectionsGraph.cpp
 * Author: Robert Chance & Dorsadaf Moinzad
 * Date:   Feb 26, 2017
 *
 * This file provides implementation for a graph to use for BST search in the
 * actorconnections file.
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
#include "ConnectionsGraph.h"
#include "ActorNode.h"
#include "DisjointSet.h"
#define NNNN 9999

using namespace std;


/* Constructor */
ConnectionsGraph::ConnectionsGraph(void) {}


/* 
 * This function takes only a file name containing the actor/movie
 * data.  The function will create a forest of single ActorNode* trees and a
 * vector containing all of the movies. Used for actorconnections.
 */ 
bool ConnectionsGraph::loadFromFile(const char* in_filename) {

  // list of ActorNodes
  unordered_map<string, ActorNode*> actorSet;
  vector<MovieNode*> movieSet;

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
    if( (tempA = nodeSet[actor_name]) ) {
      // Delete actorA node if already in map
      delete(actorA);
    }
    else {
      // Insert actor if not already in map
      nodeSet[actor_name] = actorA;
      tempA = actorA;
    }

    // Check if movie already exists
    if( (tempM = movieMap[(movieA->getTitle())+to_string(movieA->getYear())]) ) {
      delete(movieA);
      movieA = nullptr;
    }

    else {
      movieMap[(movieA->getTitle())+to_string(movieA->getYear())] = movieA;
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
  for(auto it = movieMap.begin(); it != movieMap.end(); it++) {
    movieVec.push_back(it->second);
  }
  sort(movieVec.begin(), movieVec.end(), VecCompt());

  // Return true to indicate file successfully loaded
  return true;
}




/* Breadth First Search Algorithm, returns empty string if actors are never
 * connected.
 */
string ConnectionsGraph::bfs(string actorAName, string actorBName) {
  // Output string
  string output = "";
  // create a vector of ActorNode pointers to keep track of which nodes need
  // to be reset after a traversal
  vector<ActorNode*> needsReset;

  // Creates a new node for actor's name 
  ActorNode *actorA = nodeSet[actorAName];
  // Creates a null pointer for actor B  
  ActorNode* actorB = nullptr;
  // Set the distance to start from 0
  actorA->setDistance(0);
  // Create a stack to store the tempActor->edges[i]->distance)path traversed 
  // from A to B
  stack<ActorNode*> path;

  priority_queue<ActorNode*,vector<ActorNode*>, PQComparator> pq;

  // push actorA into the priority queue
  pq.push(actorA);
  // add to reset vector to be reset later
  needsReset.push_back(actorA);

  /* If the pq not empty iterate through to find the relationship 
   * between the actors 
   */
  while(!pq.empty()){
    // Get top pq element
    ActorNode* tempActor = pq.top(); 

    // If top pq element was actorB node, stop loop
    if(tempActor->getName() == actorBName) {
      actorB = tempActor;
      break;
    }

    // Pop a node from the pq
    pq.pop();
  
    // If this node has already been visited, discard
    if(tempActor->isDone()) continue;

    // Set this node as visited
    tempActor->setDone(true); 

    // iterate this node's edge list and add all actors with edges
    for(unsigned int i = 0; i < tempActor->edges.size(); i++) {

      // set weight of path to next node
      int weight = tempActor->edges[i]->getDistance();

      // Insert neighboring node if this path is the shortest to it
      if(weight+tempActor->getDistance() < 
         tempActor->edges[i]->getActor()->getDistance()) {

        // add node to vector to be reset after traversal
        needsReset.push_back(tempActor->edges[i]->getActor());
 
        // set distance to of neighbor node
        tempActor->edges[i]->getActor()->setDistance(tempActor->
                                                     getDistance()+weight);

        // If neighbor node is target, give it priority in the queue
        if(tempActor->edges[i]->getActor()->getName() == actorBName) {
          actorB = tempActor->edges[i]->getActor();
          break;
        }

        // add neighboring node to pq
        pq.push(tempActor->edges[i]->getActor());
        // If an edge node already exists, delete it
      } 
    }
  }

  if(actorB) {
    output += " ";
  }
   
  // For each node in needsReset, change done to false to reset graph
  // Reset their distances to inf
  for(ActorNode* n: needsReset) {
    n->setDone(false);
    n->setDistance(numeric_limits<int>::max());
  }

  return output;    
}



/*
 * Finding the Actor connections.  This function reads in two actors from a
 * file and determines whether or not the two actors are connected given the 
 * movies that have been used to connect them, and the earlier year that
 * connects them.
 */ 
void ConnectionsGraph::findActorConnections(char* in_filename, ofstream &out) {

  // Initialize the file stream
  ifstream infile(in_filename);
  // Check if the file is successfully opened
  if (!infile.is_open()) {
    cerr << "Problem opening the file" << endl;
    return;
  }
  // Read the file
  bool have_header = false;  // Check if the header is processed
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
    if( !(nodeSet[actorA] && nodeSet[actorB]) ) continue;

    // Output actors and year they are connected to file
    out << actorA << "\t" << actorB << "\t" 
        << to_string(BFS(actorA, actorB));
    
    // Delete all edges to "reset" the graph to unconnected state
    for(unsigned int i = 0; i < edges.size(); i++) {
      delete(edges[i]);
      edges[i] = nullptr; 
      }
      edges.erase(edges.begin(),edges.end());  
    
    for(auto it = nodeSet.begin();it != nodeSet.end(); it++) {
      ActorNode* temp = it->second;

      temp->edges.erase(temp->edges.begin(), temp->edges.end());
    }
 
    if(!infile.eof()) out << "\n";
  }
  infile.close();
}




/*
 * BFS method adds one year worth of movies at a time and perform Dijkstra's
 * after each year is inserted.  If there is a connection after a particular
 * year, Dijkstra's will return a non-empty string, and this function will
 * return the year the connection was made.  The edges in the graph are then
 * deleted.  Used for actorconnections.
 */ 
int ConnectionsGraph::BFS(string actorAName, string actorBName) {
	
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

      // Create s between actors in MovieNode
      for(unsigned int i = 1; i < temp->actors.size(); i++) {
        Edge* newEdge1 = new Edge(temp, temp->actors[i], 1);
        Edge* newEdge2 = new Edge(temp, temp->actors[i-1], 1);
        temp->actors[i-1]->edges.push_back(newEdge1);
        temp->actors[i]->edges.push_back(newEdge2);
        edges.push_back(newEdge1);
        edges.push_back(newEdge2);
      }
      // Advance it1 to next movie
      it1++;
    }

    // Check if a connection is found by means of unweighted bfs (BFS),
    // if not, add another year of movies
    if(bfs(actorAName, actorBName).length() != 0) {

      return earliestYear;
    }	
	
    // Set the earliest year
    if(it2 != movieVec.end()) {
      earliestYear = (*it2)->getYear();
    }

  } 	  
  
  return NNNN;
} 



/*
 * Comparator class for sorting movie node vector
 */ 
bool VecCompt:: operator()(const MovieNode* a, const MovieNode* b) {
	return (a->getYear() < b->getYear());
}



/* Destructor for ConnectionsGraph to avoid memory leak */
ConnectionsGraph::~ConnectionsGraph() {

  // Delete all ActorNodes
  for(auto it = nodeSet.begin(); it!=nodeSet.end(); it++ ) {
    delete it->second;
  }
  // Delete all MovieNodes
  for(auto it = movieMap.begin(); it!=movieMap.end(); it++ ) {
    delete it->second;
  }
}


