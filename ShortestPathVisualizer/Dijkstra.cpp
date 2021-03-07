#include "Dijkstra.h"

#include "Graph.h"
#include "MinHeap.h"

#include <vector>

int Dijkstra::step() {
	if (_minHeap.isEmpty()) {
		//no more nodes
		return -1;
	}

	auto minElement = _minHeap.top();
	_minHeap.pop();
	if (minElement.second->index == _graph->endNode()) {
		// I found the end node, therefore finish
		return -1;
	}
	for (int adjNode : _graph->adjacencyList()[minElement.second->index]) {
		relaxEdge(minElement.second->index, adjNode);
	}

	return minElement.second->index;
}

void Dijkstra::initialize() {
	// initialize weights function for start node
	_weightsFromStart[_graph->startNode()] = 0.0f;

	// create heap entry for start node with weight 0.0 ...
	MinHeap::ElementType e;
	e.first = 0.0f;
	e.second = &_graph->nodes()[_graph->startNode()];

	// ... and insert it into the heap
	_minHeap.insert(e);
}

void Dijkstra::relaxEdge(int u, int v) {
	float newVWeight = _weightsFromStart[u] + 1.0f; // every edge has weight equal to 1
	if (_weightsFromStart[v] < 0.0f) {
		// if distance from start node to v is less than zero
		// then v hasn't been visited yet
		// NOTE: I use weight less than zero like infinite

		_weightsFromStart[v] = newVWeight;

		//create and insert the min heap element
		MinHeap::ElementType e;
		e.first = newVWeight;
		e.second = &_graph->nodes()[v];
		_minHeap.insert(e);
	}
	else if(_weightsFromStart[v] > newVWeight){
		_weightsFromStart[v] = newVWeight;
		_minHeap.decreaseKey(_graph->nodes()[v].heapIndex, newVWeight);
	}
}