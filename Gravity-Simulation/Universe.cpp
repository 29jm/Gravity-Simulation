#include "Universe.hpp"

using namespace sf;
using namespace std;

void Universe::addPlanet(Vector2f position, uint64_t m, Vector2f dir)
{
	planets.push_back(Body(position, m, dir));
}

void Universe::addPlanet(Body p)
{
	planets.push_back(p);
}

void Universe::createProtodisk(const int number, const int radius, const int mass, const sf::Vector2f& position)
{
	for (int i = 0; i < number; i++)
	{
		float a = distribution(rng);
		float b = distribution(rng);

		if (b > a)
		{
			float temp(a);
			a = b;
			b = temp;
		}

		Vector2f pos(b*radius*cos(2*M_PI*a/b), b*radius*sin(2*M_PI*a/b));
		pos += position;

		planets.push_back( Body(pos, mass, Vector2f()) );
	}
}

void Universe::move(float delta_t)
{
	if (planets.size() < 1)
	{
		return;
	}

	for (unsigned int i = 0; i < planets.size(); i++)
	{
		for (unsigned int j = 0; j < planets.size(); j++)
		{
			if (i != j)
			{
				if (planets[i].collideWith(planets[j]))
				{
					planets.push_back(combinedPlanets(planets[i], planets[j]));

					planets.erase(planets.begin()+j);
					planets.erase(planets.begin()+i);
				}

				planets[i].applyGravityOf(planets[j], delta_t);
			}
		}
	}

	for (Body& b : planets)
	{
		b.move(delta_t);
	}
}

void Universe::draw(RenderWindow& window)
{
	for (Body& b : planets)
	{
		b.draw(window);
	}
}

void Universe::eraseAt(const Vector2f& pos)
{
	for (unsigned int i = 0; i < planets.size(); i++)
	{
		if (planets[i].contains(Vector2f(pos.x, pos.y)))
		{
			planets.erase(planets.begin()+i);
		}
	}
}

void Universe::eraseAll()
{
	planets.clear();
}

Body combinedPlanets(const Body& a, const Body& b)
{
	Vector2f p1m1 = float(a.mass)*a.direction;
	Vector2f p2m2 = float(b.mass)*b.direction;
	
	uint64_t total_mass(a.mass+b.mass);
	Vector2f direction = (p1m1+p2m2) / float(total_mass);

	Vector2f pos = (a.mass >= b.mass ? a.position : 
									   b.position);

	return Body(pos, total_mass, direction);
}