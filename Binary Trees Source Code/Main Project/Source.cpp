#include <iostream>
#include <vector>

#include "Tree.h"


/************************************************************************/
/* 									MAIN                                */
/************************************************************************/

int main() {
	Tree<int> mainTree;
	std::vector<int> input{ 10, 20, 15, 25, 30, 16, 18, 19 };

	for (int i : input) {
		
		// Add the new input to the tree
		mainTree.addNode(i);
		
		// Check if the tree's unbalanced
		if (!mainTree) {
			
			// Print the before version
			

			// Rebalance the tree
			mainTree.rebalance();

			// Update the tree's info
			mainTree.update();
		}

		//Print the result
	}
}