#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Universe.hpp"
#include "Body.hpp"

#define MASS 1
#define BASE_LINE 2

using namespace sf;
using namespace std;

int main()
{
	RenderWindow window(VideoMode(800, 600), "Gravity Simulation by Johan");

	// Drawable elements
	Universe universe;
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
                case Keyboard::PageUp:
				case Keyboard::Add:
					mass += 100;
					cout << "Mass is now " << mass << endl;
					break;
                case Keyboard::PageDown:
				case Keyboard::Subtract:
					mass = (mass > 0 ? mass - 100 : 100);
					if (mass < 0)
					{
						mass = 1;
					}
					cout << "Mass is now " << mass << endl;
					break;
				case Keyboard::T:
					trace = !trace;
					cout << "Trace mode activated" << endl;
					break;
				case Keyboard::Space:
                    if (is_placing)
                    {
                        cout << "Planet placement cancelled" << endl;
                    }
                    is_placing = false;
                    break;
				default:
					break;
				}
			}

			if (evt.type == Event::MouseButtonPressed)
			{
                if (evt.key.shift)
                {
                    universe.eraseAt(Vector2f(evt.mouseButton.x, evt.mouseButton.y));
                }
                else
                {
                    is_placing = true;

                    line[0].position = Vector2f(evt.mouseButton.x, evt.mouseButton.y);
                    line[1].position = Vector2f(evt.mouseButton.x, evt.mouseButton.y);
                    base_line.setPosition(evt.mouseButton.x - BASE_LINE, evt.mouseButton.y - BASE_LINE);
                }
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
					Vector2i position(line[0].position.x - MASS,
							line[0].position.y - MASS);

					universe.addPlanet(position, mass, line[1].position - line[0].position);
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
		universe.move(delta_t);
		universe.draw(window);
		
		window.display();

		sleep(milliseconds(3));
	}

	return 0;
}
