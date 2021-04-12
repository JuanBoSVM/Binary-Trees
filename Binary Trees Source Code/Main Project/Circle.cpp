#include "Circle.h"

/************************************************************************/
/* 								DEFINITIONS		                        */
/************************************************************************/

void Circle::move(float x, float y) {
	
	circle.move(x, y);
	number.move(x, y);
}

/************************************************************************/
/* 						CONSTRUCTORS AND DESTRUCTOR				        */
/************************************************************************/

Circle::Circle(std::string _number) {

	// Load the font for the text
	font.loadFromFile("Fonts/JosefinSans.ttf");

	// Set the fill color for the circle
	circle.setFillColor(sf::Color::Cyan);

	// Set the attributes for the text
	number.setFont(font);
	number.setString(_number);
	number.setCharacterSize(25);
	number.setFillColor(sf::Color::Black);
	number.setOrigin( circle.getOrigin() );
	number.move(10.f, 5.f);
}

Circle::~Circle() {

}
