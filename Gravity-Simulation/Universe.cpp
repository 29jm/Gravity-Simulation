#include "Universe.hpp"

#include <set>

using namespace sf;
using namespace std;

Universe::Universe()
	: followedPlanet(nullptr), old_paths(LinesStrip), show_path(false)
{

}

void Universe::addPlanet(Vector2f position, uint64_t m, Vector2f dir)
{
	if (followedPlanet) {
		position += followedPlanet->position;
		dir += followedPlanet->direction;
	}

	planets.push_front(Body(position, m, dir, show_path));
}

void Universe::addPlanet(Body p)
{
	p.setPathEnabled(show_path);
	planets.push_front(p);
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
		planets.push_front(p);
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
	std::set<int> dlist;
	std::list<Body> new_planets;
	int i = 0, j = 0;

	for (auto it_i = planets.begin(); it_i != planets.end(); it_i++) {
		j = 0;
		auto it_j = planets.begin();
		for (; it_j != planets.end(); it_j++) {
			Body& a = *it_i;
			Body& b = *it_j;

			if (it_i == it_j || dlist.find(i) != dlist.end() || dlist.find(j) != dlist.end()) {
				j++;
				continue;
			}

			if (a.collideWith(b))
			{
				// Merge the colliding planets
				Body p = combinedPlanets(a, b);
				p.setPathEnabled(show_path);

				if (&a == followedPlanet || &b == followedPlanet) {
					p.next_to_be_followed = true;
				}

				new_planets.push_back(p);

				// Save their paths
				savePlanetPath(a);
				savePlanetPath(b);

				// Mark the colliding planets for deletion
				dlist.insert(i);
				dlist.insert(j);

			}
			else
			{
				a.applyGravityOf(b, delta_t);
			}

			j++;
		}
		i++;
	}

	auto it = planets.begin();
	i = 0;
	while (it != planets.end()) {
		if (dlist.find(i) != dlist.end()) {
			if (&*it == followedPlanet) {
				followedPlanet = nullptr;
			}

			planets.erase(it++);
		} else {
			it++;
		}

		i++;
	}

	for (Body& b : new_planets) {
		planets.push_front(b);

		if (planets.front().next_to_be_followed) {
			followedPlanet = &planets.front();
			planets.front().next_to_be_followed = false;
		}
	}

	for (Body& b : planets)
	{
		b.move(delta_t, followedPlanet ? followedPlanet->position : sf::Vector2f());
	}
}

void Universe::draw(RenderWindow& window)
{
	for (Body& b : planets)
	{
		b.draw(window, followedPlanet);
	}

	if (show_path && !followedPlanet)
	{
		for (VertexArray& path : old_paths)
		{
			window.draw(path);
		}
	}
}

Body* Universe::planetAt(const sf::Vector2f &pos)
{
	for (Body& p : planets)
	{
		if (p.contains(Vector2f(pos.x, pos.y)))
		{
			return &p;
		}
	}

	return nullptr;
}

sf::Vector2f Universe::followPlanetAt(sf::Vector2f pos)
{
	sf::Vector2f oldFollow;

	if (followedPlanet) {
		pos += followedPlanet->position;
		oldFollow = followedPlanet->position;
	}

	followedPlanet = planetAt(pos);

	if (followedPlanet) {
		// Purge paths
		old_paths.clear();
		togglePath();
		togglePath();

		return followedPlanet->position - oldFollow;
	} else {
		return -oldFollow;
	}
}

void Universe::eraseAt(const Vector2f& pos)
{
	for (auto it = planets.begin(); it != planets.end(); it++)
	{
		if (it->contains(Vector2f(pos.x, pos.y)))
		{
			if (&*it == followedPlanet) {
				followedPlanet = nullptr;
			}

			planets.erase(it);
			return;
		}
	}
}

void Universe::eraseAll()
{
	planets.clear();
	old_paths.clear();
	followedPlanet = nullptr;
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
