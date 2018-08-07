/*
 * ActorGraph.cpp
 * Author: Robert Chance & Dorsadaf Moinzad
 * Date:   Feb 26, 2017
 *
 * This file is meant to exist as a container for starter code that 
 * you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
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
#include "ActorGraph.h"
#include "ActorNode.h"
#define NNNN 9999

using namespace std;

ActorGraph::ActorGraph(void) {}


/*
 * This function loads a .tsv tab-separated input file and generates a graph
 * with actors as vertices and movies as edges. Used for pathfinder. 
 *
 */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
  // Initialize the file stream
  ifstream infile(in_filename);
  bool have_header = false;// to skip reading the header

  this->weighted = use_weighted_edges;

  // keep reading lines until the end of file is reached
  while(infile) {
    
    string s;

    if(!getline(infile,s)) break;// break upon EOF

    // skip reading the header for first iteration
    if (!have_header) {
      have_header = true;
      continue;
    }
    
    istringstream ss(s);// implement stream object for string
    vector<string> record;// store actor/movie/year info

    // Load list of actors to vector
    while(ss) {
      string next;
      // get the next string before hitting a tab character and put it in 'next'
      if(!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }
    
    if(record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);
    int weight;// 1 for unweighted graphs
    	
    // Create new nodes storing actor name, movie title/year, store movie info
    // inside of actorNode object, add actorNode and movie to map.
    ActorNode* tempActor = new ActorNode(actor_name);
    MovieNode* tempMovie = new MovieNode(movie_title,movie_year);

    // set weight to 1 for unweighted graphs, use year as weight for this graph
    if(use_weighted_edges) {
      weight = 1 + (2015-movie_year);
    }
    else weight = 1;
    

    // Declare a ActorNode pointer so we only have to reference the
    // map once.
    ActorNode* tempA1;

    // Check if actor vertex already exists
    if((tempA1 = nodeSet[actor_name])) {
        
      delete(tempActor);// we no longer need this object
      // Check if movie is already in graph, if so, create edges to actors
      // in that movie, store edges in ActorNode object of this actor and
      // the costar.
      MovieNode* temp = nullptr;

      // Check for existing movie node
      if( (temp = movies[movie_title+to_string(movie_year)]) ){

        // Add edges to all the actors in this movie node
        for(unsigned int i = 0; i < temp->actors.size(); i++) {
   
          // Create an edge pointing from this actor to other actor
          tempA1->edges.push_back(new Edge(temp, temp->actors[i], weight));
          // Create an edge pointing to this actor from other actor
          temp->actors[i]->edges.push_back(new Edge(temp, tempA1, weight));
        }

        // Add this actor to the list of actors in the movieNode
        temp->actors.push_back(tempA1);
        // Delete tempMovie node, we no longer need it
        delete(tempMovie);
      }
      // If movie not found
      if(!temp) {

        // Add this actor to the list of actors in the movieNode
        tempMovie->actors.push_back(tempA1);
        // Add tempMovie to movies map
        movies[movie_title+to_string(movie_year)] = tempMovie;
      }         
    } 

     // If actor vertex is not found
    else {
      // Add tempActor to actors map, check movie for connections to other
      // actors and create edges to all other actors
      nodeSet[actor_name] = tempActor;
      MovieNode* temp = nullptr;
      // Check for movie node
      if( (temp = movies[movie_title+to_string(movie_year)]) ) {
        // Add edges to all other actors in this movie
        for(unsigned int i = 0; i < temp->actors.size(); i++) {
          // create an edge pointing from this actor to other actor
          tempActor->edges.push_back(new Edge(temp,
                                    temp->actors[i],weight));
          // Create an edge pointing to this actor from other actor
          temp->actors[i]->edges.push_back(new Edge(temp,
                                          tempActor,weight));
        }
        // Add this actor to the list of actors in the movieNode
        temp->actors.push_back(tempActor);
        // Delete tempMovie object, we no longer need it
        delete(tempMovie);
      }

      // If movie node was found
      if(!temp) {
        // Add this actor to the list of actors in movieNode 
        tempMovie->actors.push_back(tempActor);
        // Add tempMovie to movies map
        movies[movie_title+to_string(movie_year)] = tempMovie;
      }
    }
  }

  // If file ends loop and is NOT at EOF.
  if(!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();
  return true;
}




/* Dijkstra Algorithm */
string ActorGraph::dijkstra(string actorAName, string actorBName) {
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

        // for unweighted graph, give priority to neighbor node if target
        if(tempActor->edges[i]->getActor()->getName() == 
           actorBName && !weighted) {

          tempActor->edges[i]->getActor()->setDistance(0);
        }

        // add neighboring node to pq
        pq.push(tempActor->edges[i]->getActor());
        // If an edge node already exists, delete it
        if(tempActor->edges[i]->getActor()->getPrev()) {
          delete tempActor->edges[i]->getActor()->getPrev();
        }
        // create new prev edge pointer to add to newly inserted nodes
        Edge* prevEdge = new Edge(tempActor->edges[i]->getMovie(), tempActor, 
                                  tempActor->edges[i]->getDistance());
        tempActor->edges[i]->getActor()->setPrev(prevEdge);
      } 
    }
  }

  // If Node B was found, place each node following the path from node B to 
  // node A into a stack, add the vertices and edges to an output string
  while(actorB) {
    if(actorB->getName() == actorA->getName()) {
      path.push(actorB);
      break;
    }
    // add actor B to the stack
    path.push(actorB);
    // set actor B equal to prev
    actorB = actorB->getPrev()->getActor();
      
  }
  // print the path to return string, empty stack and print path
  while(!path.empty()) { 

    // Get first node of path
    ActorNode* tempActor = path.top();
    path.pop();
    
    // If tempActor is first node in list, print actor info and don't print
    // edge info, otherwise, print edge info then actor info
    if(tempActor->getName() == actorAName) {
      output += tempActor->printNode(); 
    }
    else {

      if(tempActor->getPrev() != nullptr) {

        output += tempActor->getPrev()->printEdge();
        output += tempActor->printNode();
      }
    }
  }
  
  if(output.length() != 0) output += "\n";

  // For each node in needsReset, change done to false to reset graph
  // Reset their distances to inf
  for(ActorNode* n: needsReset) {
    n->setDone(false);
    n->setDistance(numeric_limits<int>::max());
  }

  return output;    
}




