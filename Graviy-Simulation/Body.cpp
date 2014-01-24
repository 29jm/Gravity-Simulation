#ifdef _WIN32
#define M_PI 3.14
#endif

#include <cmath>
#include <iostream>
#include <fstream>

#include "Body.hpp"

#define G 0.0000667 // TODO: Get the right number
#define DENSITY 10 // kg/m^3

using namespace sf;
using namespace std;

Body::Body(Vector2f pos, int m, Vector2f dir)
	: position(pos), direction(dir), mass(m)
{
	// Radius
	float volume = mass / DENSITY;
	float radius = cbrt((3*volume)/(4*M_PI));
	radius = (radius <= 1 ? 1 : radius);

	// Color
	Color c;
	if (mass < 1000)
	{
		c.r = 102;
		c.g =  51;
		c.b = 0;
	}

	if (mass > 1000 && mass < 10000)
	{
		c.r = 255;
		c.g = 153;
		c.b = 51;
	}
	else
	{
		c.r = 255;
	}

	shape.setRadius(radius);
	shape.setFillColor(c);

	cout << "New planet with radius " << radius << endl;
}

void Body::move(float dt)
{
	position += direction * dt;
}

void Body::applyGravityOf(const Body &b, float dt)
{
	float F = fabs(G * ( mass*b.mass / getDistanceTo(b)*getDistanceTo(b) ) );

	// Get the unit vector to the other body
	Vector2f to_Body(b.position - position);
	to_Body = to_Body / getDistanceTo(b);

	// Apply the force in the direction of the other body
	direction += (to_Body * F) * dt;
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

bool Body::collideWith(const Body &p)
{
	Vector2f a = position;
	Vector2f b = p.position;
	float d = (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);

	float r1 = shape.getRadius();
	float r2 = p.shape.getRadius();
	if (d > (r1+r2)*(r1+r2))
		return false;
	return true;
}

Vector2f Body::getPosition()
{
	return position;
}

float Body::getMass()
{
	return mass;
}

Vector2f Body::getDirection()
{
	return direction;
}
