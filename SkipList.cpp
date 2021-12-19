// Caroline El Jazmi
// UW ID: 1942686
// CSS 501
// Program 2 - Skip List


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

#include <climits>
#include <iostream>
#include <random>
#include <vector>
#include "SkipList.h"

using namespace std;

/** Overloaded output used to display each level and nodes contained in those levels.
 @param os Output stream object where characters are inserted.
 @param list Objects inserted in the form of individual lists. 
 @post  If the ouput is successful, each level will display individual nodes
 it contains, seperating each integer with a comma unless integer is at the 
 end of the list or a single integer inside a level. If level is empty,
 output should display "empty"
 @return  Display output stream */
ostream& operator<<(ostream& os, const SkipList& list)
{
	// Display each level
	for(int i=list.maxLevels_-1;i>=0;i--)
	{
		os<<"Level: "<<i<<" -- ";

		// Allocate nodes to bypass infinity nodes
		// Allocate node (commaNode), to remove danging comma at the end of each level
		SkipList::SkipListNode *frontNode=list.heads_[i]->next_;
		SkipList::SkipListNode *lastNode=list.tails_[i];
		SkipList::SkipListNode *commaNode=list.tails_[i]->prev_;

		// While the list is not empty and node is not a single number or last number in the list,
		// display value of individual nodes seperated by a comma
		while(frontNode->next_ != nullptr && frontNode != commaNode && frontNode != lastNode)
		{
			os<<frontNode->data_<<", ";
			frontNode=frontNode->next_;
		}
		// If list contains a single node or is the last number inside the list, 
		// dislay node's value without a comma following it
		if(frontNode == commaNode)
		{
			os<<frontNode->data_;
		}
		// Display Empty level
		if(list.heads_[i]->next_->next_== nullptr)
		{
			os<<"Empty";
		}

	os<< endl;

	}
	return os;
}

/** Default Constructor for SkipList
 @param maxLevels Initialize the current height of the SkipList
 @pre Must be called in transaction scope.
 @post  Constructs a new empty Skip List by initializing the head node and tail node and
 sorteding according to integer min. to integer max. while maintain a linked hierarchy between 
 the base layer’s individual nodes and each additional layer stacked on top of it.
 @return None */
SkipList::SkipList(int maxLevels) 
{
	// User input of # of levels stored in maxLevels_
	this->maxLevels_=maxLevels;

	// Allocated head and tail of list
	this->heads_=new SkipListNode *[maxLevels];
	this->tails_=new SkipListNode *[maxLevels];
	
  	int i;
 	 // For every level inside list, inititialize head and tail
	for(i=0;i<maxLevels_;i++)
	{
		// Set head as mininum int. - beginning of list
		this->heads_[i]=new SkipListNode(INT_MIN);
		// Set tail as max. int. - end of list
		this->tails_[i]=new SkipListNode(INT_MAX);
		// Connect heads and tail of each list
		this->heads_[i]->next_=this->tails_[i];
		this->tails_[i]->prev_=this->heads_[i];

		// If list has more than 1 level, connect each upper/lower level
		if(i>0)
		{
			// Connect head from lower level to upper level 
			this->heads_[i]->downLevel_=this->heads_[i-1];
			this->heads_[i-1]->upLevel_=this->heads_[i];
			// Connect tails from lower level to upper level
			this->tails_[i]->downLevel_=this->tails_[i-1];
			this->tails_[i-1]->upLevel_=this->tails_[i];
		}
	}

	// If base level only - no subsequent levels above, set head/tail to upper/lower levels to null. 
	this->heads_[0]->downLevel_= nullptr;
	this->heads_[maxLevels-1]->upLevel_= nullptr;
	this->tails_[0]->downLevel_= nullptr;
	this->tails_[maxLevels-1]->upLevel_= nullptr;
	}

/** Convenience Node Constructor
 @param item Initialize the current item to data inside of SkipList
 @pre Must be called in transaction scope.
 @post  Creates nodes, set its data, and set all pointers to nullptr
 @return None */
SkipList::SkipListNode::SkipListNode(int item) 
{	 
	this->data_=item;
	// Avoid dangling nodes- Set pointers to nullptr
	this->next_= nullptr;
	this->prev_= nullptr;
	this->upLevel_= nullptr;
	this->downLevel_= nullptr;
}

