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

	// create Street node
	Nodes::Road_Node Node1;
	Node1.set_pos(100, 100, 45, 2, 2);

	Nodes::Road_Node Node2;
	Node2.set_pos(250, 300, 0, 2, 2);

	Nodes::Road_Node Node3;
	Node3.set_pos(450, 300, 0, 2, 2);

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

		Node1.draw(window);
		Node2.draw(window);
		Node3.draw(window);

		window->display();
	}

	// return no error
	return 0;
}