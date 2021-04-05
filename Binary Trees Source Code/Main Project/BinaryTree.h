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
		* root		{ nullptr },
		* unbalanced{ nullptr };
	int
		nodes { 0 },
		length{ 0 };


/************************************************************************/
/* 								DECLARATIONS	                        */
/************************************************************************/

public:

	// Return the root as a constant
	const Node<T> getRoot() const;

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
const Node<T> BinaryTree<T>::getRoot() const { return *root; }

template <typename T>
bool BinaryTree<T>::contains(T _data) {
	
	Node<T>* cNode{ root };

	// Main search loop
	while (cNode != nullptr) {

		// The node was found
		if (cNode->value == _data) { return true; }

		// If the data looked for is lower than the current node,
		// advance to the left side of the tree
		else if (_data < cNode->value) { cNode = cNode->lChild; }

		// Otherwise, advance to the right side
		else { cNode = cNode->rChild; }
	}

	// The node wasn't found
	return false;
}

template <typename T>
void BinaryTree<T>::addNode(T _value) {

	// Save it as root if there's none
	if (root == nullptr) {
		root = new Node<T>{ _value };
	}

	// Save it as a child
	else {

		Node<T>* cNode{ root };		// Current Node
		bool added{ false };		// Has a new node been added?

		while (!added) {
			
			// Go to the left
			if (_value < cNode->value) {

				// Is the node free?
				if (cNode->lChild == nullptr) {
					
					// Save the new node there with
					// the current one as its parent
					cNode->lChild = new Node<T>{ _value, cNode };

					// The parent is no longer a root
					cNode->isLeaf = false;

					// Update the current node
					// to the newly created one
					cNode = cNode->lChild;

					// Update the new node's info
					cNode->update();

					// Update the tree's
					// weight up the line
					updateWeight(cNode);

					added = true;
				}

				// Advance down the tree
				else { cNode = cNode->lChild; }
			}

			// Go to the right
			else if (cNode->value < _value) {
				
				// Is the node free?
				if (cNode->rChild == nullptr) {

					// Save the new node there with
					// the current one as its parent
					cNode->rChild = new Node<T>{ _value, cNode };

					// The parent is no longer a root
					cNode->isLeaf = false;

					// Update the current node
					// to the newly created one
					cNode = cNode->rChild;

					// Update the new node's info
					cNode->update();

					// Update the tree's
					// weight up the line
					updateWeight(cNode);

					added = true;
				}

				// Advance down the tree
				else { cNode = cNode->rChild; }
			}
		}
	}

	nodes++;
}