/** Adds a new item in a sorted order to the Skip List
 @param item Takes int item as a parameter and inserts a node containing the data equiv. to item at the appropriate place in Skip List.
 @pre The item is not currently in the Skip List
 @pre alsoHigher() function to generate random numbers and a threshold of 0.5 for continuing to increment the level and new entry
	  in Skip List. 
 @post If successful, item is stored in the Skip List and if alsoHigher returns true, any subsequent levels on top of the base.
 @post If duplicate item is found, output that item can not be inserted
 @return True if addition was successful, or false if not. */
bool SkipList::insert(int item) 
{
	// Allocate new node pointer (current) and point it to the node after head of list. 
	SkipListNode *nextNode=heads_[0]->next_;

	// While list is not empty, and current node's value is smaller than item's value, current node travels to the right
	while(nextNode->next_!= nullptr && nextNode->data_ < item)
		nextNode=nextNode->next_;			

	// If duplicate node is found - return false
	if(nextNode->data_ == item)
		return false;
		cout<< "Duplicate found, "<< item <<" can not be inserted into Skip List" <<endl;
	
	// Allocate space for new node storing item's value.
	SkipListNode *newNode=new SkipListNode(item);
	// Pass newNode inside of addBefore method to determine organized location for new node. 
	addBefore(newNode, nextNode, level);

	// Determine if new node should move up one level
	// Int count to ensure nodes are node placed passed max. levels inside of list structure. 
	int currentLevels=1;
	// While current # of levels is less than max. levels inside of list structure.
	while(currentLevels<maxLevels_)
	{
		// Random Gen.
		if(alsoHigher())
		{
			// If returns true - Allocate new node storing item in upper level
			SkipListNode *UpperNode=new SkipListNode(item);
			// Connect new upper level node to new node in lower level
			UpperNode->downLevel_= newNode;
			// Connect new node in lower level to new node in upper level
			newNode->upLevel_=UpperNode;	
			// Allocate new node pointing to node previous to newNode
			SkipListNode *node=newNode->prev_;
			
			// While upper level of node is not empty
			while(node->upLevel_ == nullptr)
			// Connect node previous to newNode to upper level
				node=node->prev_;			
				node=node->upLevel_;

				// Input newNode inside of list
				node=node->next_;			
				addBefore(UpperNode, node, maxLevels_);

				// Set newNode to UpperNode node
				newNode=UpperNode;
				// Advance 1 level
				currentLevels++;
		}
    		// If AlsoHigher method returns false - break
    		else
			
      			break;
		}	
	// If new node has successfully been added - return true
	return true;
}

/** Generates deterministic random numbers with 50% probability for continuing to increment level and nodes within that level
 @pre Skip List is not empty
 @post If successful, generator returns true 50% of the time and false 50% of the time
 @return If return true - node is added to subsequent levels on top of the base
         If return false - random generator stops */
bool SkipList::alsoHigher() const
{
   static default_random_engine generator;    // generator
   static bernoulli_distribution coin(0.5);  // distribution
   return coin(generator);
}

/** Given a new pointer to a SkipListNode, place it before the given nextNode. Assists with insertion. 
 @param newNode Node to be inserted
 @param nextNode Node located next to newNode that will be inserted
 @param level levels present in list that can accept newNodes
 @pre Skip List is not empty
 @pre NextNode is not nullptr
 @post If successful, newNode is inserted behind nextNode
 @return No return */
void SkipList::addBefore(SkipListNode *newNode, SkipListNode *nextNode, int level) 
{
	// If the list is not empty, new node is not null
	// and newNode's item is smaller than the next_Node's item
	// If newNode is not null and level is not empty and newNode's value is less than the next nodes value
	(newNode != nullptr && nextNode != nullptr && newNode->data_ < nextNode->data_);

	// Allocate new node pointing to the node before nextNode
	SkipListNode *previousNode=nextNode->prev_;
	// Input new node 
	previousNode->next_=newNode;
	newNode->prev_=previousNode;
	newNode->next_=nextNode;
	nextNode->prev_=newNode;
}

