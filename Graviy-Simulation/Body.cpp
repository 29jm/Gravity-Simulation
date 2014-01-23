#include <cmath>
#include <iostream>
#include <fstream>

#include "Body.hpp"

#define G 9.82
#define SIZE 6

using namespace sf;
using namespace std;

Body::Body(Vector2f pos, int m, Vector2f dir)
	: shape(7), position(pos), direction(dir), mass(m)
{
	ofstream log_file("log.txt", ios::app | ios::out);
	shape.setFillColor(Color::Red);
}

void Body::move(float dt)
{
	position += direction * dt;
}

void Body::applyGravityOf(const Body &b, float dt)
{
	float F = fabs(G*(mass*b.mass / getDistanceTo(b)*getDistanceTo(b)));
	direction.x += (F / mass) * dt; // (F / mass) * dt
	direction.y += (F / mass) * dt;
	position += direction;
}

float Body::getDistanceTo(const Body &p)
{
	Vector2f a = this->position;
	Vector2f b = p.position;

	return sqrt((b.x-a.x)*(b.x-a.x)+(b.y*a.y)*(b.y*a.y));
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
