/*
 * extensionConnection.cpp
 * Authors: Dorsadaf Moinzad & Robert Chance
 * Created on: March 14, 2017
 *
 * This file provides implementation for the Youtube data set and 
 * its connection between specific ID and others that they have in common
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <algorithm>
#include <queue>
#include <vector>
#include "ConnectionsGraph.h"
#include "extensionConnection.h"
#include "ActorNode.h"
#define ARG 3


// Constructor
Extension::Extension(void)
{}



/*
 * Loads the data from file
 */
bool Extension::loadFromFile(const char* in_filename)
{
  ifstream infile(in_filename);
  if(!infile.is_open()) {
    cout <<"Error when read file: " << in_filename << endl;
    return false;
  }
  // Keep reading lines until the end of file is reached
  while(infile)  {
    string s;
    if(!getline(infile, s)) break; // break upon EOF

    // Implement stream object for string
    istringstream ss(s);
    // Store VideoID/Category/Other IDs
    vector<string> record;
    // Load single VideoID/Category/OtherIDs
    while(ss) {
      string next;
      if(!getline(ss, next, '\t')) break;
      record.push_back(next);
    }
    // Formatting Data from the file that wants to read
    if(record.size() < 9) continue; ///Invalid line, ignore

    string video_id = record[0];
    string categogy_name = record[3];
    int age(stoi(record[2]));
    auto itVideo = videoMap.find(video_id);
    auto itCategogy = categoryMap.find(categogy_name);
    
    // If video connection doesn't exists
    if(itVideo == videoMap.end()) {
      // New video ID
      VideoNode* tempV = new VideoNode(video_id);
      if(itCategogy == categoryMap.end()) {
        // New Category
        CategoryNode* tempCa = new CategoryNode(categogy_name, age);
        tempCa->actors.push_back(tempV);
       // Read from the file getting each unique videoID
       for(int i = 9; i < (int)record.size(); ++i) {
          // Add related video ID to the map and vector
    	  VideoNode* tempV2 = videoMap[record[i]];
	  // Create new edge that connects the unique ID with 20 others
	  Edge* newEdge = new Edge(tempV, tempV2, tempCa);
    	  tempCa->actors.push_back(tempV2);
	  tempV->edges.push_back(newEdge);
          // Push the newEdge to a vector to keep track of them, toBeDeleted
	  toBeDeleted.push_back(newEdge);
	}
        categoryMap.insert(pair<string, CategoryNode*>(categogy_name, tempCa));
      }
      // If connection exists
      else {
        CategoryNode* tempCa = itCategogy->second;
        for(int i = 9; i < (int)record.size(); ++i)  {
          // Add related video ID to the map and vector
          VideoNode* tempV2 = videoMap[record[i]];
          // Check to find if there are any categories duplication
	  if(find(tempCa->actors.begin(), tempCa->actors.end(),
                               tempV2) == tempCa->actors.end()) {
            // Create new edge to connect unique ID with 20 others
	    Edge* newEdge = new Edge(tempV, tempV2, tempCa);
	    tempCa->actors.push_back(tempV2);  
	    tempV->edges.push_back(newEdge);

            // Push the newEdge to a vector to keep track of them, toBeDeleted
	    toBeDeleted.push_back(newEdge);
    	  }  
    	  else
            delete tempV2;
        }
		tempCa->actors.push_back(tempV);
      }
      videoMap.insert(pair<string, VideoNode*>(video_id, tempV));
    }
    else {
      //Video already exist
      VideoNode* tempV = itVideo->second;
      if(itCategogy == categoryMap.end()) {
        // New Category
        CategoryNode* tempCa = new CategoryNode(categogy_name, age);
        tempCa->actors.push_back(tempV);
        for(int i = 9; i < (int)record.size(); ++i) {
          // Add related video ID to the map and vector
          // Create new edge to connect unique ID with 20 others
          VideoNode* tempV2 = videoMap[record[i]];
	  Edge* newEdge = new Edge(tempV, tempV2, tempCa);
          tempCa->actors.push_back(tempV2);
	  tempV->edges.push_back(newEdge);
          
          // Push the newEdge to a vector to keep track of them, toBeDeleted
	  toBeDeleted.push_back(newEdge);
     }
        categoryMap.insert(pair<string, CategoryNode*>(categogy_name, tempCa));
      }
      else {
        CategoryNode* tempCa = itCategogy->second;
        for(int i = 9; i < (int)record.size(); ++i) {
          // Add related video ID to the map and vector
          VideoNode* tempV2 = videoMap[record[i]];
          if(find(tempCa->actors.begin(), tempCa->actors.end(),
                               tempV2) == tempCa->actors.end()) {
            // Create new edge to connect unique ID with 20 others
	    Edge* newEdge = new Edge(tempV, tempV2, tempCa);
            tempCa->actors.push_back(tempV2);
	    tempV->edges.push_back(newEdge);
	  
            // Push the newEdge to a vector to keep track of them, toBeDeleted
	    toBeDeleted.push_back(newEdge);
	  } 
	  else
            delete tempV2;
        }
	tempCa->actors.push_back(tempV);
      }
    }
  }
  
  if(!infile.eof())  {
    cout << "Failed to read " << in_filename << "!\n";
    return false;
  }
  // Go through the map of categories and prep them for sorting
  for(auto it = categoryMap.begin(); it != categoryMap.end(); it++) {
    categoryVec.push_back(it->second);
  }
  sort(categoryVec.begin(), categoryVec.end(), VecCompt2());

  infile.close();
  return true;
}


