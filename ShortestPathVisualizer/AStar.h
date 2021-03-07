#pragma once

#include "IShortestPathStrategy.h"

#include "Graph.h"
#include "MinHeap.h"

#include <memory>
#include <vector>

class AStar :
    public IShortestPathStrategy
{
public:
	AStar(std::shared_ptr<Graph> graph, size_t capacity) :
		IShortestPathStrategy(),
		_graph(graph),
		_minHeap(capacity),
		_weightsFromStart(capacity, -1.0f){
		initialize();
	}

	virtual ~AStar() = default;

	int step() override final;
private:
	void initialize();
	void relaxEdge(int u, int v);
	float heuristicDistance(const Graph::Node& a, const Graph::Node& b)const;
	std::shared_ptr<Graph> _graph;
	MinHeap _minHeap;
	std::vector<float> _weightsFromStart;
};

