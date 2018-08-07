/*
 * ActorNode.cpp
 * Author: Dorsadaf Moinzad and Robert Chance
 *
 * Created on: Feb 28, 2017
 *
 * This file provides implementation for ActorNode, MovieNode and Edge classes
 * that are used to construct a connected graph of actors and movies.
 */
#include <iostream>
#include "ActorNode.h"
#include <limits>

using namespace std;


/*
 * Constructor for ActorNode
 */
ActorNode::ActorNode(string name): distance(numeric_limits<int>::max()),
           prev(nullptr), done(false), name(name) {
}


/* 
 * Destructor for ActorNode
 */
ActorNode::~ActorNode() {
  if(prev) delete prev;
  for(unsigned int i = 0; i < edges.size(); i++) {
    delete edges[i];
  }
}


/* Overload equality operator to compare Actor names of nodes */
bool ActorNode::operator==(const ActorNode* n) const {

  if(this->name == n->name) return true;
  else return false;
}


/* Function to print name of Actor in this node */
string ActorNode::printNode() {
  string s = "";
  s += "(";
  s += name;
  s += ")";
  return s;
}


/* Accessor function to return distance of this node */
unsigned int ActorNode::getDistance() const {
  
  return this->distance;
}


/* Mutator function to set distance of this node */
void ActorNode::setDistance(unsigned int i) {

  this->distance = i;
}


/* Accessor function for returning prev edge */
Edge* ActorNode::getPrev() {

  return this->prev;
}


/* Mutator function for setting prev edge */
void ActorNode::setPrev(Edge* e) {

  this->prev = e;
}


/* Accessor function to return name of this node */
string ActorNode::getName() {

  return this->name;
}


/* Accessor function to return parent of this node, used in unionfind */
ActorNode* ActorNode::getParent() {

  return this->parent;
}


/* Mutator function to set parent node, used in unionfind */
void ActorNode::setParent(ActorNode* a) {

  this->parent = a;
}


/* Accessor function to return if node has been explored */
bool ActorNode::isDone() {

  return this->done;
}


/* Mutator function to set node to done */
void ActorNode::setDone(bool b) {

  this->done = b;
}


/*
 * Comparator class to find the shorterst distance >
 */ 
bool PQComparator::operator()(const ActorNode* a, const ActorNode* b) {
	return (a->getDistance() > b->getDistance());
}


/*
 * Edge class to connect actors and movie and their distances
 */
Edge::Edge(MovieNode *movies, ActorNode *actor, int distance) :
	movie(movies), actor(actor), distance(distance) {}  

/*
 * Edge class used in extension
 */
 Edge::Edge(ActorNode* & actor1, ActorNode* & actor2, MovieNode* & movie) :
 	    movie(movie), actor(actor1), actor2(actor2), distance(0) {}

/*
 * Class to print edge info to a string
 *
 */
string Edge::printEdge() {
  if(movie) {
    string s = "";
    s += "--[";
    s += movie->getTitle();
    s += "#@";
    s += to_string(movie->getYear());
    s += "]-->";
    return s;
  }
}


/* Accessor for returning MovieNode stored in edge */
MovieNode* Edge::getMovie() {

  return this->movie;
}


/* Accessor for returning ActorNode stored in edge */
ActorNode* Edge::getActor() {

  return this->actor;
}

/* Accessor for returning ActorNode stored in edge for extension */
ActorNode* Edge::getActor2() {
  
  return this->actor2;
}


/* Accessor for returning distance of this edge */
int Edge::getDistance() {

  return this->distance;
}


/* Mutator for setting distance of this edge */
void Edge::setDistance(int d) {

  this->distance = d;
}


/* 
 * Constructur for MovieNode 
 */
MovieNode::MovieNode (string title, int year){
	this->title = title;
	this->year = year;
}


/*
 * Comparator definition for == operator 
 */
bool MovieNode::operator==(const MovieNode* n) const {

  if(this->title == n->title && this->year == n->year) return true;
  else return false;
}

Edge::~Edge() {
  movie = nullptr;
  actor = nullptr;
}


/* Accessor for returning movie title */
string MovieNode::getTitle() {

  return this->title;
}


/* Accessor for returning movie year */
int MovieNode::getYear() const {

  return this->year;
}
