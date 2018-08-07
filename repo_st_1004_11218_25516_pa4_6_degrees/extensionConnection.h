/*
 * extensionConnection.h
 * Created on : March 7th, 2017
 * Authors: Dorsadaf Moinzad & Robert Chance
 */

#ifndef EXTENSIONCONNECTION_H_
#define EXTENSIONCONNECTION_H_

#include <iostream>
#include <vector>
#include <fstream>
#include "ActorNode.h"
#include <sstream>

using namespace std;
//VideoID <==> actor
//category <==> movie

typedef ActorNode VideoNode;
typedef MovieNode CategoryNode;
class Extension {
protected:
  unordered_map<string, VideoNode*> videoMap;
  // Container that holds the movie names
  unordered_map<string, CategoryNode*> categoryMap;
  // Movie vector to sort in unionFind
  vector<CategoryNode*> categoryVec;
 // Vector of edges to be cleared after each actor connection
//  vector<Edge*> edges;
  vector<Edge*> toBeDeleted;

public:

  Extension(void);
  ~Extension();
    
  bool loadFromFile(const char*);
  /*
   * Find the edge actor connection
   */
  void connectVideo();

  /*
   * for extension traverse
   */
  int traverseDist(VideoNode* &, VideoNode* &);

  /*
   * find the shortest path
   */
  int tracePathLength(VideoNode* &);

  /*
   * function that executes extension part
   */
  void extension(string);

  /*
   * function that clears edges between connections
   */
  void clearEdges();

};

class VecCompt2 {
public:
  bool operator()(const CategoryNode *a, const CategoryNode* b);
};

#endif // EXTENSIONCONNECTION_H_
