// Caroline El Jazmi
// UW ID: 1942686
// CSS 501
// Program 2 - Skip List

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


#pragma once 

using namespace std;

class SkipList
{
    public:

     /** Friend operator to SkipList class
     Overloaded output used to display each level and nodes contained in those levels.
     @param os Output stream object where characters are inserted.
     @param list Objects inserted in the form of individual lists. 
     @post  If the ouput is successful, each level will display individual nodes
     it contains, seperating each integer with a comma unless integer is at the 
     end of the list or a single integer inside a level. If level is empty,
     output should display "empty"
     @return  Display output stream */
    friend ostream& operator<<(ostream& os, const SkipList& list);

/** Default Constructor for SkipList
 @param maxLevels Initialize the current height of the SkipList
 @pre Must be called in transaction scope.
 @post  Constructs a new empty Skip List by initializing the head node and tail node and
 sorteding according to integer min. to integer max. while maintain a linked hierarchy between 
 the base layer’s individual nodes and each additional layer stacked on top of it.
 @return None */
explicit SkipList(int maxLevels = 1);

/** Destructor used to destroy object and free memory allocated by object.
 @pre Skip List is not empty
 @post If successful, Objects and memory allocated by object are destroyed
 @return No return */
virtual ~SkipList();

/** Adds a new item in a sorted order to the Skip List
 @param item Takes int item as a parameter and inserts a node containing the data equiv. to item at the appropriate place in Skip List.
 @pre The item is not currently in the Skip List
 @pre alsoHigher() function to generate random numbers and a threshold of 0.5 for continuing to increment the level and new entry
	  in Skip List. 
 @post If successful, item is stored in the Skip List and if alsoHigher returns true, any subsequent levels on top of the base.
 @post If duplicate item is found, output that item can not be inserted
 @return True if addition was successful, or false if not. */
bool insert(int item);

/** Removes existing item from Skip List
 @param item Takes int item as a parameter and removes a node containing the data equiv. to item at the appropriate place in Skip List.
 @pre Skip List is not empty
 @pre Node containing data equiv. to item is exists inside Skip List
 @post If successful, node containing the data equiv. to item is removed from the base level and any subsequent levels above. 
 @return Returns true if removal was successful, false if not. */
bool erase(int item);

/** Tests whether Skip List contains a given entry.
 @param item Takes int item as a parameter and searches for node 
             containing the data equiv. to item at the appropriate place in Skip List.
 @pre Skip List is not empty
 @post If successful, outputs "List contains number (item)" if Skip List contain item
       and "List does not contain number (item)" if Skip List does not contain item
 @return bool true is item found inside Skip List, bool false if item is not found inside Skip List */
bool contains(int item) const;

    private:
    struct SkipListNode {

   // Convenience constructor to create node, set its data, and set all pointers to nullptr
   explicit SkipListNode(int data);

   // data for SNode
   int data_;

   // link to next at same level
   SkipListNode* next_;

   // link to previous at same level
   SkipListNode* prev_;

   // link up one level
   SkipListNode* upLevel_;

   // link down one level
   SkipListNode* downLevel_;

};

// Stores int maxLevels
int maxLevels_;

// Stores int level
int level;

// Array of maxLevels_ SkipListNode pointers as head pointers.
SkipListNode** heads_;

// Array of maxLevels_ SkipListNode pointers as tail pointers.
SkipListNode** tails_;

// Pointer to head node
SkipListNode* head;

/** Given a new pointer to a SkipListNode, place it before the given nextNode. Assists with insertion. 
 @param newNode Node to be inserted
 @param nextNode Node located next to newNode that will be inserted
 @param level levels present in list that can accept newNodes
 @pre Skip List is not empty
 @pre NextNode is not nullptr
 @post If successful, newNode is inserted behind nextNode
 @return No return */
void addBefore(SkipListNode* newNode, SkipListNode* nextNode, int level);

/** Generates deterministic random numbers with 50% probability for continuing to increment level and nodes within that level
 @pre Skip List is not empty
 @post If successful, generator returns true 50% of the time and false 50% of the time
 @return return true - node is added to subsequent levels on top of the base
		 return false - random generator stops */
bool alsoHigher() const;

}; // end SkipList
