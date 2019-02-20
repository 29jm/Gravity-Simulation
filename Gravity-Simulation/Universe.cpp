#include "Universe.hpp"

using namespace sf;
using namespace std;

Universe::Universe()
	: old_paths(LinesStrip), show_path(false)
{

}

void Universe::addPlanet(Vector2f position, uint64_t m, Vector2f dir)
{
	planets.push_back(Body(position, m, dir, show_path));
}

void Universe::addPlanet(Body p)
{
	p.setPathEnabled(show_path);
	planets.push_back(p);
}

void Universe::createProtodisk(const int number, const int radius, const int mass, const sf::Vector2f& position)
{
	std::uniform_real_distribution<float> distribution(0.0f, float(radius));
	auto random = std::bind(distribution, rng);

	for (int i = 0; i < number; i++)
	{
		float t = 2*M_PI*random();
		float u = random()+random();
		float r = (u > 1 ? 2-u : u);
		Vector2f pos(r*cos(t), r*sin(t));

		float len = sqrt(pos.x*pos.x + pos.y*pos.y);
		pos += position;

		Vector2f dir = tangentThroughPoint(position, pos);

		dir *= interpolate(-5.0f, 60.0f, len/radius);

		Body p(pos, mass, dir, show_path);
		planets.push_back(p);
	}
}

void Universe::togglePath()
{
	show_path = !show_path;

	for (Body& b : planets)
	{
		b.setPathEnabled(show_path);
	}

	if (!show_path)
	{
		old_paths.clear();
	}
}

uint64_t Universe::getPlanetNumber() const
{
	return planets.size();
}

bool Universe::isPathEnabled() const
{
	return show_path;
}

void Universe::move(float delta_t)
{
	for (unsigned int i = 0; i < planets.size(); i++)
	{
		for (unsigned int j = 0; j < planets.size(); j++)
		{
			if (i == j)
				continue;

			if (planets[i].collideWith(planets[j]))
			{
				Body p = combinedPlanets(planets[i], planets[j]);
				p.setPathEnabled(show_path);

				savePlanetPath(planets[i]);
				savePlanetPath(planets[j]);

				int a = max(i, j), b = min(i, j);
				planets.erase(planets.begin()+a);
				planets.erase(planets.begin()+b);

				planets.push_back(p);
			}
			else
			{
				planets[i].applyGravityOf(planets[j], delta_t);
			}

			if (i >= planets.size())
				break;
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

	if (show_path)
	{
		for (VertexArray& path : old_paths)
		{
			window.draw(path);
		}
	}
}

void Universe::eraseAt(const Vector2f& pos)
{
	for (unsigned int i = 0; i < planets.size(); i++)
	{
		if (planets[i].contains(Vector2f(pos.x, pos.y)))
		{
			planets.erase(planets.begin()+i);
			cout << "one planet erased" << endl;
		}
	}
}

void Universe::eraseAll()
{
	planets.clear();
	old_paths.clear();
}

void Universe::savePlanetPath(const Body& b)
{
	old_paths.push_back(b.getPath());
}

// helper functions
Body combinedPlanets(const Body& a, const Body& b)
{
	Vector2f p1m1 = float(a.mass)*a.direction;
	Vector2f p2m2 = float(b.mass)*b.direction;
	
	uint64_t total_mass(a.mass+b.mass);
	Vector2f direction = (p1m1+p2m2) / float(total_mass);

	Vector2f pos = (a.mass >= b.mass ? a.position : 
									   b.position);

	return Body(pos, total_mass, direction, false);
}

Vector2f tangentThroughPoint(Vector2f circle_center, Vector2f point)
{
	Vector2f r(point - circle_center);
	Vector2f tangent(r.y, -r.x);
	tangent /= sqrt(r.x*r.x + r.y*r.y); // unit vector = vec/vec.length

	return tangent;
}
