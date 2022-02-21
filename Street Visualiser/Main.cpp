#include "Roadmap.h"

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int main() 
{
	// create Window
	RenderWindow* window = new RenderWindow(VideoMode(500, 500), "This is the Title");

	// set limit Framerate
	window->setFramerateLimit(60);

	// set street class
	Nodes::Bezier bez;
	bez.set_Nodes(0, 0, 150, 50, 550, 300, 400, 450);
	bez.create_bezier(10);

	// Window_loop
	while (window->isOpen()) 
	{
		// loop through all events that acured las frame
		Event event;
		while (window->pollEvent(event)) 
		{
			// check if window event is == closed
			if (event.type == Event::Closed) 
			{
				window->close();
			}
		}

		// draw stuff
		window->clear(Color::White);

		bez.draw(window);

		window->display();
	}

	// return no error
	return 0;
}