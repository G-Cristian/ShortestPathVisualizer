#include "Graph.h"
#include "MinHeap.h"

#include <assert.h>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

/*************  DEFINES  ****************/

#define TEST(func, errCount, okCount) do{\
										 bool ret = func();\
										 if(ret){\
											++okCount;\
										 }else{\
											++errCount;\
										 }\
										 std::cout << "Test " << #func << (ret ? " OK" : " FAIL") << std::endl;\
									  }while(false)

/***********  END DEFINES  **************/

/************  FUNCTIONS  ***************/

bool test_empty_heap();
bool test_empty_heap_capacity_three_size_equals_zero_and_capacity_equals_three();
bool test_insert_element_not_empty();
bool test_insert_element_size_equals_one();
bool test_insert_pop_element_size_equals_zero();
bool test_top_smallest();
bool test_insert_big_small_preserves_invariant();
bool test_insert_small_big_smallest_preserves_invariant();
bool decrease_key_first_preserves_invariant();
bool decrease_key_second_making_it_smallest_preserves_invariant();
bool decrease_key_forth_making_it_smallest_preserves_invariant();
bool decrease_key_forth_but_does_not_make_it_smallest_preserves_invariant();
bool insert_pop_insert_preserves_invariant_and_does_not_change_previous_insertion();
bool insert_pop_insert_insert_preserves_invariant_and_does_not_change_previous_insertion();
bool insert_and_asks_for_key_less_than_zero();
bool insert_pop_and_asks_for_key_equal_zero();
bool insert_and_asks_for_key_zero();

/**********  END FUNCTIONS  *************/

int main() {
	int okCount = 0;
	int errCount = 0;
	int totalTests = 17;

	TEST(test_empty_heap, errCount, okCount);
	TEST(test_empty_heap_capacity_three_size_equals_zero_and_capacity_equals_three, errCount, okCount);
	TEST(test_insert_element_not_empty, errCount, okCount);
	TEST(test_insert_element_size_equals_one, errCount, okCount);
	TEST(test_insert_pop_element_size_equals_zero, errCount, okCount);
	TEST(test_top_smallest, errCount, okCount);
	TEST(test_insert_big_small_preserves_invariant, errCount, okCount);
	TEST(test_insert_small_big_smallest_preserves_invariant, errCount, okCount);
	TEST(decrease_key_first_preserves_invariant, errCount, okCount);
	TEST(decrease_key_second_making_it_smallest_preserves_invariant, errCount, okCount);
	TEST(decrease_key_forth_making_it_smallest_preserves_invariant, errCount, okCount);
	TEST(decrease_key_forth_but_does_not_make_it_smallest_preserves_invariant, errCount, okCount);
	TEST(insert_pop_insert_preserves_invariant_and_does_not_change_previous_insertion, errCount, okCount);
	TEST(insert_pop_insert_insert_preserves_invariant_and_does_not_change_previous_insertion, errCount, okCount);
	TEST(insert_and_asks_for_key_less_than_zero, errCount, okCount);
	TEST(insert_pop_and_asks_for_key_equal_zero, errCount, okCount);
	TEST(insert_and_asks_for_key_zero, errCount, okCount);

	assert(totalTests == (okCount + errCount));

	std::cout << std::endl;
	std::cout << "Ok tests: " << okCount << std::endl;
	std::cout << "Fail tests: " << errCount << std::endl;
	std::cout << "Total tests: " << totalTests << std::endl;
}

bool test_empty_heap() {
	MinHeap heap;
	return heap.isEmpty();
}

bool test_empty_heap_capacity_three_size_equals_zero_and_capacity_equals_three() {
	MinHeap heap(3);
	return heap.isEmpty() && heap.size() == 0 && heap.capacity() == 3;
}

bool test_insert_element_not_empty() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node node;
	e.first = 2;
	e.second = &node;

	heap.insert(e);
	return !heap.isEmpty();
}

bool test_insert_element_size_equals_one() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node node;
	e.first = 2;
	e.second = &node;

	heap.insert(e);
	return heap.size() == 1;
}

bool test_insert_pop_element_size_equals_zero() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node node;
	e.first = 2;
	e.second = &node;

	heap.insert(e);
	heap.pop();
	return heap.isEmpty() && heap.size() == 0;
}

bool test_top_smallest() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node node;
	e.first = 2;
	e.second = &node;

	heap.insert(e);
	auto t = heap.top();

	return	t.first == 2 &&
		*(t.second) == node &&
		t.second->heapIndex == 0 &&
		node.heapIndex == 0;
}

