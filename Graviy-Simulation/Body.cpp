#ifdef _WIN32
#define M_PI 3.14
#endif

#include <cmath>
#include <iostream>
#include <fstream>

#include "Body.hpp"

#define G 15 // TODO: Get the right number
#define SIZE 6
#define DENSITY 7878 // kg/m^3

using namespace sf;
using namespace std;

Body::Body(Vector2f pos, int m, Vector2f dir)
	: position(pos), direction(dir), mass(m)
{
	/*
	float volume = DENSITY / mass;
	float radius = cbrt(3*volume/4*M_PI) * 0.5;
	*/
	shape.setRadius(mass / 2);
	shape.setFillColor(Color::Red);
}

void Body::move(float dt)
{
	position += direction * dt;
}

void Body::applyGravityOf(const Body &b, float dt)
{
	// Get the force between bodies
	float F = fabs(G * ((mass*b.mass) / (getDistanceTo(b)*getDistanceTo(b)) ) );

	// Get the unit vector to the other body
	Vector2f to_Body(b.position - position);
	to_Body = to_Body / getDistanceTo(b); // unit vector = v / v.length

	// Apply the force in the direction of the other body
	position += (to_Body * F) * dt;
}

float Body::getDistanceTo(const Body &p)
{
	Vector2f a = this->position;
	Vector2f b = p.position;

	return sqrt((b.x-a.x)*(b.x-a.x) + (b.y*a.y)*(b.y*a.y));
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
