#pragma once

#include "Graph.h"

#include <assert.h>
#include <utility>
#include <vector>

class MinHeap
{
public:
	typedef std::pair<float, Graph::Node*> ElementType;

	MinHeap() = default;
	MinHeap(size_t capacity);

	~MinHeap() = default;

	void insert(ElementType& element);
	void pop();

	const ElementType& top() const;
	ElementType& top();

	void decreaseKey(int index, float newValue);

	bool isEmpty()const { return _elements.empty(); }
	size_t size()const { return _elements.size(); }
	size_t capacity()const { return _elements.capacity(); }
private:
	//precondition index > 0
	int parent(int index) const {
		assert(index > 0);
		//(index-1) / 2
		return (index - 1) >> 1;
	}

	int left(int index)const {
		int child = (index << 1) + 1;	//(index * 2) + 1

		return child;
	}

	int right(int index)const {
		int child = (index << 1) + 2;	//(index * 2) + 2

		return child;
	}

	void minHeapify(int index);
	void swapElements(int first, int second);

	std::vector<ElementType> _elements;
};
