#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <cmath>
#include "Body.hpp"

#ifndef M_PI
	#define M_PI 3.14
#endif

class Universe
{
public:
	void addPlanet(sf::Vector2f position, uint64_t m, sf::Vector2f dir);
	void addPlanet(Body p);
	void createProtodisk(const int number, const int radius, const int mass, const sf::Vector2f& position);

	void move(float delta_t);
	void draw(sf::RenderWindow& window);
	void eraseAt(const sf::Vector2f& pos);
	void eraseAll();

private:
	std::vector<Body> planets;

	// Protodisk stuff
	std::mt19937 rng;
	std::uniform_real_distribution<float> distribution;
};

Body combinedPlanets(const Body& a, const Body& b);

#endif // UNIVERSE_HPP