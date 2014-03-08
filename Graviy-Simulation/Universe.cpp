#include "Universe.hpp"

using namespace sf;

void Universe::addPlanet(Vector2i position, float m, Vector2f dir)
{
	planets.push_back(Body(position, m, dir));
}

void Universe::move(float delta_t)
{
	if (planets.size() < 2)
	{
		return;
	}

	for (unsigned int i = 0; i < planets.size(); i++)
	{
		for (unsigned int j = 0; j < planets.size();j++)
		{
			if (i != j)
			{
				if (planets[i].collideWith(planets[j]))
				{
                    Vector2f p1m1 = planets[i].mass*planets[i].direction;
                    Vector2f p2m2 = planets[j].mass*planets[j].direction;

                    Vector2f Pt = p1m1+p2m2;
                    float Mt = planets[i].mass+planets[j].mass;
                    Vector2f Df = Pt / Mt;

                    Body p(Vector2i(int(planets[i].position.x), int(planets[i].position.y)), Mt, Df);
                    
                    planets.erase(planets.begin()+j);
                    planets.erase(planets.begin()+i);

                    planets.push_back(p);
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