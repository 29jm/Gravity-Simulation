#ifndef BODY_HPP
#define BODY_HPP

#include <SFML/Graphics.hpp>

class Body
{
public:
	Body(sf::Vector2f pos, int m, sf::Vector2f(dir));

	void move(float dt);
	void applyGravityOf(const Body& b, float dt);
	void draw(sf::RenderWindow& window);
	bool collideWith(const Body& p);
	sf::Vector2f getPosition();
	sf::Vector2f getDirection();
	float getMass();

private:
	float getDistanceTo(const Body& p);

	sf::CircleShape shape;
	sf::Vector2f position;
	sf::Vector2f direction;
	float mass;
};

#endif // BODY_HPP
