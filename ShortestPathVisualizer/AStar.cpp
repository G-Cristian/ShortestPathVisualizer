#include "AStar.h"

#include "Graph.h"
#include "MinHeap.h"

#include <vector>

int AStar::step() {
	if (_minHeap.isEmpty()) {
		//no more nodes
		return -1;
	}

	auto minNode = _minHeap.top();
	_minHeap.pop();

	if (minNode.second->index == _graph->endNode()) {
		// I found the end node, therefore finish
		return -1;
	}

	for (int adj: _graph->adjacencyList()[minNode.second->index]) {
		relaxEdge(minNode.second->index, adj);
	}

	return minNode.second->index;
}

void AStar::initialize() {
	// initialize weights functions for start node
	int start = _graph->startNode();
	_weightsFromStart[start] = 0.0f;

	// create heap entry for start node with weight 0.0 ...
	MinHeap::ElementType e;
	e.first = 0.0f;
	e.second = &_graph->nodes()[start];

	// ... and insert it into the heap
	_minHeap.insert(e);
}

void AStar::relaxEdge(int u, int v) {
	float newWeightFromStart = _weightsFromStart[u] + 1.0f;		// every edge has weight equal to 1
	float vWeightFromStart = _weightsFromStart[v];
	float newWeightWithHeuristic = newWeightFromStart + heuristicDistance(_graph->nodes()[v], _graph->nodes()[_graph->endNode()]);

	if (vWeightFromStart < 0.0f) {
		// if distance from start node to v is less than zero
		// then v hasn't been visited yet
		// NOTE: I use weight less than zero like infinite

		_weightsFromStart[v] = newWeightFromStart;

		//create and insert the min heap element
		MinHeap::ElementType e;
		e.first = newWeightWithHeuristic;
		e.second = &_graph->nodes()[v];
		_minHeap.insert(e);

		// and update parents
		_parents[v] = u;
	}
	else if (vWeightFromStart> newWeightFromStart) {
		_weightsFromStart[v] = newWeightFromStart;
		_minHeap.decreaseKey(_graph->nodes()[v].heapIndex, newWeightWithHeuristic);
		_parents[v] = u;
	}
}

float AStar::heuristicDistance(const Graph::Node& a, const Graph::Node& b)const {
	return std::abs(b.x - a.x) + std::abs(b.y - a.y);
}