bool test_insert_big_small_preserves_invariant() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node big;
	e.first = 3;
	e.second = &big;

	heap.insert(e);

	Graph::Node small;
	e.first = 2;
	e.second = &small;

	heap.insert(e);

	// top is smallest and heap indices are correct for big and small node
	auto t = heap.top();
	bool ok = t.first == 2 &&
		*(t.second) == small &&
		t.second->heapIndex == 0 &&
		small.heapIndex == 0 &&
		big.heapIndex == 1;

	heap.pop();
	t = heap.top();
	ok = ok && t.first == 3 &&
		*(t.second) == big &&
		t.second->heapIndex == 0 &&
		//small.heapIndex == 1 &&  // small is no longer in the heap therefore ignore
		big.heapIndex == 0;

	return	ok;
}

bool test_insert_small_big_smallest_preserves_invariant() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node small;
	e.first = 2;
	e.second = &small;

	heap.insert(e);

	Graph::Node big;
	e.first = 3;
	e.second = &big;

	heap.insert(e);

	Graph::Node smallest;
	e.first = 1;
	e.second = &smallest;

	heap.insert(e);

	// top is smallest and heap indices are correct for big, small and smallest node
	auto t = heap.top();
	bool ok = t.first == 1 &&
		*(t.second) == smallest &&
		t.second->heapIndex == 0 &&
		smallest.heapIndex == 0 &&
		small.heapIndex == 2 &&
		big.heapIndex == 1;

	heap.pop();

	// top is small and heap indices are correct for big and smallode
	t = heap.top();
	ok = ok && t.first == 2 &&
		*(t.second) == small &&
		t.second->heapIndex == 0 &&
		//smallest.heapIndex == 0 &&	// smallest is no longer in the heap therefore ignore
		small.heapIndex == 0 &&
		big.heapIndex == 1;

	heap.pop();
	// top is big and heap indices are correct for big
	t = heap.top();
	ok = ok && t.first == 3 &&
		*(t.second) == big &&
		t.second->heapIndex == 0 &&
		//smallest.heapIndex == 0 &&	// smallest is no longer in the heap therefore ignore
		//small.heapIndex == 0 &&		// small is no longer in the heap therefore ignore
		big.heapIndex == 0;

	return	ok;
}

bool decrease_key_first_preserves_invariant() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node small;
	e.first = 2;
	e.second = &small;

	heap.insert(e);

	Graph::Node big;
	e.first = 3;
	e.second = &big;

	heap.insert(e);

	Graph::Node smallest;
	e.first = 1;
	e.second = &smallest;

	heap.insert(e);

	heap.decreaseKey(0, 0);

	// top is smallest and heap indices are correct for big, small and smallest node
	auto t = heap.top();
	bool ok = t.first == 0 &&
		*(t.second) == smallest &&
		t.second->heapIndex == 0 &&
		smallest.heapIndex == 0 &&
		small.heapIndex == 2 &&
		big.heapIndex == 1;

	heap.pop();

	// top is small and heap indices are correct for big and smallode
	t = heap.top();
	ok = ok &&
		t.first == 2 &&
		*(t.second) == small &&
		t.second->heapIndex == 0 &&
		//smallest.heapIndex == 0 &&	// smallest is no longer in the heap therefore ignore
		small.heapIndex == 0 &&
		big.heapIndex == 1;

	heap.pop();
	// top is big and heap indices are correct for big
	t = heap.top();
	ok = ok && t.first == 3 &&
		*(t.second) == big &&
		t.second->heapIndex == 0 &&
		//smallest.heapIndex == 0 &&	// smallest is no longer in the heap therefore ignore
		//small.heapIndex == 0 &&		// small is no longer in the heap therefore ignore
		big.heapIndex == 0;

	return	ok;
}