/*
 * This function loads actor pairs and calls dijkstra() to determine if the
 * pairs are connected.  Used for pathfinder.
 */
bool ActorGraph::ConnectPairs(const char* in_filename, ofstream& out) {

  // Initialize the file stream
  ifstream infile(in_filename);
  bool have_header = false; // to skip reading the header
  string output = "(actor)--[movie#@year]-->(actor)--...\n";

  // keep reading lines until the end of file is reached
  while (infile) {
    
    string s;

    if(!getline(infile,s)) break; // break upon EOF
    // skip reading the header for first iteration
    if (!have_header) {
      have_header = true;
      continue;
    }
    
    istringstream ss(s);// implement stream object for string
    vector <string> record;// store actor/movie/year info

    // Load pair of actors to compare
    while (ss) {
      string next;
      // get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }
    
    if (record.size() != 2) {
      continue;
    }
	
    // ensure actors are in data set
    if(nodeSet[record[0]] && nodeSet[record[1]]) { 
      // run Dijkstra's  on actor pair
      output += dijkstra(record[0], record[1]);
    }

    else cerr << "Actor(s) not found" << endl;
  }

  // If loop ends without reaching eof
  if (!infile.eof()) {
  return false;
  }

  infile.close();
  out << output;
  return true;
}




/*
 * Comparator class for sorting movie node vector
 */ 
bool VecCompt:: operator()(const MovieNode* a, const MovieNode* b) {
	return (a->getYear() < b->getYear());
}



/* Destructor for ActorGraph to avoid memory leak */
ActorGraph::~ActorGraph() {

  // Delete all ActorNodes
  for(auto it = nodeSet.begin(); it!=nodeSet.end(); it++ ) {
    delete it->second;
  }
  // Delete all MovieNodes
  for(auto it = movies.begin(); it!=movies.end(); it++ ) {
    delete it->second;
  }
}


