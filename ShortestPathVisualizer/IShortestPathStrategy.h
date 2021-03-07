#pragma once

#include <vector>

class IShortestPathStrategy {
public:
	virtual ~IShortestPathStrategy() = default;

	virtual int step() = 0;

	const std::vector<int>& parents()const { return _parents; }
protected:
	IShortestPathStrategy(size_t capacity) :
		_parents(capacity, -1.0f) {
	}

	std::vector<int> _parents;
};