#ifdef _WIN32
#define M_PI 3.14
#endif

#include <cmath>
#include <iostream>
#include <fstream>

#include "Body.hpp"

#define G 20 // TODO: Get the right number
#define DENSITY 100 // kg/m^3

using namespace sf;
using namespace std;

Body::Body(Vector2f pos, uint64_t m, Vector2f dir, bool with_path)
	: position(pos), direction(dir), mass(m), path(LinesStrip), show_path(with_path)
{
	if (mass <= 0)
	{
		cout << "FATAL ERROR: MASS <= 0" << endl;
	}

	cout << "MASS=" << mass << endl;

	// Radius
	float volume = mass / DENSITY;
	radius = cbrt((3*volume)/(4*M_PI));
	
	if (radius < 1)
	{
		radius = 1;
	}

	Color color(Color::White);
	Color clear_yellow(212, 193, 106);
	Color brown(175, 75, 0);

	if (mass <= 10000)
	{
		color = interpolate(Color::White, clear_yellow, mass / 10000.0f);
	}
	else if (mass <= 100000)
	{
		color = interpolate(clear_yellow, brown, mass / 100000.0f);
	}
	else if (mass >= 1000000)
	{
		color = Color::Red;
	}
	else
	{
		color = interpolate(brown, Color::Red, mass / 1000000.0f);
	}

	shape.setRadius(radius);
	shape.setFillColor(color);
}

void Body::move(float dt)
{
	position += direction * dt;
		
	if (show_path)
	{
		path.append(Vertex(getCenter(), shape.getFillColor()));
	}
}

void Body::applyGravityOf(const Body &b, float dt)
{
	float r = getDistanceTo(b);

	if (r <= 0)
	{
		cout << "return" << endl;
		return;
	}

	float F = (G*mass*b.mass) / (r*r);

	// Make the force proportional to the mass
	F /= mass;

	// Get the unit vector to the other body
	Vector2f to_Body(b.getCenter() - getCenter());
	to_Body = to_Body / r;

	// Apply the force in the direction of the other body
	direction += (to_Body * F) * dt;
}

float Body::getDistanceTo(const Body &b) const
{
	Vector2f c = b.getCenter() - getCenter();

	return sqrt(c.x*c.x + c.y * c.y);
}

Vector2f Body::getCenter() const
{
	return Vector2f(position.x+radius, position.y+radius);
}

void Body::draw(RenderWindow &window)
{
	shape.setPosition(position);
	window.draw(shape);
	
	if (show_path)
	{
		window.draw(path);
	}
}

bool Body::collideWith(const Body &b) const
{
	Vector2f a = getCenter();
	Vector2f c = b.getCenter();

	float d = (a.x-c.x)*(a.x-c.x) + (a.y-c.y)*(a.y-c.y);

	return (d <= (radius+b.radius)*(radius+b.radius));
}

bool Body::contains(const Vector2f &point) const
{
	Vector2f center(getCenter());
	return (point.x-center.x)*(point.x-center.x)
		 + (point.y-center.y)*(point.y-center.y) <= radius*radius;
}

void Body::setPathEnabled(bool state)
{
	show_path = state;

	if (!show_path)
	{
		path.clear();
	}
}

VertexArray Body::getPath() const
{
	return path;
}

// Handy helper fonctions

/*
 *	ratio is a number between 0.0f and 1.0f
 */
Color interpolate(Color a, Color b, float ratio)
{
	Color c;
	c.r = (b.r-a.r) * ratio + a.r;
	c.g = (b.g-a.g) * ratio + a.g;
	c.b = (b.b-a.b) * ratio + a.b;

	return c;
}
