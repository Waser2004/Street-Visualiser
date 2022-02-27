#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;
/* 

line style:
0 -> Line is solid
1 -> Line is dashed
2 -> Line is dubbled

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
	vector<Node> Nodes = {};

	vector<int> curve_x = {};
	vector<int> curve_y = {};

	bool show_Bezier_Points = false;
	int line_style = 0;

	void create_bezier(int substeps) {
		for (int step = 0; step < substeps + 1; step++) {
			vector<float> pos_x;
			vector<float> pos_y;
			for (int node = 0; node < Nodes.size(); node++) {
				pos_x.push_back(Nodes[node].x);
				pos_y.push_back(Nodes[node].y);
			}
			while (pos_x.size() > 2) {
				vector<float> new_pos_x;
				vector<float> new_pos_y;
				for (int i = 0; i < pos_x.size() - 1; i++) {
					new_pos_x.push_back(pos_x[i] + ((pos_x[i + 1] - pos_x[i]) / (float)substeps * (float)step));
					new_pos_y.push_back(pos_y[i] + ((pos_y[i + 1] - pos_y[i]) / (float)substeps * (float)step));
				}
				pos_x = new_pos_x;
				pos_y = new_pos_y;
			}
			curve_x.push_back(pos_x[0] + ((pos_x[1] - pos_x[0]) / (float)substeps * (float)step));
			curve_y.push_back(pos_y[0] + ((pos_y[1] - pos_y[0]) / (float)substeps * (float)step));
		}
	}

	void add_Nodes(vector<Node> node) {
		for (int i = 0; i < node.size(); i++) {
			Nodes.push_back(node[i]);
		}
	}

	void add_Node(Node node) {
		Nodes.push_back(node);
	}

	void draw(sf::RenderWindow* win) {
		// --- draw points --- //
		if (show_Bezier_Points == true) {
			for (int i = 0; i < Nodes.size(); i++) {
				sf::CircleShape circle;
				circle.setRadius(5);
				circle.setPosition(Nodes[i].x - 5, Nodes[i].y - 5);
				circle.setFillColor(sf::Color::Black);

				win->draw(circle);
			}
		}

		// --- draw bezeier --- //
		vector<sf::Vertex> bezier_curve;
		for (int i = 0; i < curve_x.size(); i++) {
			bezier_curve.push_back(sf::Vertex(sf::Vector2f((int)curve_x[i], (int)curve_y[i]), sf::Color::Black));
		}

		win->draw(&bezier_curve[0], bezier_curve.size(), sf::LineStrip);
	}
};
};

