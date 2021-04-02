#pragma once

#include "Node.h"

template <typename T>

class Tree {

private:

	Node<T>
		* root		{ nullptr },
		* unbalanced{ nullptr };

/************************************************************************/
/* 								DECLARATIONS	                        */
/************************************************************************/

public:

	// Return the root as a constant
	const Node<T> getRoot() const;

	// Add a new node to the tree
	void addNode(T);

	// Rebalance the tree
	void rebalance();

	// Is the tree balanced?
	explicit operator bool();

private:

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

	~Tree();
};


/************************************************************************/
/* 								DEFINITIONS		                        */
/************************************************************************/

template <typename T>
const Node<T> Tree<T>::getRoot() const { return *root; }

template <typename T>
void Tree<T>::addNode(T _value) {

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

					// Assign a brother to the new node
					cNode->updateBrother();

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

					// Assign a brother to the new node
					cNode->updateBrother();

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
}

template <typename T>
void Tree<T>::rebalance() {

}

template <typename T>
void Tree<T>::delNodes(Node<T>* _node) {

	// If there are nodes to the left, delete them
	if (_node->lChild != nullptr) { delNodes(_node->lChild); }

	// If there are nodes to the right, delete them
	if (_node->rChild != nullptr) { delNodes(_node->rChild); }

	// After the node has no children, delete it
	delete _node;
}

template <typename T>
bool Tree<T>::isBalanced(Node<T>* _node) {

	// If the node has no children, return true
	if (_node->isLeaf) { return true; }
	
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
	else { return true; }
}

template <typename T>
void Tree<T>::updateWeight(Node<T>* _node) {
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
		_node->parent->updateLength();

		// Update the current node
		_node = _node->parent;
	}
}

template <typename T>
Tree<T>::~Tree() { delNodes(root); }


/************************************************************************/
/*								OPERATORS	                            */
/************************************************************************/

template <typename T>
Tree<T>::operator bool() { return isBalanced(root); }