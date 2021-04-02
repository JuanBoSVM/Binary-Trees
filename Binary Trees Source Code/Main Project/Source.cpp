#include <iostream>
#include <vector>

#include "Tree.h"


/************************************************************************/
/* 									MAIN                                */
/************************************************************************/

int main() {
	Tree<int> mainTree;
	std::vector<int> input{ 10, 8, 11, 7, 9, 5/*, 6*/ };

	for (int i : input) {
		
		// Add the new input to the tree
		mainTree.addNode(i);
		
		// Check if the tree's unbalanced
		if (!mainTree) {

			// Rebalance the tree

		}
	}
}