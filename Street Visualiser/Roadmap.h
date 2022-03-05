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
		int road_width = 20;

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

		void draw(sf::RenderWindow* win) {
			sf::CircleShape circle;
			circle.setRadius(4);
			circle.setPosition((int)x - 2, (int)y - 2);
			circle.setFillColor(sf::Color::Red);

			win->draw(circle);
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
		vector<Nodes::Node> supporting_Nodes;

		Curve::Bezier Bezier;
		vector<vector<sf::Vertex>> road_lanes;

		void add_Nodes(vector<Nodes::Road_Node> Road_Nodes) {
			for (int i = 0; i < Road_Nodes.size(); i++) {
				if (Nodes.size() == 2) {
					// remove first element
					Nodes.erase(Nodes.begin());
					supporting_Nodes.erase(supporting_Nodes.begin());
				}
				// add starting and ending point
				Nodes.push_back(Road_Nodes[i]);
				// add supporting nodes
				Nodes::Node node;
				node.set_pos(Road_Nodes[i].pos_x + 200 * sin((-Road_Nodes[i].rotation + 90) / 180 * PI), Road_Nodes[i].pos_y + 200 * cos((-Road_Nodes[i].rotation + 90) / 180 * PI));
				supporting_Nodes.push_back(node);
			}
		}

		void generate_street() {
			this->generate_bezier();
		}

		void generate_bezier() {
			// create center road lane
			// starting node
			Nodes::Node node_s;
			node_s.set_pos(Nodes[0].pos_x, Nodes[0].pos_y);
			Bezier.add_Node(node_s);

			// nodes in middle
			for (int res = 0; res < supporting_Nodes.size(); res++) {
				Bezier.add_Node(supporting_Nodes[res]);
			}

			// ending node
			Nodes::Node node_e;
			node_e.set_pos(Nodes[1].pos_x, Nodes[1].pos_y);
			Bezier.add_Node(node_e);

			// generate bezier with resolution of 20
			Bezier.create_bezier(20);

			// add Vertex ovectors to vector
			for (int rl_lanes = 0; rl_lanes < Nodes[0].r_Road_lanes + Nodes[0].l_Road_lanes; rl_lanes++) {
				vector<sf::Vertex> r_curve;
				road_lanes.push_back(r_curve);
			}

			// loop thorough each position
			for (int pos = 0; pos < Bezier.curve_x.size()-1; pos++) {
				// get rotation of bezier section
				float delta_x = Bezier.curve_x[pos + 1] - Bezier.curve_x[pos];
				float delta_y = Bezier.curve_y[pos + 1] - Bezier.curve_y[pos];
				float hyp = sqrt(delta_x * delta_x + delta_y * delta_y);
				float rotation;
				if (delta_y >= 0) {
					rotation = asin(delta_x / hyp) * 180 / PI + 90;
				}
				else {
					rotation = acos(delta_x / hyp) * 180 / PI;
				}
				// loop thorugh all lanes
				for (int lanes = 0; lanes < max(Nodes[0].r_Road_lanes, Nodes[0].l_Road_lanes); lanes++) {
					// add starting postion
					if (pos == 0) {
						// left side
						if (lanes < Nodes[0].l_Road_lanes) {
							float rotation = -Nodes[0].rotation;
							int new_x = Bezier.curve_x[0] - sin(rotation / 180 * PI) * (lanes + 1) * 20;
							int new_y = Bezier.curve_y[0] - cos(rotation / 180 * PI) * (lanes + 1) * 20;
							road_lanes[lanes].push_back(sf::Vertex(sf::Vector2f(new_x, new_y), sf::Color::Black));
						}
						// right side
						if (lanes < Nodes[0].r_Road_lanes) {
							float rotation = -Nodes[0].rotation;
							int new_x = Bezier.curve_x[0] + sin(rotation / 180 * PI) * (lanes + 1) * 20;
							int new_y = Bezier.curve_y[0] + cos(rotation / 180 * PI) * (lanes + 1) * 20;
							road_lanes[lanes + Nodes[0].l_Road_lanes].push_back(sf::Vertex(sf::Vector2f(new_x, new_y), sf::Color::Black));
						}
					}
					// check if rotation is negative
					if (rotation <= 0) {
						// creat curve for left lanes
						if (lanes < Nodes[0].l_Road_lanes) {
							int new_x = Bezier.curve_x[pos] + sin(rotation / 180 * PI) * (lanes + 1) * 20;
							int new_y = Bezier.curve_y[pos] + cos(rotation / 180 * PI) * (lanes + 1) * 20;
							road_lanes[lanes].push_back(sf::Vertex(sf::Vector2f(new_x, new_y), sf::Color::Black));
						}
						// craete curve for right lanes
						if (lanes < Nodes[0].r_Road_lanes) {
							int new_x = Bezier.curve_x[pos] - sin(rotation / 180 * PI) * (lanes + 1) * 20;
							int new_y = Bezier.curve_y[pos] - cos(rotation / 180 * PI) * (lanes + 1) * 20;
							road_lanes[lanes + Nodes[0].l_Road_lanes].push_back(sf::Vertex(sf::Vector2f(new_x, new_y), sf::Color::Black));
						}
					}
					// check if rotation is positive
					else {
						// creat curve for left lanes
						if (lanes < Nodes[0].l_Road_lanes) {
							int new_x = Bezier.curve_x[pos] - sin(rotation / 180 * PI) * (lanes + 1) * 20;
							int new_y = Bezier.curve_y[pos] - cos(rotation / 180 * PI) * (lanes + 1) * 20;
							road_lanes[lanes].push_back(sf::Vertex(sf::Vector2f(new_x, new_y), sf::Color::Black));
						}
						// craete curve for right lanes
						if (lanes < Nodes[0].r_Road_lanes) {
							int new_x = Bezier.curve_x[pos] + sin(rotation / 180 * PI) * (lanes + 1) * 20;
							int new_y = Bezier.curve_y[pos] + cos(rotation / 180 * PI) * (lanes + 1) * 20;
							road_lanes[lanes + Nodes[0].l_Road_lanes].push_back(sf::Vertex(sf::Vector2f(new_x, new_y), sf::Color::Black));
						}
					}
					// add last postion
					if (pos == Bezier.curve_x.size() - 2) {
						// left side
						if (lanes < Nodes[0].l_Road_lanes) {
							float rotation = -Nodes[Nodes.size() - 1].rotation;
							int new_x = Bezier.curve_x[Bezier.curve_x.size() - 1] + sin(rotation / 180 * PI) * (lanes + 1) * 20;
							int new_y = Bezier.curve_y[Bezier.curve_x.size() - 1] + cos(rotation / 180 * PI) * (lanes + 1) * 20;
							road_lanes[lanes].push_back(sf::Vertex(sf::Vector2f(new_x, new_y), sf::Color::Black));
						}
						// right side
						if (lanes < Nodes[0].r_Road_lanes) {
							float rotation = -Nodes[Nodes.size() - 1].rotation;
							int new_x = Bezier.curve_x[Bezier.curve_x.size() - 1] - sin(rotation / 180 * PI) * (lanes + 1) * 20;
							int new_y = Bezier.curve_y[Bezier.curve_x.size() - 1] - cos(rotation / 180 * PI) * (lanes + 1) * 20;
							road_lanes[lanes + Nodes[0].l_Road_lanes].push_back(sf::Vertex(sf::Vector2f(new_x, new_y), sf::Color::Black));
						}
					}
				}
			}
		}

		void draw(sf::RenderWindow* window) {
			Bezier.draw(window);

			for (int i = 0; i < road_lanes.size(); i++) {
				window->draw(&road_lanes[i][0], road_lanes[i].size(), sf::LinesStrip);
			}

			for (int i = 0; i < supporting_Nodes.size(); i++) {
				supporting_Nodes[i].draw(window);
			}
		}
	};
};

