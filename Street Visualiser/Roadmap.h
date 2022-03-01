#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>

#define PI 3.14159265
#define dash_length 10

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

		// calculate node Positions
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
			// set position, rotation and amount of lanes
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

			// --- draw keft side line connectors --- //
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
};

namespace Curve
{
	class Bezier {
	public:
		vector<Nodes::Node> Nodes = {};

		vector<int> curve_x = {};
		vector<int> curve_y = {};

		bool show_Bezier_Points = false;
		int line_style = 0;

		float get_length() {
			float len = 0;
			// loop through each segment
			for (int i = 0; i < curve_x.size()-1; i++) {
				len += sqrt((curve_x[i] - curve_x[i + 1]) * (curve_x[i] - curve_x[i + 1]) + (curve_y[i] - curve_y[i + 1]) * (curve_y[i] - curve_y[i + 1]));
			}
			return len;
		}

		void create_bezier(int substeps) {
			// calculate multidimensional Bezier
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

			// define length of road marking
			float length = this->get_length();
			int dash_amount = ceil(length / (float)dash_length);
			if (dash_amount % 2 == 1) {
				dash_amount += 1;
			}
			float dash_len = length / (float)dash_amount;

			// if line is dashed
			if (line_style == 1) {
				// new positions
				vector<int> new_curve_x = { curve_x[0] };
				vector<int> new_curve_y = { curve_y[0] };
				// algorythem
				int pos = 0;
				float last_x = curve_x[0];
				float last_y = curve_y[0];
				while (pos < curve_x.size() - 1) {
					float seg_len = sqrt((last_x - curve_x[pos + 1]) * (last_x - curve_x[pos + 1]) + (last_y - curve_y[pos + 1]) * (last_y - curve_y[pos + 1]));
					if (dash_len < seg_len) {
						float delta_x = curve_x[pos + 1] - last_x;
						float delta_y = curve_y[pos + 1] - last_y;
						last_x = delta_x / seg_len * dash_len + last_x;
						last_y = delta_y / seg_len * dash_len + last_y;
						new_curve_x.push_back(last_x);
						new_curve_y.push_back(last_y);
					}
					else {
						pos += 1;
					}
				}
				// add last point of bezier if needed
				if (new_curve_x.size() % 2 == 1) {
					new_curve_x.push_back(curve_x[curve_x.size() - 1]);
					new_curve_y.push_back(curve_y[curve_y.size() - 1]);
				}
				// put back values
				curve_x = new_curve_x;
				curve_y = new_curve_y;
			}
		}

		void add_Nodes(vector<Nodes::Node> node) {
			for (int i = 0; i < node.size(); i++) {
				Nodes.push_back(node[i]);
			}
		}

		void add_Node(Nodes::Node node) {
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
			// solid line
			vector<sf::Vertex> bezier_curve;
			for (int i = 0; i < curve_x.size(); i++) 
			{
				bezier_curve.push_back(sf::Vertex(sf::Vector2f((int)curve_x[i], (int)curve_y[i]), sf::Color::Black));
			}

			// solid line 
			if (line_style == 0) {
				win->draw(&bezier_curve[0], bezier_curve.size(), sf::LinesStrip);
			}
			// dashed line
			if (line_style == 1) {
				win->draw(&bezier_curve[0], bezier_curve.size(), sf::Lines);
			}
		}
	};
};

namespace Streets
{
	class Street {
	public:
		vector<Nodes::Road_Node> Nodes;

		vector<Curve::Bezier> Beziers;

		void add_Nodes(vector<Nodes::Road_Node> Road_Nodes) {
			for (int i = 0; i < Road_Nodes.size(); i++) {
				Nodes.push_back(Road_Nodes[i]);
			}
		}

		void add_Node(Nodes::Road_Node Road_Node) {
			Nodes.push_back(Road_Node);
		}

		void generate_street() {
			this->generate_bezier();
		}

		void generate_bezier() {
			// --- generate center lane road marking --- //
			Curve::Bezier center_Bez;
			for (int res = 0; res < Nodes.size(); res++) {
				Nodes::Node node;
				node.set_pos(Nodes[res].pos_x, Nodes[res].pos_y);

				center_Bez.add_Node(node);
			}
			center_Bez.create_bezier(100);
			Beziers.push_back(center_Bez);

			// --- generate right lane road markings --- //
			for (int r = 0; r < Nodes[0].r_Road_lanes; r++) {
				Curve::Bezier Bez;
				for (int res = 0; res < Nodes.size(); res++) {
					Nodes::Node node;
					node.set_pos(Nodes[res].r_Road_conectors_x[r], Nodes[res].r_Road_conectors_y[r]);

					Bez.add_Node(node);

					// turn lane into dashed lane
					if (r < Nodes[0].r_Road_lanes - 1) {
						Bez.line_style = 1;
					}
				}
				Bez.create_bezier(100);
				Beziers.push_back(Bez);
			}

			// --- generate left lane road markings --- //
			for (int l = 0; l < Nodes[0].l_Road_lanes; l++) {
				Curve::Bezier Bez;
				for (int res = 0; res < Nodes.size(); res++) {
					Nodes::Node node;
					node.set_pos(Nodes[res].l_Road_conectors_x[l], Nodes[res].l_Road_conectors_y[l]);

					Bez.add_Node(node);

					// turn lane into dashed lane
					if (l < Nodes[0].l_Road_lanes - 1) {
						Bez.line_style = 1;
					}
				}
				Bez.create_bezier(100);
				Beziers.push_back(Bez);
			}
		}

		void draw(sf::RenderWindow* window) {
			for (int i = 0; i < Beziers.size(); i++) {
				Beziers[i].draw(window);
			}
		}
	};
};

