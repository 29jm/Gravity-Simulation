#ifdef _WIN32
#define M_PI 3.14
#endif

#include <cmath>
#include <iostream>
#include <fstream>

#include "Body.hpp"

#define G 9.82 // TODO: Get the right number
#define DENSITY 20 // kg/m^3

using namespace sf;
using namespace std;

Body::Body(Vector2f pos, int m, Vector2f dir)
	: position(pos), direction(dir), mass(m)
{
	if (mass <= 0)
	{
		cout << "FATAL ERROR: MASS <= 0" << endl;
	}

	// Radius
	float volume = mass / DENSITY;
	radius = cbrt((3*volume)/(4*M_PI));
	
	if (radius <= 1)
	{
		radius = 1;
	}

	shape.setRadius(radius);
	shape.setFillColor(Color::Red);

	cout << "New planet with radius " << radius << " at position (" << pos.x << ';' << pos.y << ")" << endl;
}

void Body::move(float dt)
{
	position += direction * dt;
}

void Body::applyGravityOf(const Body &b, float dt)
{
    float r = getDistanceTo(b);

    if (r <= 0)
    {
    	cout << "FATAL ERROR: distance <= 0" << endl;
    	return;
    }

    float F = (G*mass*b.mass) / (r*r);

    // Make the force proportional to the mass
    F /= mass;

	// Get the unit vector to the other body
	Vector2f to_Body(b.position - position);
	to_Body = to_Body / r;

    // Apply the force in the direction of the other body
    direction += (to_Body * F) * dt;
}

float Body::getDistanceTo(const Body &p)
{
    Vector2f a = position;
	Vector2f b = p.position;
    Vector2f c = b - a;

    return sqrt(c.x*c.x + c.y * c.y);
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

bool Body::contains(const Vector2f &point)
{
    Vector2f a = position;
    Vector2f b = point;
    Vector2f c = b - a;

    if ((c.x*c.x + c.y * c.y) <= shape.getRadius()*shape.getRadius())
    {
        return true;
    }

    return false;
}
