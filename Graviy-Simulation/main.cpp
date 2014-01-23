#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Body.hpp"

#define SIZE 6
#define ACCEL 9.82

using namespace sf;
using namespace std;

bool operator!=(const Body& a, const Body& b);

int main()
{
	RenderWindow window(VideoMode(800, 600), "");

	// Drawable elements
	vector<Body> planets;
	VertexArray line(Lines, 2);

	// Logic vars
	Clock timer;
	float delta_t(0);
	bool running = true;
	bool is_placing = false;

	while (running)
	{
		Event evt;
		while (window.pollEvent(evt))
		{
			if (evt.type == Event::Closed)
			{
				running = false;
			}

			if (evt.type == Event::KeyPressed)
			{
				if (evt.key.code == Keyboard::Escape)
				{
					running = false;
				}
			}

			if (evt.type == Event::MouseButtonPressed)
			{
				is_placing = true;

				line[0].position = Vector2f(evt.mouseButton.x, evt.mouseButton.y);
				line[1].position = Vector2f(evt.mouseButton.x, evt.mouseButton.y);
			}

			if (evt.type == Event::MouseMoved)
			{
				if (is_placing)
				{
					line[1].position = Vector2f(evt.mouseMove.x, evt.mouseMove.y);
				}
			}

			if (evt.type == Event::MouseButtonReleased)
			{
				if (is_placing)
				{
					Vector2f position(line[0].position.x - SIZE,
							line[0].position.y - SIZE);
					Body p(position, SIZE, line[1].position - line[0].position);
					cout << "vec: " << (line[1].position - line[0].position).x<<';'<<(line[1].position - line[0].position).y << endl;

					planets.push_back(p);
				}

				is_placing = false;
			}
		}

		window.clear();

		// Delta T
		delta_t = timer.restart().asSeconds();

		// Vector Preview - wow - need an arrow
		if (is_placing)
		{
			window.draw(line);
		}

		// Planets
		if (planets.size())
		{
			for (Body& b : planets)
			{
				b.move(delta_t);
			}
		}

		/*
		if (planets.size() > 1)
		{
			for (unsigned int i = 0; i < planets.size(); i++)
			{
				for (unsigned int j = 0; j < planets.size();j++)
				{
					if (i != j)
					{
						planets[i].applyGravityOf(planets[j], delta_t);
					}
				}
			}
		}
		*/

		for (Body& b : planets)
		{
			b.draw(window);
		}

		window.display();
	}

	return 0;
}
