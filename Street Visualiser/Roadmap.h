#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

using namespace std;
/* 
road types:
0 -> this is a one way road
1 -> there is oncoming traffic

parkinglot types:
0 -> there are no parking lots on the side of the road
1 -> there are parkinglots to park parallel
2 -> there are parkinglots to park vertical
3 -> there are parkinglots to park with an angle

line markings:
0 -> there is no line marking
1 -> there is a dashed line marking
2 -> there is a solid line marking
3 -> there is a double solid line marking

pavement:
0 -> there is no pavement
1 -> there is a pavement

bike lane position:
0 -> there is no bike lane
1 -> the bike lane is on the pavement
2 -> the bike lane is on the street

bike lane:
0 -> there is no bike line
1 -> there is a bike lane indicated with dashed lines & symbols
2 -> there is a bike lane indicated with dashed lines
3 -> there is a bike lane indicated with symbols

*/

namespace Nodes 
{
class Node {
public:
	float x;
	float y;

	void set_pos(int pos_x, int pos_y) {
		x = pos_x;
		y = pos_y;
	}
};

class Bezier {
public:
	Node p1;
	Node p2;
	Node p3;
	Node p4;

	float curve[11][2];

	void create_bezier(int substeps) {
		for (int i = 0; i < substeps + 1; i++) {
			float pos_1_x = p1.x + (p2.x - p1.x) / (float)substeps * i;
			float pos_1_y = p1.y + (p2.y - p1.y) / (float)substeps * i;
			float pos_2_x = p2.x + (p3.x - p2.x) / (float)substeps * i;
			float pos_2_y = p2.y + (p3.y - p2.y) / (float)substeps * i;
			float pos_3_x = p3.x + (p4.x - p3.x) / (float)substeps * i;
			float pos_3_y = p3.y + (p4.y - p3.y) / (float)substeps * i;
			float pos_4_x = pos_1_x + (pos_2_x - pos_1_x) / (float)substeps * i;
			float pos_4_y = pos_1_y + (pos_2_y - pos_1_y) / (float)substeps * i;
			float pos_5_x = pos_2_x + (pos_3_x - pos_2_x) / (float)substeps * i;
			float pos_5_y = pos_2_y + (pos_3_y - pos_2_y) / (float)substeps * i;
			curve[i][0] = pos_4_x + (pos_5_x - pos_4_x) / (float)substeps * i;
			curve[i][1] = pos_4_y + (pos_5_y - pos_4_y) / (float)substeps * i;
		}
	}

	void set_Nodes(int start_x, int start_y, int start_conector_x, int start_conector_y, int end_conector_x, int end_conector_y, int end_x, int end_y) {
		p1.set_pos(start_x, start_y);
		p2.set_pos(start_conector_x, start_conector_y);
		p3.set_pos(end_conector_x, end_conector_y);
		p4.set_pos(end_x, end_y);
	}

	void draw(sf::RenderWindow* win) {
		// --- draw points --- //
		// start
		sf::CircleShape circle1;
		circle1.setRadius(5);
		circle1.setPosition(p1.x - 5, p1.y - 5);
		circle1.setFillColor(sf::Color::Black);

		// start conector
		sf::CircleShape circle2;
		circle2.setRadius(5);
		circle2.setPosition(p2.x - 5, p2.y - 5);
		circle2.setFillColor(sf::Color::Blue);

		// end conector
		sf::CircleShape circle3;
		circle3.setRadius(5);
		circle3.setPosition(p3.x - 5, p3.y - 5);
		circle3.setFillColor(sf::Color::Blue);

		// end
		sf::CircleShape circle4;
		circle4.setRadius(5);
		circle4.setPosition(p4.x - 5, p4.y - 5);
		circle4.setFillColor(sf::Color::Black);

		win->draw(circle1);
		win->draw(circle2);
		win->draw(circle3);
		win->draw(circle4);

		// --- draw lines --- //
		// start conector
		sf::Vertex start_line[] = {
			sf::Vertex(sf::Vector2f(p1.x, p1.y), sf::Color::Black),
			sf::Vertex(sf::Vector2f(p2.x, p2.y), sf::Color::Black)
		};

		// end conector
		sf::Vertex end_line[] = {
			sf::Vertex(sf::Vector2f(p3.x, p3.y), sf::Color::Black),
			sf::Vertex(sf::Vector2f(p4.x, p4.y), sf::Color::Black)
		};

		win->draw(start_line, 2, sf::Lines);
		win->draw(end_line, 2, sf::Lines);


		// --- draw bezeier --- //
		sf::Vertex bezier_curve[11];
		for (int i = 0; i < 11; i++) {
			bezier_curve[i] = sf::Vertex(sf::Vector2f((int)curve[i][0], (int)curve[i][1]), sf::Color::Black);
		}

		win->draw(bezier_curve, 11, sf::LinesStrip);
	}
};
};

