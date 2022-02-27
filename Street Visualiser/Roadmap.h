#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>

#define PI 3.14159265

using namespace std;
/* 

line style:
0 -> Line is solid
1 -> Line is dashed
2 -> Line is dubbled

*/

namespace Nodes 
{
	class Road_Node {
	public:
		int road_width = 10;

		int r_Road_lanes = 5;
		int l_Road_lanes = 2;
		float rotation = 45;

		vector<int> r_Road_conectors_x;
		vector<int> r_Road_conectors_y;
		vector<int> l_Road_conectors_x;
		vector<int> l_Road_conectors_y;

		int pos_x = 0;
		int pos_y = 0;

		void setup() {
			for (int r = 1; r < r_Road_lanes + 1; r++) {
				r_Road_conectors_x.push_back(pos_x - (double)sin(rotation * PI / 180) * road_width * r);
				r_Road_conectors_y.push_back(pos_y + (double)cos(rotation * PI / 180) * road_width * r);
			}

			for (int l = 1; l < l_Road_lanes + 1; l++) {
				l_Road_conectors_x.push_back(pos_x + (double)sin(rotation * PI / 180) * road_width * l);
				l_Road_conectors_y.push_back(pos_y - (double)cos(rotation * PI / 180) * road_width * l);
			}
		}

		void set_pos(int x, int y, float rot, int r_lanes = 2, int l_lanes = 2) {
			pos_x = x;
			pos_y = y;
			rotation = rot;
			r_Road_lanes = r_lanes;
			l_Road_lanes = l_lanes;
			this->setup();
		}

		void draw(sf::RenderWindow* win) {
			// --- draw Node --- //
			sf::CircleShape circle;
			circle.setRadius(5);
			circle.setPosition(pos_x - 5, pos_y - 5);
			circle.setFillColor(sf::Color::Black);

			win->draw(circle);

			// --- draw right side line conectors --- //
			for (int r = 0; r < r_Road_conectors_x.size(); r++) {
				sf::CircleShape circle;
				circle.setRadius(2);
				circle.setPosition(r_Road_conectors_x[r] - 2, r_Road_conectors_y[r] - 2);
				circle.setFillColor(sf::Color::Black);

				win->draw(circle);
			}

			// --- draw keft side line connectors
			for (int l = 0; l < l_Road_conectors_x.size(); l++) {
				sf::CircleShape circle;
				circle.setRadius(2);
				circle.setPosition(l_Road_conectors_x[l] - 2, l_Road_conectors_y[l] - 2);
				circle.setFillColor(sf::Color::Black);

				win->draw(circle);
			}
		}
	};

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