/** Removes existing item from Skip List
 @param item Takes int item as a parameter and removes a node containing the data equiv. to item at the appropriate place in Skip List.
 @pre Skip List is not empty
 @pre Node containing data equiv. to item is exists inside Skip List
 @post If successful, node containing the data equiv. to item is removed from the base level and any subsequent levels above. 
 @return Returns true if Skip List contains item, false if not. 
 @return if true - outputs "item has been removed from Skip List"*/
bool SkipList::erase(int item) 
{
	bool ret = false;
	// Allocate memory for nodes
	SkipListNode* node = heads_[maxLevels_-1], *delt = nullptr;
	SkipListNode* last = tails_[maxLevels_-1];
	// While list is not empty
	while (node) 
	{
		if (!delt) 
		{
			// While list is not empty, and current node's value is smaller than item's value, current node travels to the right
			while (node->next_ && node->next_->data_ < item) node = node->next_;
			// If node containing data equiv. to item - node is stored and traverse continues
			if (node->next_ && node->next_->data_ == item)
			{
				delt = node->next_;
				ret = true;
				cout<<"Number "<< item <<" has been removed from Skip List" <<endl;
			}
		}
		// Deaallocate node in current level
		if (delt) 
		{ 
			// While list is not empty, and current node's value is smaller than item's value, current node travels to the right
			while (node->next_ != delt) node = node->next_;
			// If node is equiv. to delt node & delt is not located at the end of the list
			if(node->next_ && node->next_ == delt && node->next_ != nullptr)
			{
				// Remove node from list and continue search by traversing down 1 level
				SkipListNode* temp = node->next_;
				delt = node->next_->downLevel_;
				node->next_ = node->next_->next_;
				delete temp;
			}
			// If node is equiv. to delt node & delt is is located at the end of the list
			else if (node->next_ && node->next_ == delt && node->next_->next_== nullptr)
			{
				// Remove node from list and continue search by traversing down 1 level
				SkipListNode* temp = node->next_;
				delt = node->next_->downLevel_;
				node->prev_->next_= node->next_;
				node=last;
				delete temp;			
			}

		}
	// Node continues search by traversing down 1 level
	node = node->downLevel_;
	}
	delete delt;
	// If item is not found
	return ret;
	cout<<"Number "<< item <<" does not exist inside Skip List" <<endl;
}


/** Tests whether Skip List contains a given entry.
 @param item Takes int item as a parameter and searches for node 
             containing the data equiv. to item at the appropriate place in Skip List.
 @pre Skip List is not empty
 @post If successful, outputs "List contains number (item)" if Skip List contain item
	   and "List does not contain number (item)" if Skip List does not contain item
 @return bool true is item found inside Skip List, bool false if item is not found inside Skip List */
bool SkipList::contains(int item) const 
{
	bool ret = false;
	// Allocate memory for node pointing to head of list at highest level inside Skip List
	SkipListNode *current=heads_[maxLevels_-1];
	// While list is not empty
	while(current != nullptr)
	{
		// While list is not empty, and current node's value is smaller than item's value, current node travels to the right
		while(current->next_->data_<item) current=current->next_;			
		// If node containing data equiv. to item is found - return true
		if(current->next_->data_==item)
			return true;
			cout<< "Skip List contains number "<< item <<endl;

		// Node continues search by traversing down 1 level
		current=current->downLevel_;
	}
	// If node containing data equiv. to item is not found - return false
	cout<< "Skip List does not contain number "<< item <<endl;
	return ret;
}

/** Destructor used to destroy object and free memory allocated by object.
 @pre Skip List is not empty
 @post If successful, Objects and memory allocated by object are destroyed
 @return No return */
SkipList::~SkipList() 
{
	// Allocate new nodes
	SkipListNode *node,*temp;

	// Loop through each level, delete nodes individually
	int i;
	for(i=maxLevels_-1;i>=0;i--)
	{
	 	node=heads_[i];

		while(node!= nullptr)
		{
			  temp=node;

			node=node->next_;
			delete temp;
		}
	}

	// Deallocate head and tail array
	delete [] heads_;
	delete [] tails_;
};
