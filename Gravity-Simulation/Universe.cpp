#include "Universe.hpp"

using namespace sf;
using namespace std;

Universe::Universe()
	: show_path(false), old_paths(LinesStrip)
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
	cout << "New protodisk: radius " << radius << endl;
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
		float len = sqrt(pos.x*pos.x + pos.y*pos.y);
		pos += position;

		Vector2f dir = tangentThroughPoint(position, len, pos);
		if (isnan(dir.x) || isnan(dir.y))
		{
			dir = Vector2f();
		}

		dir *= 125.0f;

		cout << "dir=" << dir.x << ';' << dir.y << endl;

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
					Body p = combinedPlanets(planets[i], planets[j]);
					p.setPathEnabled(show_path);

					planets.push_back(p);

					savePlanetPath(planets[i]);
					savePlanetPath(planets[j]);

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

Vector2f tangentThroughPoint(Vector2f circle_center, float radius, Vector2f point)
{
	Vector2f dir = point - circle_center;
	float len = sqrt(dir.x*dir.x + dir.y*dir.y);
	float angle = atan2(dir.y, dir.x);

	float tangent_length = sqrt(len*len - radius*radius);
	float tangent_angle = asin(radius/len);

	//Vector2f pos(angle+tangent_angle);
	float neg(angle-tangent_angle);

	return Vector2f(cos(neg), sin(neg));
}
