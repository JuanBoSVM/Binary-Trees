#include <iostream>
#include <string>
#include <vector>
#include <algorithm>			// Min
#include <SFML/Graphics.hpp>

#include "BinaryTree.h"
#include "Circle.h"

/************************************************************************/
/*								OPERATORS	                            */
/************************************************************************/

// Draw both of a circle's members onto a window
void operator << (sf::RenderWindow&, Circle&);

/************************************************************************/
/* 								DECLARATIONS	                        */
/************************************************************************/

// Print the current state of the tree in the selected window
void print(sf::RenderWindow&, const BinaryTree<int>&);

// Get the nodes from the tree and print them
void getNodes(
	sf::RenderWindow& _window, std::vector<int>, Node<int>*, int, int = 0
);


/************************************************************************/
/* 								DEFINITIONS		                        */
/************************************************************************/

void print(sf::RenderWindow& _window, const BinaryTree<int>& _tree) {

	int
		screenX{ 1920 },
		screenY{ 1080 };

	// Create the window to display the tree
	_window.create(
		sf::VideoMode(screenX, screenY),
		"Binary Trees",
		sf::Style::Close
	);


	// Main Window loop
	while (_window.isOpen()) {
		
		// Event management
		sf::Event event;
		while (_window.pollEvent(event)) {
			
			// Close the window
			if (event.type == sf::Event::Closed) { _window.close();	}
		}

		_window.clear();

		// List of the number of nodes per level
		std::vector<int> nPerLevel;

		// Get the root from the tree
		Node<int>* cNode{ _tree.getRoot() };

		// Print the nodes from the tree
		getNodes(_window, nPerLevel, cNode, screenX >> 1);

		_window.display();
	}
}

void getNodes(
	sf::RenderWindow& _window, std::vector<int> _nLvl,
	Node<int>* _cNode, int xPos, int _lvl
) {

	// If the node's empty, return
	if (_cNode == nullptr) { return; }

	std::string number{ std::to_string(_cNode->value) };

	Circle cNode{ number };

	// Get the current level of the node
	cNode.level = _lvl + 1;

	// Add that level to the list
	_nLvl.push_back(cNode.level);

	// Get the correct y position of the node
	int yPos = 75;
	yPos *= cNode.level;

	// Move the node to the correct position
	cNode.move(xPos, yPos);

	// Print the node
	_window << cNode;

	// Offset in the x axis for the children
	int 
		offset{ 1920 },
		lPos,
		rPos;

	// Calculate the offset based on the closest border
	for (int i{ 0 }; i < cNode.level + 1; i++) {
		
		offset = offset >> 1;
	}	

	lPos = xPos - offset;
	rPos = xPos + offset;

	// Get the left side
	getNodes(_window, _nLvl, _cNode->lChild, lPos, cNode.level);

	// Get the right side
	getNodes(_window, _nLvl, _cNode->rChild, rPos, cNode.level );
}


/************************************************************************/
/*								OPERATORS	                            */
/************************************************************************/

void operator << (sf::RenderWindow& _window, Circle& _node) {
	
	// Draw both members
	_window.draw(_node.circle);
	_window.draw(_node.number);
}

/************************************************************************/
/* 									MAIN                                */
/************************************************************************/

int main() {
	BinaryTree<int> tree;
	std::vector<int> input;

	sf::RenderWindow window;

	std::cout << "Enter the numbers to use in the tree:" << std::endl;

	// Fill the input array
	for (int next{ 0 }; std::cin >> next; input.push_back(next)) {
		if (std::cin.peek() == '\n') { std::cin.clear( std::ios::eofbit); }
	}

	std::cin.clear();

	for (int i : input) {
		
		// Add the new input to the tree
		tree.addNode(i);
		
		
		// Update the tree's info
		tree.update();

		// Check if the tree's unbalanced
		if (!tree) {
			
			// Print the before version
			//print(window, tree);

			// Rebalance the tree
			tree.rebalance();
		}
		
		//Print the result
		//print(window, tree);
	}

	print(window, tree);
}