bool decrease_key_second_making_it_smallest_preserves_invariant() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node small;
	e.first = 2;
	e.second = &small;

	heap.insert(e);

	Graph::Node big;
	e.first = 3;
	e.second = &big;

	heap.insert(e);

	Graph::Node smallest;
	e.first = 1;
	e.second = &smallest;

	heap.insert(e);

	//big is in position 1
	heap.decreaseKey(1, 0);

	// top is big and heap indices are correct for big, small and smallest node
	auto t = heap.top();
	bool ok = t.first == 0 &&
		*(t.second) == big &&
		t.second->heapIndex == 0 &&
		smallest.heapIndex == 1 &&
		small.heapIndex == 2 &&
		big.heapIndex == 0;

	heap.pop();

	// top is smallest and heap indices are correct for big and smallode
	t = heap.top();
	ok = ok && t.first == 1 &&
		*(t.second) == smallest &&
		t.second->heapIndex == 0 &&
		smallest.heapIndex == 0 &&
		small.heapIndex == 1; //&&
		//big.heapIndex == 2; // big is no longer in the heap therefore ignore

	heap.pop();
	// top is small and heap indices are correct for big
	t = heap.top();
	ok = ok && t.first == 2 &&
		*(t.second) == small &&
		t.second->heapIndex == 0 &&
		smallest.heapIndex == 1 &&	// smallest is no longer in the heap therefore ignore but I will test it anyway just to make sure everything works as expected
		small.heapIndex == 0; //&&
		//big.heapIndex == 2;		// big is no longer in the heap therefore ignore

	return	ok;
}

bool decrease_key_forth_making_it_smallest_preserves_invariant() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node small;
	e.first = 2;
	e.second = &small;

	heap.insert(e);

	Graph::Node big;
	e.first = 3;
	e.second = &big;

	heap.insert(e);

	Graph::Node smallest;
	e.first = 1;
	e.second = &smallest;

	heap.insert(e);

	Graph::Node biggest;
	e.first = 4;
	e.second = &biggest;

	heap.insert(e);

	//biggest is in position 3
	heap.decreaseKey(3, 0);

	// top is biggest and heap indices are correct for biggest, big, small and smallest node
	auto t = heap.top();
	bool ok = heap.size() == 4 && !heap.isEmpty() &&
		t.first == 0 &&
		*(t.second) == biggest &&
		t.second->heapIndex == 0 &&
		smallest.heapIndex == 1 &&
		small.heapIndex == 2 &&
		big.heapIndex == 3 &&
		biggest.heapIndex == 0;

	heap.pop();

	// top is smallest and heap indices are correct for big and smallode
	t = heap.top();
	ok = ok && heap.size() == 3 && !heap.isEmpty() &&
		t.first == 1 &&
		*(t.second) == smallest &&
		t.second->heapIndex == 0 &&
		smallest.heapIndex == 0 &&
		small.heapIndex == 2 &&
		big.heapIndex == 1 &&
		biggest.heapIndex == 3;		//biggest is no longer in the heap but I will test it anyway just to make sure everything works as expected

	heap.pop();
	// top is small and heap indices are correct for big
	t = heap.top();
	ok = ok && heap.size() == 2 && !heap.isEmpty() &&
		t.first == 2 &&
		*(t.second) == small &&
		t.second->heapIndex == 0 &&
		smallest.heapIndex == 2 &&	// smallest is no longer in the heap but I will test it anyway just to make sure everything works as expected
		small.heapIndex == 0 &&		
		big.heapIndex == 1 &&
		biggest.heapIndex == 3;		//biggest is no longer in the heap but I will test it anyway just to make sure everything works as expected

	heap.pop();
	// top is big and heap indices are correct for big
	t = heap.top();
	ok = ok && heap.size() == 1 && !heap.isEmpty() &&
		t.first == 3 &&
		*(t.second) == big &&
		t.second->heapIndex == 0 &&
		smallest.heapIndex == 2 &&	// smallest is no longer in the heap but I will test it anyway just to make sure everything works as expected
		small.heapIndex == 1 &&		// small is no longer in the heap but I will test it anyway just to make sure everything works as expected
		big.heapIndex == 0 &&
		biggest.heapIndex == 3;		//biggest is no longer in the heap but I will test it anyway just to make sure everything works as expected

	return	ok;
}

