#include <iostream>
#include <string>
#include <vector>
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

// Get the nodes from the tree and print them
void getNodes(
	sf::RenderWindow& _window, std::vector<int>, Node<int>*, int, int = 0
);


/************************************************************************/
/* 								DEFINITIONS		                        */
/************************************************************************/
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

	// Set the color depending on the level of the node
	switch (cNode.level % 6) {
	
	case 1:
		cNode.circle.setFillColor(sf::Color::Cyan);
		break;

	case 2:
		cNode.circle.setFillColor(sf::Color::Yellow);
		break;

	case 3:
		cNode.circle.setFillColor(sf::Color::Green);
		break;

	case 4:
		cNode.circle.setFillColor(sf::Color::Magenta);
		break;

	case 5:
		cNode.circle.setFillColor(sf::Color::Red);
	}

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

	int
		screenX{ 1920 },
		screenY{ 1080 };

	// Create the window to display the tree
	window.create(
		sf::VideoMode(screenX, screenY),
		"Binary Trees",
		sf::Style::Close
	);

	// Index for the input array
	int index{ 0 };

	// Main Window loop
	while (window.isOpen()) {


		// Event management
		sf::Event event;
		while (window.pollEvent(event)) {

			// Next step of the tree
			if (event.type == sf::Event::MouseButtonPressed) { 

				// Rebalance the tree if it's unbalanced
				if (!tree) { tree.rebalance(); }

				// Otherwise, add the new input to the tree
				else if (index < input.size()) { tree.addNode(input[index++]); }

				// Update the tree's info
				tree.update();
			}

			// Close the window
			else if (event.type == sf::Event::Closed) { window.close(); }
		}
		
		window.clear();

		// List of the number of nodes per level
		std::vector<int> nPerLevel;

		// Get the root from the tree
		Node<int>* cNode{ tree.getRoot() };

		// Print the nodes from the tree
		getNodes(window, nPerLevel, cNode, screenX >> 1);

		window.display();
	}
}