#pragma once

#include "Node.h"

/************************************************************************/
/* 							SUPPORT STRUCTURES			                */
/************************************************************************/

// Type of unbalance
enum class UB_TYPE {

	LL,	// Left-left
	RR,	// Right-right
	LR,	// Left-Right
	RL	// Right-Left
};

template <typename T>

class BinaryTree {

private:

	Node<T>
		* root       { nullptr },
		* unbalanced { nullptr },
		* lastChecked{ nullptr };
	
public:

	int
		nodes { 0 },
		length{ 0 };


/************************************************************************/
/* 								DECLARATIONS	                        */
/************************************************************************/

public:

	// Return a pointer to the root
	Node<T>* getRoot() const;

	// Does the tree contain a specific node?
	bool contains(T);

	// Add a new node to the tree
	void addNode(T);

	// Get the type of unbalance
	UB_TYPE getUbType(Node<T>*);

	// Rebalance the tree
	void rebalance();

	// Update every node in the tree
	void update();

	// Is the tree balanced?
	explicit operator bool();

private:

	// Start updating from the root
	void update(Node<T>*);

	// Pull the tree to the left
	void pullLeft(Node<T>*);

	// Pull the tree to the right
	void pullRight(Node<T>*);

	// Delete all the nodes from a tree
	void delNodes(Node<T>*);

	// Is the node valanced?
	bool isBalanced(Node<T>*);

	// Update a Node's weight
	void updateWeight(Node<T>*);


/************************************************************************/
/* 								DESTRUCTOR	                            */
/************************************************************************/

public:

	~BinaryTree();
};


/************************************************************************/
/* 								DEFINITIONS		                        */
/************************************************************************/

template <typename T>
Node<T>* BinaryTree<T>::getRoot() const { return root; }

template <typename T>
bool BinaryTree<T>::contains(T _data) {
	
	Node<T>* cNode{ root };

	// Main search loop
	while (cNode != nullptr) {

		// The node was found
		if (cNode->value == _data) { return true; }

		// If the data looked for is lower than the current node,
		// advance to the left side of the tree
		else if (_data < cNode->value) {
			lastChecked = cNode;
			cNode = cNode->lChild;
		}

		// Otherwise, advance to the right side
		else {
			lastChecked = cNode;
			cNode = cNode->rChild;
		}
	}

	// The node wasn't found
	return false;
}

template <typename T>
void BinaryTree<T>::addNode(T _value) {

	// Make sure that the tree doesn't contain the node already
	if (!contains(_value)) {

		// Save it as root if there's none
		if (root == nullptr) {
			root = new Node<T>{ _value };
		}

		// Save it as a child
		else {

			// Current Node
			Node<T>* cNode{ lastChecked };

			// Should the new node go to the left of the last checked one?
			bool left{ _value < cNode->value };

			// Save the new node to the correct side of the
			// last checked one, with it as its parent
			if (left) { cNode->lChild = new Node<T>{ _value, cNode }; }
			else { cNode->rChild = new Node<T>{ _value, cNode }; }
		}

		// Reset the last checked node,
		// and add one to the count of nodes
		lastChecked = nullptr;
		nodes++;
	}
}

template <typename T>
UB_TYPE BinaryTree<T>::getUbType(Node<T>* _ubNode) {

	// First step of the unbalance
	bool left{ _ubNode->lWeight > _ubNode->rWeight };

	// The first step is left
	if (left) {
		
		// Second step of the unbalance
		left = _ubNode->lChild->lWeight > _ubNode->lChild->rWeight;

		// Combine the two steps
		if (left) { return UB_TYPE::LL; }
		else { return UB_TYPE::LR; }
	}

	// The first step is right
	else {

		// Second step of the unbalance
		left = _ubNode->rChild->lWeight > _ubNode->rChild->rWeight;

		// Combine the two steps
		if (left) { return UB_TYPE::RL; }
		else { return UB_TYPE::RR; }
	}
}

template <typename T>
void BinaryTree<T>::rebalance() {
	
	UB_TYPE cUnbalance{ getUbType(unbalanced) }; // Current unbalance

	switch (cUnbalance) {

	// Pull to the left
	case UB_TYPE::LR:
		pullLeft(unbalanced->lChild);

	// Pull to the right
	case UB_TYPE::LL:
		pullRight(unbalanced);
		break;


	// Pull to the right
	case UB_TYPE::RL:
		pullRight(unbalanced->rChild);

	// Pull to the left
	case UB_TYPE::RR:
		pullLeft(unbalanced);
		break;
	}

	// Update the tree after the changes
	unbalanced = nullptr;
	update(root);
}

template <typename T>
void BinaryTree<T>::update() {
	
	// Update each node, starting from the root
	update(root);

	// Update the length of the tree
	length = root->mLength;
}

template <typename T>
void BinaryTree<T>::update(Node<T>* _cNode) {

	// If the tree's just the root, ignore it
	if (!root->isLeaf()) {

		// Look for the leafs from the left side
		if (_cNode->lChild != nullptr) { update(_cNode->lChild); }
		
		// Look for the leafs from the right side
		if (_cNode->rChild != nullptr) { update(_cNode->rChild); }

		// If there are no further nodes below,
		// update the node's weight
		if (_cNode->isLeaf()) { updateWeight(_cNode); }

		// Update the rest of the node's info
		else { _cNode->update(); }
	}
}

