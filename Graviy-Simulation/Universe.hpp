#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Body.hpp"

class Universe
{
public:
	void addPlanet(sf::Vector2f position, uint64_t m, sf::Vector2f dir);
	void addPlanet(Body p);

	void move(float delta_t);
	void draw(sf::RenderWindow& window);
	void eraseAt(const sf::Vector2f& pos);

private:
	std::vector<Body> planets;
};

#endif