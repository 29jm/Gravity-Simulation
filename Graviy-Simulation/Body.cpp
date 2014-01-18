#include "Body.hpp"
#include <cmath>

Body::Body(int m, sf::Vector2f pos, float vel)
	: mass(m), velocity(vel), position(pos), shape(m)
{

}

int Body::getMass() const
{
	return mass;
}

sf::Vector2f Body::getPosition() const
{
	return position;
}

float Body::distanceTo(const Body &b)
{
	sf::Vector2f pos2 = b.getPosition();

	return sqrt((position.x - position.y)*(position.x - position.y) + (pos2.x - pos2.y)*(pos2.x - pos2.y));
}

void Body::applyGravityOf(const Body &b, float dt)
{
	int m2 = b.getMass();
	float d = distanceTo(b);

	float force = 9.82*(mass*m2/d*d);

	accel = force / mass;
	position.x += (dt * (velocity + dt*accel / 2)) / 2;
	position.y += (dt * (velocity + dt*accel / 2)) / 2;
}

void Body::update(const Body& b, float dt)
{
	int m2 = b.getMass();
	float d = distanceTo(b);

	float force = 9.82*(mass*m2/d*d);

	int new_accel = force / mass;
	velocity += dt * (accel+new_accel) / 2;
}

void Body::draw(sf::RenderWindow &window)
{
	shape.setPosition(position);
	window.draw(shape);
}
