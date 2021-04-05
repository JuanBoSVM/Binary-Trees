#include <iostream>
#include <vector>

#include "BinaryTree.h"


/************************************************************************/
/* 									MAIN                                */
/************************************************************************/

int main() {
	BinaryTree<int> Tree;
	std::vector<int> input{ 10, 20, 15, 25, 30, 16, 18, 19 };

	for (int i : input) {
		
		// Add the new input to the tree
		// only if it doesn't exist already
		if (!Tree.contains(i)) { Tree.addNode(i); }
		
		// Update the tree's info
		Tree.update();

		// Check if the tree's unbalanced
		if (!Tree) {
			
			// Print the before version
			

			// Rebalance the tree
			Tree.rebalance();
		}
		
		//Print the result

	}
}