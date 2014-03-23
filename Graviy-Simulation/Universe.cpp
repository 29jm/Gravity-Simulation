#include "Universe.hpp"

using namespace sf;
using namespace std;

void Universe::addPlanet(Vector2f position, float m, Vector2f dir)
{
	planets.push_back(Body(position, m, dir));
}

void Universe::addPlanet(Body p)
{
	planets.push_back(p);
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
					Vector2f p1(planets[i].position), p2(planets[j].position);
					float m1(planets[i].mass), m2(planets[j].mass);
					Vector2f d1(planets[i].direction), d2(planets[j].direction);

					if (m1 == 0 || m2 == 0)
					{
					   cout << "FATAL ERROR" << endl;
					   return;
					}

							planets.erase(planets.begin()+j);
					planets.erase(planets.begin()+i);

					Vector2f p1m1 = m1*d1;
					Vector2f p2m2 = m2*d2;

					Vector2f Pt = p1m1+p2m2;
					float Mt = m1+m2;
					Vector2f Df = Pt / Mt;

					Vector2f pos = (m1 >= m2 ? p1 : p2);

					planets.push_back(Body(pos, Mt, Df));
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
	if (planets.size())
	{
		for (Body& b : planets)
		{
			b.draw(window);
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