bool decrease_key_forth_but_does_not_make_it_smallest_preserves_invariant() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node small;
	e.first = 2;
	e.second = &small;

	heap.insert(e);

	Graph::Node big;
	e.first = 4;
	e.second = &big;

	heap.insert(e);

	Graph::Node smallest;
	e.first = 1;
	e.second = &smallest;

	heap.insert(e);

	Graph::Node biggest;
	e.first = 5;
	e.second = &biggest;

	heap.insert(e);

	//biggest is in position 3
	heap.decreaseKey(3, 3);

	// top is smallest and heap indices are correct for biggest, big, small and smallest node
	auto t = heap.top();
	bool ok = heap.size() == 4 && !heap.isEmpty() &&
		t.first == 1 &&
		*(t.second) == smallest &&
		t.second->heapIndex == 0 &&
		smallest.heapIndex == 0 &&
		small.heapIndex == 2 &&
		big.heapIndex == 3 &&
		biggest.heapIndex == 1;

	heap.pop();

	// top is small and heap indices are correct for big and smallode
	t = heap.top();
	ok = ok && heap.size() == 3 && !heap.isEmpty() &&
		t.first == 2 &&
		*(t.second) == small &&
		t.second->heapIndex == 0 &&		//smallest is no longer in the heap but I will test it anyway just to make sure everything works as expected
		smallest.heapIndex == 3 &&
		small.heapIndex == 0 &&
		big.heapIndex == 2 &&
		biggest.heapIndex == 1;

	heap.pop();
	// top is biggest and heap indices are correct for big
	t = heap.top();
	ok = ok && heap.size() == 2 && !heap.isEmpty() &&
		t.first == 3 &&
		*(t.second) == biggest &&
		t.second->heapIndex == 0 &&
		smallest.heapIndex == 3 &&	// smallest is no longer in the heap but I will test it anyway just to make sure everything works as expected
		small.heapIndex == 2 &&		//small is no longer in the heap but I will test it anyway just to make sure everything works as expected
		big.heapIndex == 1 &&
		biggest.heapIndex == 0;

	heap.pop();
	// top is big and heap indices are correct for big
	t = heap.top();
	ok = ok && heap.size() == 1 && !heap.isEmpty() &&
		t.first == 4 &&
		*(t.second) == big &&
		t.second->heapIndex == 0 &&
		smallest.heapIndex == 3 &&	// smallest is no longer in the heap but I will test it anyway just to make sure everything works as expected
		small.heapIndex == 2 &&		// small is no longer in the heap but I will test it anyway just to make sure everything works as expected
		big.heapIndex == 0 &&
		biggest.heapIndex == 1;		//biggest is no longer in the heap but I will test it anyway just to make sure everything works as expected

	return	ok;
}

bool insert_pop_insert_preserves_invariant_and_does_not_change_previous_insertion() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node a = { 0,1,2,-1 };
	e.first = 2;
	e.second = &a;

	heap.insert(e);

	heap.pop();

	Graph::Node b = { 1,3,4,-1 };
	e.first = 4;
	e.second = &b;

	heap.insert(e);

	auto t = heap.top();

	return	heap.size() == 1 &&
		t.first == 4 &&
		*(t.second) == b &&
		t.second->heapIndex == 0 &&
		a.index == 0 &&
		a.x == 1 &&
		a.y == 2 &&
		a.heapIndex == 0 &&
		b.index == 1 &&
		b.x == 3 &&
		b.y == 4 &&
		b.heapIndex == 0;
}

bool insert_pop_insert_insert_preserves_invariant_and_does_not_change_previous_insertion() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node a = { 0,1,2,-1 };
	e.first = 2;
	e.second = &a;

	heap.insert(e);

	heap.pop();

	Graph::Node b = {1, 3,4,-1 };
	e.first = 4;
	e.second = &b;

	heap.insert(e);

	Graph::Node c = {2, 5,6,-1 };
	e.first = 5;
	e.second = &c;

	heap.insert(e);

	auto t = heap.top();

	return	heap.size() == 2 &&
		t.first == 4 &&
		*(t.second) == b &&
		t.second->heapIndex == 0 &&
		a.index == 0 &&
		a.x == 1 &&
		a.y == 2 &&
		a.heapIndex == 0 &&
		b.index == 1 &&
		b.x == 3 &&
		b.y == 4 &&
		b.heapIndex == 0 &&
		c.index == 2 &&
		c.x == 5 &&
		c.y == 6 &&
		c.heapIndex == 1;
}

bool insert_and_asks_for_key_less_than_zero() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node a = {0, 1,2,-1 };
	e.first = 2;
	e.second = &a;

	heap.insert(e);

	return !heap.hasKey(-1);
}

bool insert_pop_and_asks_for_key_equal_zero() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node a = {0, 1,2,-1 };
	e.first = 2;
	e.second = &a;

	heap.insert(e);
	heap.pop();

	return !heap.hasKey(0);
}

bool insert_and_asks_for_key_zero() {
	MinHeap heap;
	MinHeap::ElementType e;
	Graph::Node a = {0, 1,2,-1 };
	e.first = 2;
	e.second = &a;

	heap.insert(e);

	return heap.hasKey(0);
}
