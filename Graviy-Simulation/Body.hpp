#ifndef BODY_HPP
#define BODY_HPP

#include <SFML/Graphics.hpp>

class Body
{
public:
	Body(sf::Vector2f pos, int m, sf::Vector2f(dir));

	void move(float dt);
	void applyGravityOf(const Body& b, float dt);
	float getDistanceTo(const Body& p);
	void draw(sf::RenderWindow& window);

	bool operator!=(const Body& b);

private:
	sf::CircleShape shape;
	sf::Vector2f position;
	sf::Vector2f direction;
	int mass;
};

#endif // BODY_HPP
