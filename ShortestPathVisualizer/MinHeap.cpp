#include "MinHeap.h"

#include "Graph.h"

#include <assert.h>
#include <utility>
#include <vector>

MinHeap::MinHeap(size_t capacity) {
	_elements.reserve(capacity);
}

void MinHeap::insert(MinHeap::ElementType& element) {
	_elements.push_back(element);
	element.second->heapIndex = _elements.size() - 1;
	decreaseKey(_elements.size() - 1, element.first);
}

void MinHeap::pop() {
	assert(size() > 0);
	swapElements(0, _elements.size() - 1);
	_elements.pop_back();
	minHeapify(0);
}

const MinHeap::ElementType& MinHeap::top() const {
	assert(size() > 0);

	return _elements[0];
}

MinHeap::ElementType& MinHeap::top() {
	assert(size() > 0);

	return _elements[0];
}

void MinHeap::decreaseKey(int index, float newValue) {
	assert(index >= 0 && index < size());
	assert(newValue <= _elements[index].first);

	_elements[index].first = newValue;
	
	while (index != 0 && _elements[parent(index)].first > newValue) {
		swapElements(parent(index), index);
		index = parent(index);
	}
}

void MinHeap::minHeapify(int index) {
	assert(index >= 0);

	int lft = left(index);
	int rght = right(index);

	int minIndex = index;
	if (lft < size() && _elements[lft] < _elements[minIndex]) {
		minIndex = lft;
	}

	if (rght < size() && _elements[rght] < _elements[minIndex]) {
		minIndex = rght;
	}

	if (minIndex != index) {
		swapElements(index, minIndex);
		minHeapify(minIndex);
	}
}

void MinHeap::swapElements(int first, int second) {
	using std::swap;

	swap(_elements[first], _elements[second]);
	//don't forget to update the nodes heap index to maintain the invariant
	//i.e. nodes's heap index must point to the position in the heap
	_elements[first].second->heapIndex = first;
	_elements[second].second->heapIndex = second;
}