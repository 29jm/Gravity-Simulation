#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include <SFML/Graphics.hpp>
#include "Body.hpp"

class Universe
{
public:
	void addPlanet(sf::Vector2i position, float m, sf::Vector2f dir);
	void move(float delta_t);
	void draw(sf::RenderWindow& window);
	void eraseAt(const sf::Vector2f& pos);

private:

	std::vector<Body> planets;
};

#endif