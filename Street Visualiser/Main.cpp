#include "Roadmap.h"

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int main() 
{
	// create Window
	RenderWindow* window = new RenderWindow(VideoMode(1500, 1000), "This is the Title");

	// set limit Framerate
	window->setFramerateLimit(60);

	// create Street node
	Nodes::Road_Node Node1;
	Node1.set_pos(800, 200, 180, 3, 2);

	Nodes::Road_Node Node2;
	Node2.set_pos(200, 200, 0, 2, 3);

	vector<Nodes::Road_Node> road_nodes = { Node1, Node2 };
	Streets::Street Street1;
	Street1.add_Nodes(road_nodes);
	Street1.generate_street();

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

		Street1.draw(window);

		window->display();
	}

	// return no error
	return 0;
}