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

					// Assign a brother to the new node
					if (cNode->rChild != nullptr) {
						
						cNode->lChild->brother = cNode->rChild;
						cNode->rChild->brother = cNode->lChild;
					}

					// Update the current node
					cNode = cNode->lChild;

					// Update the parent's weight
					while (cNode->parent != nullptr) {

						// If the node's the left child,
						// Update the left weight of the parent
						if (cNode == cNode->parent->lChild) {
							cNode->parent->lWeight = cNode->mLength + 1;
						}

						// Otherwise, update the right weight
						else {
							cNode->parent->rWeight = cNode->mLength + 1;
						}

						// Update the parent's max length
						cNode->parent->updateLength();
						
						// Update the current node
						cNode = cNode->parent;
					}

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

					// Assign a brother to the new node
					if (cNode->lChild != nullptr) {

						cNode->rChild->brother = cNode->lChild;
						cNode->lChild->brother = cNode->rChild;
					}

					// Update the current node
					cNode = cNode->rChild;

					// Update the parent's weight
					while (cNode->parent != nullptr) {

						// If the node's the right child,
						// Update the right weight of the parent
						if (cNode == cNode->parent->rChild) {
							cNode->parent->rWeight = cNode->mLength + 1;
						}

						// Otherwise, update the left weight
						else {
							cNode->parent->lWeight = cNode->mLength + 1;
						}

						// Update the parent's max length
						cNode->parent->updateLength();

						// Update the current node
						cNode = cNode->parent;
					}

					added = true;
				}

				// Advance down the tree
				else { cNode = cNode->lChild; }
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
Tree<T>::~Tree() { delNodes(root); }


/************************************************************************/
/*								OPERATORS	                            */
/************************************************************************/

template <typename T>
Tree<T>::operator bool() { return isBalanced(root); }