template <typename T>
void BinaryTree<T>::pullRight(Node<T>* _head) {

	// Check if we have to move the root
	if (_head == root) {

		// List of modified nodes
		Node<T>
			* nodeA{ root },
			* nodeB{ root->lChild },
			* nodeC{ root->lChild->rChild };

		// Reassign the tree's root to it's left child
		// and get rid of its parent since it's the root
		root = nodeB;
		root->parent = nullptr;

		// The new root now has a child
		// and the former root now has a parent
		nodeA->parent = nodeB;
		nodeB->rChild = nodeA;

		// If node C exists, adjust its
		// relationship with node A
		if (nodeC != nullptr) {

			nodeA->lChild = nodeC;
			nodeC->parent = nodeA;
		}	

		// Otherwise, node A has no child
		else { nodeA->lChild = nullptr; }

		// Update the information
		// of the modified nodes
		nodeA->update();
		nodeB->update();
		nodeC->update();
	}

	// The root of the tree won't change
	else {

		// List of modified nodes
		Node<T>
			* nodeA{ _head },
			* nodeB{ _head->lChild },
			* nodeC{ _head->lChild->rChild },
			* nodeD{ _head->parent };

		// Adjust node D's relationship with node A,
		// depending on which child node A is
		if (nodeD->lChild == nodeA) { nodeD->lChild = nodeB; }
		else { nodeD->rChild = nodeB; }
		
		nodeB->parent = nodeD;

		// Adjust node A and B's relationship
		nodeA->parent = nodeB;
		nodeB->rChild = nodeA;

		// If node C exists, adjust its
		// relationship with node A
		if (nodeC != nullptr) {

			nodeA->lChild = nodeC;
			nodeC->parent = nodeA;
		}

		// Otherwise, node A has no child
		else { nodeA->lChild = nullptr; }

		// Update the information
		// of the modified nodes
		nodeA->update();
		nodeB->update();
		nodeC->update();
		nodeD->update();
	}
}

template <typename T>
void BinaryTree<T>::pullLeft(Node<T>* _head) {

	// Check if we have to move the root
	if (_head == root) {

		// List of modified nodes
		Node<T>
			* nodeA{ root },
			* nodeB{ root->rChild },
			* nodeC{ root->rChild->lChild };

		// Reassign the tree's root to it's left child
		// and get rid of its parent since it's the root
		root = nodeB;
		root->parent = nullptr;

		// The new root now has a child
		// and the former root now has a parent
		nodeA->parent = nodeB;
		nodeB->lChild = nodeA;

		// If node C exists, adjust its
		// relationship with node A
		if (nodeC != nullptr) {

			nodeA->rChild = nodeC;
			nodeC->parent = nodeA;
		}

		// Otherwise, node A has no child
		else { nodeA->rChild = nullptr; }

		// Update the information
		// of the modified nodes
		nodeA->update();
		nodeB->update();
		nodeC->update();
	}

	// The root of the tree won't change
	else {

		// List of modified nodes
		Node<T>
			* nodeA{ _head },
			* nodeB{ _head->rChild },
			* nodeC{ _head->rChild->lChild },
			* nodeD{ _head->parent };

		// Adjust node D's relationship with node A,
		// depending on which child node A is
		if (nodeD->rChild == nodeA) { nodeD->rChild = nodeB; }
		else { nodeD->lChild = nodeB; }

		nodeB->parent = nodeD;

		// Adjust node A and B's relationship
		nodeA->parent = nodeB;
		nodeB->lChild = nodeA;

		// If node C exists, adjust its
		// relationship with node A
		if (nodeC != nullptr) {

			nodeA->rChild = nodeC;
			nodeC->parent = nodeA;
		}

		// Otherwise, node A has no child
		else { nodeA->rChild = nullptr; }

		// Update the information
		// of the modified nodes
		nodeA->update();
		nodeB->update();
		nodeC->update();
		nodeD->update();
	}
}

template <typename T>
void BinaryTree<T>::delNodes(Node<T>* _node) {

	// If there are nodes to the left, delete them
	if (_node->lChild != nullptr) { delNodes(_node->lChild); }

	// If there are nodes to the right, delete them
	if (_node->rChild != nullptr) { delNodes(_node->rChild); }

	// After the node has no children, delete it
	delete _node;
}

template <typename T>
bool BinaryTree<T>::isBalanced(Node<T>* _node) {

	// If the node has no children, return true
	if (_node->isLeaf()) {
		unbalanced = nullptr;
		return true;
	}
	
	// If there are nodes to the left, check if they're balanced
	if (_node->lChild != nullptr) { 
		
		// If the child isn't balanced, return false
		if (!isBalanced(_node->lChild)) { return false; }
	}

	// If there are nodes to the right, check if they're balanced
	if (_node->rChild != nullptr) {
		
		// If the child isn't balanced, return false
		if (!isBalanced(_node->rChild)) { return false; }
	}

	// If the difference in weight is greater
	// than 1, the tree's unbalanced
	if (std::abs(_node->lWeight - _node->rWeight) > 1) {
		unbalanced = _node;
		return false;
	}

	// The node's balanced
	else {
		unbalanced = nullptr;
		return true;
	}
}

template <typename T>
void BinaryTree<T>::updateWeight(Node<T>* _node) {
	
	// If the node's a leaf, update it's info
	if (_node->isLeaf()) { _node->update(); }

	// Look for the root, parent from parent
	while (_node->parent != nullptr) {

		// Is the node the left child?
		bool left{ _node == _node->parent->lChild };

		// Update the left weight of the parent
		if (left) { _node->parent->lWeight = _node->mLength + 1; }

		// Update the right weight
		else { _node->parent->rWeight = _node->mLength + 1; }

		// Update the parent's max length
		_node->parent->update();

		// Update the current node
		_node = _node->parent;
	}
}

template <typename T>
BinaryTree<T>::~BinaryTree() { delNodes(root); }


/************************************************************************/
/*								OPERATORS	                            */
/************************************************************************/

template <typename T>
BinaryTree<T>::operator bool() {
	if (root == nullptr) { return true; }
	else { return isBalanced(root); }
}