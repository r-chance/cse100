/*
 * ActorNode.h
 *
 * Authors: Robert Chance & Dorsadaf Moinzad
 * Created on: Feb 26, 2017
 *
 * This header file contains the interface to the ActorNode, MovieNode and Edge
 * classes.
 */

#ifndef ACTORNODE_H_
#define ACTORNODE_H_

#include <unordered_map>
#include <string>
#include <utility>
#include <functional>
#include <vector>
class MovieNode; // Forward declare MovieNode class
class Edge;      

using namespace std;

/* In this class each actor/actess is defined as a single node in the graph
 * Two nodes (actors) are connected by an undirected edge (using unordered
 * map) if the corresponding actor played in the same movie.
 */ 
class ActorNode
{
public:

  ActorNode* prevNode;
  vector<Edge*> edges;
 
  // Public member functions
  ActorNode(string); // ActorNode constructor
  ~ActorNode(); // ActorNode destructor


  /* Equality operator overload */
  bool operator==(const ActorNode* n) const; 

  /* function to print ActorNode data */
  string printNode(); 


  /* Accessor and mutator functions to access distance */
  unsigned int getDistance() const; 
  void setDistance(unsigned int i); 

  /* Accessor and mutator functions to access prev edge */
  Edge* getPrev();
  void setPrev(Edge* e);

  /* Accessor to return name */
  string getName();

  /* Accessor and mutator functions to access parent node in ufind*/
  ActorNode* getParent();
  void setParent(ActorNode* a);

  /* Accessor and mutator functions to access done boolean */
  bool isDone();
  void setDone(bool b);

private:
  unsigned int distance; // The distance from target node to this node
  Edge* prev = nullptr;
  bool done; // If shortest path has been explored
  string name; // name of this node  
  ActorNode* parent = nullptr; // Check for unionFind
  
};




/* This class compares the unweighted shortest path between
 * two actors in test_pairs.tsv
 */ 
class PQComparator {
public:
  bool operator()(const ActorNode *a, const ActorNode *b);
};




/* Edge class to make a relationship between the nodes */
class Edge {
public:

  // Constructor and destructor for edge class
  Edge(MovieNode*, ActorNode*, int);
  // Constructor for edge class used in extension
  Edge(ActorNode* &, ActorNode* &, MovieNode* &);
  ~Edge();

  string printEdge();// prints edge/vertex info for this edge and actor      

  // Accessor for returning MovieNode
  MovieNode* getMovie();

  // Accessor for returning ActorNode
  ActorNode* getActor();
  // Accessor for returning in extension
  ActorNode* getActor2();
  // Accessor and mutator for getting/setting distance
  int getDistance();
  void setDistance(int d);


private:
  MovieNode* movie;
  ActorNode* actor;
  ActorNode* actor2;
  int distance;
};




/* Movie class to store title and date of movie */
class MovieNode {

public:

  // Constructor for MovieNode
  MovieNode(string title, int year);
  // Vector to store actors of this movie
  vector<ActorNode*> actors;
  // Equality comparator for movie nodes
  bool operator==(const MovieNode* n) const;
  // Accessor functions for movie title and year
  int getYear() const;
  string getTitle();

private:
  int year; // movie year
  string title; // movie title   
};



#endif /* ACTORNODE_H_ */
