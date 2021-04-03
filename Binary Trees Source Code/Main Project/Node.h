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

public:
	
	// Update the node's information
	void update();

private:

	// Update the max length of the node
	void updateLength();

	// Update a node's brother
	void updateBrother();
};


/************************************************************************/
/* 								DEFINITIONS		                        */
/************************************************************************/

template <typename T>
void Node<T>::update() {
	
	// Ignore empty nodes
	if (this != nullptr) {

		updateLength();
		updateBrother();

		// Update it's leaf status
		if (lChild == nullptr && rChild == nullptr) {
			
			// Update it's length and weight
			mLength = 0;
			lWeight = 0;
			rWeight = 0;

			isLeaf = true;
		}
		else { isLeaf = false; }
	}
}

template <typename T>
void Node<T>::updateLength() {
	mLength = std::max(lWeight, rWeight);
}

template <typename T>
void Node<T>::updateBrother() {
	
	// The node's a root
	if (parent == nullptr) { brother = nullptr; }

	// Check if there are two children
	else if (parent->lChild != nullptr && parent->rChild != nullptr) {
		
		// Create the relationship
		parent->rChild->brother = parent->lChild;
		parent->lChild->brother = parent->rChild;
	}
}