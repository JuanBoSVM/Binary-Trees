#pragma once

#include <cmath>

template <typename T>

class Node {

public:

	T value{ 0 };
	Node
		* parent { nullptr },	// Parent
		* brother{ nullptr },	// Brother
		* lChild { nullptr },	// Left Child
		* rChild { nullptr };	// Right Child
	int
		mLength{ 0 },	// Max Length
		lWeight{ 0 },	// Left Weight
		rWeight{ 0 };	// Right Weight
	bool isLeaf{ true };// Is the node a leaf?

/************************************************************************/
/* 								DECLARATIONS	                        */
/************************************************************************/
	
	// Update the max length of the node
	void updateLength();

	// Update a node's brother
	void updateBrother();
};


/************************************************************************/
/* 								DEFINITIONS		                        */
/************************************************************************/

template <typename T>
void Node<T>::updateLength() { mLength = std::max(lWeight, rWeight); }

template <typename T>
void Node<T>::updateBrother() {
	
	// Check if there are two children
	if (parent->lChild != nullptr && parent->rChild != nullptr) {
		
		// Create the relationship
		parent->rChild->brother = parent->lChild;
		parent->lChild->brother = parent->rChild;
	}
}