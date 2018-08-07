/*
 * pathfinder.cpp
 *
 * Authors: Dorsadaf Moinzad & Robert Chance
 * Created on: Feb 26, 2017
 *
 */
 
#include <fstream>
#include "ActorGraph.h"
#define NUM_ARGS 5

/**
 * This program is based off of the 6 degrees of Kevin Bacon. 
 * It takes as input any two actors/actresses and finds the shortest
 * sequence of shared movies between them using unweighted shortest path.
 */
int main(int argc, char *argv[]) {

  // check for correct number of arguments
  if(argc != NUM_ARGS) {
    
    cout << "Correct usage: ./pathfinder movie_casts.tsv u test_pairs.tsv "
         << "out_paths_unweighted.tsv" << endl;
    return -1;
  }

  bool weighted;
  if(*argv[2]=='u') weighted = false;
  else weighted = true;

  ActorGraph* graph = new ActorGraph();

  if (!graph->loadFromFile(argv[1], weighted)) {
	cout << "Load file:" << argv[1] << " failed." << endl;
    return -1;
  }

  // Create an out stream
  ofstream out;

  out.open(argv[4]);
  if (!out.is_open()) {
   cout << "Can't write to the file: " << argv[4] << endl;
   return -1;
  }
  // Traversing the graph
  if (!graph->ConnectPairs(argv[3], out)) {
   cout << "Traversal failed" << endl;
  }

  out.close();

  delete graph;
  return 0;
}
