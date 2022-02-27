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
	Nodes::Node node1;
	node1.set_pos(20, 20);
	Nodes::Node node2;
	node2.set_pos(200, 20);
	Nodes::Node node3;
	node3.set_pos(100, 200);
	Nodes::Node node4;
	node4.set_pos(250, 200);
	bez.add_Nodes({node1, node2, node3, node4});
	bez.create_bezier(50);

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