/* Returns the averge path length between the Video ID and the related IDs */
int Extension::tracePathLength(VideoNode* &traceNode)
{
  if(traceNode->getPrev() == nullptr)
    return 0;

  return 1 + tracePathLength(traceNode->prevNode);
}



/*
 * Traverses up the tree to find the shortest distance
 */
int Extension::traverseDist(VideoNode* &videoA, VideoNode* &videoB)
{
  /* Set up the queue */
  for(auto it = videoMap.begin(); it != videoMap.end(); ++it)  {
    it->second->setDone(false);
    it->second->setPrev(nullptr);
    it->second->prevNode = nullptr;
  }
  // create a queue for Movie IDs
  queue<VideoNode*> queueNode;
  videoA->setDone(true);
  queueNode.push(videoA);
  while(!queueNode.empty())  {
    VideoNode* currentVideo = queueNode.front();
    queueNode.pop();

    if(currentVideo == videoB)	{
    // If there is a path, return 1, otherwise 0    
    if (tracePathLength(videoB)) return 1;
      else return 0;
    }
    // Going through each of curr neighbors */
    for(int i = 0; i < (int) currentVideo->edges.size(); i++) {
      ActorNode *next = 0;
      Edge *currEdge = currentVideo->edges[i];
      if(currEdge->getActor() == currentVideo)
        next = currEdge->getActor2();
      else
        next = currEdge->getActor();
      // If  node hasn't been visited 
      if(!next->isDone()) {
        next->setDone(true);
        next->prevNode = currentVideo;
        next->setPrev(currEdge);
        queueNode.push(next);
      }
    }
  }
  
  return 0;
}



/*
 * the driver for extension.cpp
 */
void Extension::extension(string video)
{ 

  double avg = 0;  // for find the average path between to videoID
  auto itVideo = videoMap.find(video);
  // Check if we have reached the eof
  if(itVideo == videoMap.end()) {
    cout << "Video doesn't exist in database" << endl;
    return;
  }
  
  VideoNode* videoA = itVideo->second;
  // For printing out the average path
  for(auto it = videoMap.begin(); it != videoMap.end(); ++it) {
    VideoNode* videoB = it->second;
    if(videoB->getName() != video) {
      int dist = 0;
      dist = traverseDist(videoB, videoA);
      avg = avg + dist;
    }
  }
  cout << "The average shortest path to " << video << ": "
       << (double) avg / (videoMap.size() - 1) << endl;
  return;
}


/* Function to clear edges between making connections */
void Extension::clearEdges() {

  for(auto it = toBeDeleted.begin();it != toBeDeleted.end(); it++) {
    delete *it;
  }
}



/* Destructor */
Extension::~Extension() {
  // Delete all the category nodes
  for(auto it = categoryMap.begin(); it != categoryMap.end(); it++) {
    delete it->second;
  }
  // Delete al the video nodes
  for(auto it = videoMap.begin(); it != videoMap.end(); it++){
    delete it->second;
  }
}



/*
 * Comparator class for sorting movie node vector
 */
bool VecCompt2:: operator()(const CategoryNode* a, const CategoryNode* b) {
  return (a->getYear() < b->getYear());
}

