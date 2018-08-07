/*
 * ConnectionsGraph.h
 * Authors: Robert Chance and Dorsadaf Moinzad
 * Date: 3/11/17
 * 
 * This file will provide the implementation for a graph class to be used with
 * the actorconnections file.  It contains an algorith for BFS.
 */

#ifndef CONNECTIONSGRAPH_H
#define CONNECTIONSGRAPH_H

#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "ActorNode.h"

using namespace std;

/* Comparator class for Movie vectors to be sorted */
class VecCompt {
public:
  bool operator()(const MovieNode *a, const MovieNode* b);
};



/* This class provides implementation to create a connected graph and perform
 * a breadth first search on the nodes.
 */
class ConnectionsGraph {
protected:
  // Container that holds the ActorNode set
  unordered_map<string, ActorNode*> nodeSet; 
  // Container that holds the Movie names
  unordered_map<string, MovieNode*> movies;
  // Movie vector to sort in unionFind
  vector<MovieNode*> movieVec;
  // Movie map, for quicker loading
  unordered_map<string, MovieNode*> movieMap;
  // Vector of edges to be cleared after each actor connection
  vector<Edge*> edges;

public:
  ConnectionsGraph(void);
  // Destructor
  ~ConnectionsGraph();

  /*
   * This function takes in a file name containing actor/movie info and 
   * constructs an unconnected graph with the information. It will store
   * the movies in a data structure to be added to the graph later.  Returns
   * true if successful.
   */
  bool loadFromFile(const char* in_filename, bool use_weighted_edges);
	

  /* This function takes in a filename containing actor pairs and an output
   * stream to write to.  It will call BFS on the two actors and print the
   * year they became connected to the output stream 
   */
  bool ConnectPairs(const char* in_filename, ofstream &out);

  /* 
   * This function takes in two strings as actor names and performs a
   * BFS to determine if they are connected.  Returns an empty string if
   * both actors are never connected.
   */  
  string bfs(string actorAName, string actorBName);

  /*
   * This function takes 2 string actor names as input and finds whether two
   * actors are connected by calling the bfs function on them.
   */    
  int BFS(string, string);
  
  /*
   * Takes 2 actor nodes and merges the two disjoint sets into a single set 
   * that will be used in unionFind search.
   */
  bool mergeSet(ActorNode*, ActorNode*);

  /*
   * Takes in a char* file name containing actor pairs and an output stream,
   * compares actors and writes output to stream.
   */ 
  void findActorConnections(char*, ofstream&);
  
  /*
   * Another load from file to implement the graph for unionFind
   */  
  bool loadFromFile (const char* in_filename); 

  
};


#endif // CONNECTIONSGRAPH_H
