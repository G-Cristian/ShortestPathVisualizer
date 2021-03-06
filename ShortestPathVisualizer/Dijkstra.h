#pragma once

#include "IShortestPathStrategy.h"

#include "Graph.h"
#include "MinHeap.h"

#include <memory>
#include <vector>

class Dijkstra :
    public IShortestPathStrategy
{
public:
	Dijkstra(std::shared_ptr<Graph> graph, size_t capacity) :
		IShortestPathStrategy(),
		_graph(graph),
		_minHeap(capacity),
		_weightsFromStart(capacity, -1.0f){
		initialize();
	}

	virtual ~Dijkstra() = default;

	int step() override final;
private:
	void initialize();
	void relaxEdge(int u, int v);

	std::shared_ptr<Graph> _graph;
	MinHeap _minHeap;
	std::vector<float> _weightsFromStart;
};

