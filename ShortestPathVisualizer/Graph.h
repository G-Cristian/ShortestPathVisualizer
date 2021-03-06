#pragma once

#include <list>
#include <vector>

class Graph
{
public:
	struct Node {
		int index;
		int x;
		int y;
		int heapIndex;
	};

	Graph(int numberOfNodes);
	~Graph() = default;

	const std::vector<Node>& nodes() const { return _nodes; }
	std::vector<Node>& nodes() { return _nodes; }

	const std::vector<std::list<int>>& adjacencyList() const { return _adjacencyList; }
	std::vector<std::list<int>>& adjacencyList() { return _adjacencyList; }

	int startNode() const { return _startNode; }
	int& startNode() { return _startNode; }

	int endNode() const { return _endNode; }
	int& endNode() { return _endNode; }

private:
	std::vector<Node> _nodes;
	std::vector<std::list<int>> _adjacencyList;
	int _startNode;
	int _endNode;
};

bool operator==(const Graph::Node& left, const Graph::Node& right);
