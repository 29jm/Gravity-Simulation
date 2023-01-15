#pragma once

#include "Universe.hpp"

#include <SFML/Graphics.hpp>

#define MASS 10
#define BASE_LINE 2
#define CLUSTER_RADIUS 100
#define CLUSTER_NUMBER 1000
#define CLUSTER_MASS 1000

class Interface {

public:
	Interface(sf::RenderWindow& win, Universe& verse);

	void handle_event(sf::Event evt);
	void draw();

private:
	struct {
		bool is_placing;
		bool is_moving;
		bool control;
		bool shift;
		bool rcontrol;
	} modifiers;

	Universe& universe;
	Body* followedPlanet;
	sf::RenderWindow& window;

	sf::View view;
	sf::CircleShape base_line;
	sf::VertexArray line;
	sf::Font font;
	sf::Text mass_text;
	sf::Text num_planets_text;
	sf::Text paths_text;

	uint64_t mass;
	uint64_t num_planets;
	float zoom;
	sf::Vector2f mousePosition;
	sf::Vector2f start;
	sf::Vector2f end;
};

