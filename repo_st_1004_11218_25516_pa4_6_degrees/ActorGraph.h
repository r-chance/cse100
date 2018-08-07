/*
 * ActorGraph.h
 * Author: Dorsadaf Moinzad & Robert Chance
 * Date:   Feb 26, 2017
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

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



/* The ActorGraph class provides a container in which to build a connected
 * graph of actor vertices and movie edges
 */
class ActorGraph {
protected:
  // Container that holds the ActorNode set
  unordered_map<string, ActorNode*> nodeSet; 
  // Container that holds the movie names
  unordered_map<string, MovieNode*> movies;
  // Movie vector to sort in unionFind
  vector<MovieNode*> movieVec;
  // Movie map, for speed
  unordered_map<string, MovieNode*> movieMap;
  // Vector of edges to be cleared after each actor connection
  vector<Edge*> edges;

public:

  ActorGraph(void);
  bool weighted = false;
  // Destructor
  ~ActorGraph();

  /** 
   * This funciton takes in a file name containing actor/movie info and
   * a boolean declaring if the graph is weighted or unweighted.  It will
   * build the graph with actors as vertices and movies as connections and
   * add edges connecting actors.  
   */
  bool loadFromFile(const char* in_filename, bool use_weighted_edges);

  /* We need to traverse through all the adjacent vertices of graph 
   * using BFS and use a Min Heap to store the vertices that are not
   * yet explored and update their values
   */
  bool ConnectPairs(const char* in_filename, ofstream &out);

  /* This function takes in two actor name strings and performs 
   * Dijkstra's algorithm to search for the shorted weighted or unweighted
   * path between the two.  It will return a string representation of
   * the path between the two actors.
   */  
  string dijkstra(string actorAName, string actorBName); 
};


#endif // ACTORGRAPH_H
