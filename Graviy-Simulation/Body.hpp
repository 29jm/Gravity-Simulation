#ifndef BODY_HPP
#define BODY_HPP

#include <SFML/Graphics.hpp>

class Body
{
public:
	Body(int m, sf::Vector2f pos, float vel);

	void applyGravityOf(const Body& b, float dt);
	void update(const Body& b, float dt);
	float distanceTo(const Body& b);
	void draw(sf::RenderWindow& window);
	int getMass() const;
	sf::Vector2f getPosition() const ;

private:
	int mass;
	float velocity;
	float accel;
	sf::Vector2f position;

	sf::CircleShape shape;
};

#endif // BODY_HPP
