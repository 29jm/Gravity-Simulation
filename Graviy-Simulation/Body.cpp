#include <cmath>
#include <iostream>

#include "Body.hpp"

#define G 9.82
#define SIZE 6

using namespace sf;
using namespace std;

Body::Body(Vector2f pos, int m, Vector2f dir)
	: shape(7), position(pos), direction(dir), mass(m)
{
	shape.setFillColor(Color::Red);
	cout << "New body : pos(" << position.x << ';' << position.y << ")" << endl;
}

void Body::applyGravityOf(const Body &b, float dt)
{
	//cout << "distance : " << getDistanceTo(b) << endl;

	float F = G*(mass*b.mass / getDistanceTo(b)*getDistanceTo(b));
	direction.x += F * dt; // (F / mass) * dt
	direction.y += F * dt;
	position += direction;
}

float Body::getDistanceTo(const Body &b)
{
	Vector2f move = b.position - position;
	return sqrt(move.x*move.x + move.y*move.y);
}

void Body::draw(RenderWindow &window)
{
	shape.setPosition(position);
	window.draw(shape);
}

bool Body::operator!=(const Body& b)
{
	return (this->direction != b.direction) && (this->mass != b.mass) && (this->position != b.position);
}
