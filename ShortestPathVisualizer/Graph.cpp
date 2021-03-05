#include "Graph.h"

#include <list>
#include <vector>

Graph::Graph(int numberOfNodes):
	_startNode(0),
	_endNode(0) {
	_nodes.reserve(numberOfNodes);
	_adjacencyList.reserve(numberOfNodes);
}