#ifndef BODY_HPP
#define BODY_HPP

#include <SFML/Graphics.hpp>

class Body
{
public:
    Body(sf::Vector2i pos, float m, sf::Vector2f dir);

	void move(float dt);
	void applyGravityOf(const Body& b, float dt);
	void draw(sf::RenderWindow& window);
    bool collideWith(const Body& p);
    bool contains(const sf::Vector2f& point);

    sf::Vector2f position;
    sf::Vector2f direction;
    float mass;

private:
	float getDistanceTo(const Body& p);

	sf::CircleShape shape;
};

#endif // BODY_HPP
