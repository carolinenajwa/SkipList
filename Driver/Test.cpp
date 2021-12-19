// Caroline El Jazmi
// UW ID: 1942686
// CSS 501
// Program 2 - Skip List

// SkipList Method Testing

// Tested: No memory leak found -  All methods appear to be working as expected
// Bugs Detected: Comma lingering before removed node if removed node was located at the end of the level inside of Skip List

// This program implements a doubly linked list structure that will hold 
// integer nodes and execute a Skip List strategy. The general 
// strategy of a Skip List uses subsequent layers of doubly linked lists built upon a base list.
// Additional nodes to the skip inserted at the base-level of the list and are then 
// consulted by a random generator at insertion time. The random generator executes a
// “coin-toss” which gives each node a 50% chance of being inserted into the next 
// upper level. Generally, nodes inserted into level i have a 50% chance of being inserted 
// into level i+1. Each additional layers above the base layer contains fewer elements but 
// no new node. Skip list’s maintain a linked hierarchy between the base layer’s individual
// nodes and each additional layer stacked on top of it to better assist in fast locating, 
// insertion and deletion of nodes. 
// The Skip List is a probabilistic data structure that behaves as a sorted list. If the Skip List
// contains no subsequent levels above the base-level, the logarithmic running-time for inserting, 
// locating and deletion of nodes inside the Skip List is equivilant to of O(n). If the SkipList
// has subsequent levels above the base level, the logarithmic running-time for inserting, 
// locating and deletion of nodes inside the Skip List is equivilant to of O(log(n)).

#include <iostream>
#include "SkipList.h"

using namespace std;

int main()
{
	// Outputs 4 levels inside Skip List
	SkipList s(5);
	default_random_engine generator;
	// Generated int from 1 to 100
	uniform_int_distribution<int> uniform(1, 100);

	// Outputs 10 different integers inside base level
	for (int i = 0; i < 10; ++i) 
	{
		// Calls upon insert method to generate a SkipList
		// Insert Method generates "coin toss"
		int number = uniform(generator);
		s.insert(number);
		// Outputs integer added
		cout << "After adding " << number << endl;
		// Output Skip List
		cout << s << endl << endl;
	}
	// Calls upon erase function
	// If int is erased, int should not appear in any levels inside the list
	// If int being erased is the only node within a level - level will output "empty"
	s.erase(1);
	cout << s << endl << endl;
	s.erase(94);
	cout << s << endl << endl;
	
	// Calls upon contain function to determine if int exists inside SkipList
	// Output true
	s.contains(1);
	// Output false
	s.contains(10)
};
