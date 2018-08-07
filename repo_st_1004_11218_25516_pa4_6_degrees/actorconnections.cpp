/*
 * actorconnections.cpp
 *
 * Created on: March 6th, 2017
 * Authors: Dory Moinzad & Robert Chance
 *
 */ 

#include <iostream>
#include <fstream>
#include "ConnectionsGraph.h"
#include "DisjointSet.h"

using namespace std;
#define ARG 5

/*
 * This program performs either BFS or union-find on a data set. It first
 * calls loadFromFile on the data set to create a (disjoint) set of single
 * actor nodes.  If arg 4 specifies BFS, this program then runs the BFS
 * function, adding an entire year's worth of movies and checking after each
 * year if two actors are connected.  If arg 4 specifies union-find, the
 * program runs the union-find function, adds a year of movies and checks if
 * the sentinel node for two actors is the same. If arg 4 does not exist, 
 * the program performs union-find by default.
 */ 

int main (int argc, char *argv[])
{

  bool uFindSearch = true; // set search type to ufind by default
  Timer timer;// create a new timer to record runtimes 
  long long runtime;// runtime of the searches in ms 


  // Create an out stream
  ofstream out;

  // open stream to output file
  out.open(argv[3]);
  if (!out.is_open()) {
    cout << "Can't write to the file: " << argv[3] << endl;
    return -1;
  }
  
  // Check if the number of arguments is correct
  if (argc != ARG && argc != ARG-1) {
    cout << "Wrong number of arguments" << endl;
    return -1;
  }

  // If ufind flag is present
  if(argc == ARG) {

    string s(argv[ARG-1]);
    if(s == "ufind") uFindSearch = true;
    else if(s == "bfs") uFindSearch = false;
    else cout << "Invalid arg: " << s << endl;
  }

  // if ufind flag is false, load graph and perform bfs
  if(!uFindSearch) {


    // Create graph object
    ConnectionsGraph* graph = new ConnectionsGraph();

    // Call load from file on graph, if loading is unsucessful, return -1
    // Graph contains all (unconnected) actors after this.
    if(!graph->loadFromFile(argv[1])) {
      cout << "Load file: " << argv[1] << " failed." << endl;
      return -1;
    } 

    timer.begin_timer();
    // Load actor pairs and perform BFS
    graph->findActorConnections(argv[2], out);
    delete graph;
    
  }

  // if ufind flag is true, load disjoint set and perform union-find
  else {


    // Create new disjoint set object
    DisjointSet* djSet = new DisjointSet();

    // Call load from file on disjoint set, if unsuccessful, return -1
    if(!djSet->loadFromFile(argv[1])) {
      cout << "Load file: " << argv[1] << " failed." << endl;
      return -1;
    }

    timer.begin_timer();
    // Load actor pairs and perform union-find, write output to file
    djSet->connectPairs(argv[2], out);
    delete djSet;
  }

  
  runtime = timer.end_timer();
  cout << "The search took " << runtime/1000000 << " ms to run." << endl;
  out.close();
  return 0;


}
