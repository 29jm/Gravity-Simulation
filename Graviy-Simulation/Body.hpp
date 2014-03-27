#ifndef BODY_HPP
#define BODY_HPP

#include <SFML/Graphics.hpp>

class Body
{
public:
    Body(sf::Vector2f pos, uint64_t m, sf::Vector2f dir);

	void move(float dt);
	void applyGravityOf(const Body& b, float dt);
	void draw(sf::RenderWindow& window);
    bool collideWith(const Body& p);
    bool contains(const sf::Vector2f& point);

    sf::Vector2f position;
    sf::Vector2f direction;
    uint64_t mass;
    float radius;

private:
	float getDistanceTo(const Body& p);

	sf::CircleShape shape;
};

#endif // BODY_HPP
