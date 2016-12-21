#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <functional>
#include <cmath>
#include "Body.hpp"

#ifndef M_PI
	#define M_PI 3.14
#endif

class Universe
{
public:
	Universe();

	void addPlanet(sf::Vector2f position, uint64_t m, sf::Vector2f dir);
	void addPlanet(Body p);
	void createProtodisk(const int number, const int radius, const int mass, const sf::Vector2f& position);
	void togglePath();

	uint64_t getPlanetNumber() const;
	bool isPathEnabled() const;

	void move(float delta_t);
	void draw(sf::RenderWindow& window);
	void eraseAt(const sf::Vector2f& pos);
	void eraseAll();

private:
	void savePlanetPath(const Body& b);

	std::vector<Body> planets;
	std::vector<sf::VertexArray> old_paths;
	bool show_path;

	// Protodisk stuff
	std::mt19937 rng;
};

Body combinedPlanets(const Body& a, const Body& b);
sf::Vector2f tangentThroughPoint(sf::Vector2f circle_center, sf::Vector2f point);

#endif // UNIVERSE_HPP
