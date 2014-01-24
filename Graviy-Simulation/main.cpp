#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Body.hpp"

#define MASS 1
#define BASE_LINE 2

using namespace sf;
using namespace std;

int main()
{
	RenderWindow window(VideoMode(800, 600), "Gravity Simulation by Johan");

	// Drawable elements
	vector<Body> planets;
	VertexArray line(Lines, 2);
	CircleShape base_line(BASE_LINE);

	// Logic vars
	Clock timer;
	float delta_t(0);
	int mass(MASS);
	bool running = true;
	bool is_placing = false;
	bool trace = false;

	// Some inits
	line[0].color = Color::Blue;
	line[1].color = Color::Green;
	base_line.setFillColor(Color::Blue);

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

				switch (evt.key.code) // TODO: +- mass; toggle "clear()"; pause;
				{
				case Keyboard::Add:
					mass += 100;
					cout << "Mass is now " << mass << endl;
					break;
				case Keyboard::Subtract:
					mass -= 100;
					break;
				case Keyboard::T:
					trace = !trace;
					break;
				case Keyboard::Space:
					is_placing = false;
					break;
				default:
					break;
				}
			}

			if (evt.type == Event::MouseButtonPressed)
			{
				is_placing = true;

				line[0].position = Vector2f(evt.mouseButton.x, evt.mouseButton.y);
				line[1].position = Vector2f(evt.mouseButton.x, evt.mouseButton.y);
				base_line.setPosition(evt.mouseButton.x - BASE_LINE, evt.mouseButton.y - BASE_LINE);
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
					Vector2f position(line[0].position.x - MASS,
							line[0].position.y - MASS);
					Body p(position, mass, line[1].position - line[0].position);

					planets.push_back(p);
				}

				is_placing = false;
			}
		}

		if (!trace)
		{
			window.clear();
		}

		// Delta T
		delta_t = timer.restart().asSeconds();

		// Vector Preview - wow - need an arrow - hard
		if (is_placing)
		{
			window.draw(line);
			window.draw(base_line);
		}

		// Planets
		if (planets.size() > 1)
		{
			for (unsigned int i = 0; i < planets.size(); i++)
			{
				for (unsigned int j = 0; j < planets.size();j++)
				{
					if (i != j)
					{
						if (planets[i].collideWith(planets[j]))
						{
							Body p(planets[i].getPosition(), planets[i].getMass()+planets[j].getMass(),
								   planets[i].getDirection()+planets[j].getDirection());
							planets.erase(planets.begin()+i);
							planets.erase(planets.begin()+j);
							planets.push_back(p);
						}

						planets[i].applyGravityOf(planets[j], delta_t);
					}
				}
			}
		}

		if (planets.size())
		{
			for (Body& b : planets)
			{
				b.move(delta_t);
				b.draw(window);
			}
		}

		window.display();
	}

	return 0;
}