template <typename T>
UB_TYPE BinaryTree<T>::getUbType(Node<T>* _ubNode) {

	// The first step is left
	if (_ubNode->lWeight > _ubNode->rWeight) {
		
		// The second step is left
		if (_ubNode->lChild->lWeight > _ubNode->lChild->rWeight) {
			
			return UB_TYPE::LL;
		}

		// The second step is right
		else { return UB_TYPE::LR; }
	}

	// The first step is right
	else {

		// The second step is left
		if (_ubNode->rChild->lWeight > _ubNode->rChild->rWeight) {

			return UB_TYPE::RL;
		}

		// The second step is right
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
	if (!root->isLeaf) {

		// Look for the leafs from the left side
		if (_cNode->lChild != nullptr) { update(_cNode->lChild); }
		
		// Look for the leafs from the right side
		if (_cNode->rChild != nullptr) { update(_cNode->rChild); }

		// If there are no further nodes below,
		// update the node's weight
		if (_cNode->isLeaf) { updateWeight(_cNode); }

		// Update the rest of the node's info
		else { _cNode->update(); }
	}
}

template <typename T>
void BinaryTree<T>::pullRight(Node<T>* _head) {

	// Check if we have to move the root
	if (_head == root) {

		// Reassign the tree's root to it's left child
		root = _head->lChild;

		// The former root now has a parent
		root->parent->parent = root;

		// Adjust the former root's left child
		root->parent->lChild = root->rChild;

		// Make that child's parent the former root
		// if it has a child
		if (root->rChild != nullptr) {

			root->rChild->parent = root->parent;
		}
		
		// Make the former root the right child
		// of the new root
		root->rChild = root->parent;

		// Erase the new root's parent
		root->parent = nullptr;

		// List of modified nodes
		Node<T>
			* mNode01{ root },
			* mNode02{ root->lChild },
			* mNode03{ root->rChild },
			* mNode04{ root->lChild->lChild },
			* mNode05{ root->rChild->rChild };

		// Update the information
		// of the modified nodes
		mNode01->update();
		mNode02->update();
		mNode03->update();
		mNode04->update();
		mNode05->update();
	}

	// The root of the tree won't change
	else {

		// List of modified nodes
		Node<T>
			* nodeA{ _head },
			* nodeB{ _head->parent },
			* nodeC{ _head->lChild },
			* nodeD{ _head->lChild->rChild };

		// Change node B's right child to node C
		// and adjust node C's parent
		nodeB->rChild = nodeC;
		nodeC->parent = nodeB;
		
		// Change node C's right child to node A
		// and adjust node A's parent to node C
		nodeC->rChild = nodeA;
		nodeA->parent = nodeC;

		// Change node A's left child to node D
		// and adjust node D's parent as long as it isn't empty
		nodeA->lChild = nodeD;
		if (nodeD != nullptr) { nodeD->parent = nodeA; }

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
		
		// Reassign the tree's root to it's right child
		root = _head->rChild;

		// The former root now has a parent
		root->parent->parent = root;

		// Adjust the former root's right child
		root->parent->rChild = root->lChild;

		// Make that child's parent the former root
		// if it has a child
		if (root->lChild != nullptr) {

			root->lChild->parent = root->parent;
		}

		// Make the former root the left child
		// of the new root
		root->lChild = root->parent;

		// Erase the new root's parent
		root->parent = nullptr;

		// List of modified nodes
		Node<T>
			* mNode01{ root },
			* mNode02{ root->lChild },
			* mNode03{ root->rChild },
			* mNode04{ root->lChild->lChild },
			* mNode05{ root->rChild->rChild };

		// Update the information
		// of the modified nodes
		mNode01->update();
		mNode02->update();
		mNode03->update();
		mNode04->update();
		mNode05->update();
	}

	// The root of the tree won't change
	else {

		// List of modified nodes
		Node<T>
			* nodeA{ _head },
			* nodeB{ _head->parent },
			* nodeC{ _head->rChild },
			* nodeD{ _head->rChild->lChild };

		// Change node B's right child to node C
		// and adjust node C's parent
		nodeB->lChild = nodeC;
		nodeC->parent = nodeB;

		// Change node C's right child to node A
		// and adjust node A's parent to node C
		nodeC->lChild = nodeA;
		nodeA->parent = nodeC;

		// Change node A's left child to node D
		// and adjust node D's parent as long as it isn't empty
		nodeA->rChild = nodeD;
		if (nodeD != nullptr) { nodeD->parent = nodeA; }

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
	if (_node->isLeaf) {
		unbalanced = nullptr;
		return true;
	}
	
	// If there are nodes to the left, check if they're balanced
	if (_node->lChild != nullptr) { isBalanced(_node->lChild); }

	// If there are nodes to the right, check if they're balanced
	if (_node->rChild != nullptr) { isBalanced(_node->rChild); }

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
	if (_node->isLeaf) { _node->update(); }

	// Look for the root, parent from parent
	while (_node->parent != nullptr) {

		// If the node's the left child,
		// Update the left weight of the parent
		if (_node == _node->parent->lChild) {
			_node->parent->lWeight = _node->mLength + 1;
		}

		// Otherwise, update the right weight
		else {
			_node->parent->rWeight = _node->mLength + 1;
		}

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
BinaryTree<T>::operator bool() { return isBalanced(root); }