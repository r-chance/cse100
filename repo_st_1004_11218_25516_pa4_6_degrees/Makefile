# A simple makefile for CSE 100 PA4

CC=g++
CXXFLAGS=-std=c++11 -g -Wall
LDFLAGS=-g

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
    CPPFLAGS += -O3
    LDFLAGS += -O3
else
    CPPFLAGS += -g
    LDFLAGS += -g
endif

all: pathfinder actorconnections extension



# include what ever source code *.h files pathfinder relies on (these are 
# merely the ones that were used in the solution)

pathfinder: ActorGraph.o ActorNode.o
actorconnections: ConnectionsGraph.o DisjointSet.o ActorNode.o
extension: ConnectionsGraph.o extensionConnection.o ActorNode.o DisjointSet.o

# include what ever source code *.h files ActorGraph relies on (these are 
# merely the ones that were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired 
# *.h file that is already included with class/method headers

#ActorGraph.o: UnionFind.hpp Movie.h ActorNode.h ActorEdge.h ActorGraph.h
ActorGraph.o: ActorGraph.h
ActorNode.o: ActorNode.h
ConnectionsGraph.o: ConnectionsGraph.h
DisjointSet.o: DisjointSet.h
extension.o: extensionConnection.h
extensionConnection.o: extensionConnection.h
clean:
	rm -f actorconnections extension pathfinder *.o core*

