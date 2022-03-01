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
	Node1.set_pos(200, 200, 45, 2, 3);

	Nodes::Road_Node Node2;
	Node2.set_pos(400, 400, 45, 2, 3);

	Nodes::Road_Node Node3;
	Node3.set_pos(500, 600, 0, 2, 3);

	Nodes::Road_Node Node4;
	Node4.set_pos(900, 600, 0, 2, 3);

	Nodes::Road_Node Node5;
	Node5.set_pos(1100, 600, 0, 2, 3);

	Nodes::Road_Node Node6;
	Node6.set_pos(1200, 500, -45, 2, 3);

	Nodes::Road_Node Node7;
	Node7.set_pos(1300, 400, -45, 2, 3);

	vector<Nodes::Road_Node> road_nodes = { Node1, Node2, Node3, Node4 };
	Streets::Street Street1;
	Street1.add_Nodes(road_nodes);
	Street1.generate_street();

	vector<Nodes::Road_Node> road_nodes_2 = { Node4, Node5, Node6, Node7 };
	Streets::Street Street2;
	Street2.add_Nodes(road_nodes_2);
	Street2.generate_street();

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

		//Node1.draw(window);
		//Node2.draw(window);
		//Node3.draw(window);
		//Node4.draw(window);

		Street1.draw(window);
		Street2.draw(window);

		window->display();
	}

	// return no error
	return 0;
}