#include "Graph.h"

#include <list>
#include <vector>

bool operator==(const Graph::Node& left, const Graph::Node& right) {
	return left.index == right.index && left.x == right.x && left.y == right.y;
	// ignore the heapIndex member
}

Graph::Graph(int numberOfNodes):
	_startNode(0),
	_endNode(0) {
	_nodes.reserve(numberOfNodes);
	_adjacencyList.reserve(numberOfNodes);
}