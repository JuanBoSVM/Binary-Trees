#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Circle {

public:

	sf::CircleShape circle{ 25.f };
	sf::Font		font;
	sf::Text		number;
	int
		level{ 0 },	// Level of the node
		pos  { 0 };	// Position in the level


/************************************************************************/
/*								METHODS                                 */
/************************************************************************/

public:

	// Move the whole circle
	void move(float, float);


/************************************************************************/
/* 						CONSTRUCTORS AND DESTRUCTOR				        */
/************************************************************************/

	Circle(std::string);
	~Circle();
};