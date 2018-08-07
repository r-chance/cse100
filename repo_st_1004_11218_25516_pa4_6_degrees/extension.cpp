/*
 * extension.cpp
 * Created on : March 13, 2017
 *
 */ 

#include "ConnectionsGraph.h"
#include "extensionConnection.h"
#include "ActorNode.h"
#include <iostream>
#include <fstream>

using namespace std;

#define ARG 3

/*
 * Main function of the program that takes input from the user
 * and computes the avg shortest path to a particular actor.
 *
 * argv 1 : the name of the file read from
 * argv 2 : the name of the Unique viedo ID
 */ 
int main (int argc, char **argv) 
{

  // Check if the number of arguements is valid
  if (argc != ARG && argc != ARG - 1) {
     cout << "Wrong number of arguements" << endl;
     cout << "./extension YouTube Data Set" << endl;
     return -1;
  }

  string video_id(argv[2]);

  // Create and connect graph
  Extension* graph = new Extension();

  if (!graph->loadFromFile(argv[1])) {
    cout << "Load file: " << argv[1] << "failed." << endl;
    return -1;
  }
 
  graph->extension(video_id);
  graph->clearEdges();

  //delete graph;
  return 0;

}
