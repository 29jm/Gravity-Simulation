#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Universe.hpp"
#include "Body.hpp"

#define MASS 10
#define BASE_LINE 2

using namespace sf;
using namespace std;

int main()
{
	unsigned int width(800), height(600);
	RenderWindow window(VideoMode(width, height), "Gravity Simulation by Johan");
	View view(FloatRect(0, 0, width, height));

	// Drawable elements
	Universe universe;
	VertexArray line(Lines, 2);
	CircleShape base_line(BASE_LINE);

	// Logic vars
	Clock timer;
	float delta_t(0);
	int mass(MASS);
	Vector2f mousePosition;
	Vector2f start, end;
	bool running = true;
	bool is_placing = false;
	bool is_moving = false;
	bool trace = false;
	bool control = false;
	bool shift = false;

	// Some inits
	line[0].color = Color::Blue;
	line[1].color = Color::Green;
	base_line.setFillColor(Color::Blue);
	window.setView(view);

	while (running)
	{
		Event evt;
		while (window.pollEvent(evt))
		{
			if (evt.type == Event::Closed)
			{
				running = false;
			}

			if (evt.type == Event::Resized)
			{
				view.setSize(evt.size.width, evt.size.height);
				window.setView(view);
			}

			if (evt.type == Event::KeyPressed)
			{
				if (evt.key.code == Keyboard::Escape)
				{
					running = false;
				}

				switch (evt.key.code)
				{
                case Keyboard::PageUp:
				case Keyboard::Add:
					mass *= 10;
					if (mass < 0)
					{
						mass = MASS;
					}
					cout << "Mass is now " << mass << endl;
					break;

                case Keyboard::PageDown:
				case Keyboard::Subtract:
					if (mass > 10)
					{
						mass /= 10;
					}
					else
					{
						mass = MASS;
					}
					cout << "Mass is now " << mass << endl;
					break;

				case Keyboard::T:
					trace = !trace;
					cout << "Trace mode " << (trace ? "enabled" : "disabled") << endl;
					break;

				case Keyboard::Space:
                    if (is_placing)
                    {
                        cout << "Planet placement cancelled" << endl;
                    }
      
                    is_placing = false;
                    break;

                case Keyboard::LControl:
                	control = true;
                	break;

                case Keyboard::LShift:
                	shift = true;
                	break;
				default:
					break;
				}
			}

			if (evt.type == Event::KeyReleased)
			{
				switch (evt.key.code)
				{
				case Keyboard::LControl:
					control = false;

				case Keyboard::LShift:
					shift = false;

				default:
					break;
				}
			}

			if (evt.type == Event::MouseButtonPressed)
			{
                if (shift)
                {
                    universe.eraseAt(Vector2f(evt.mouseButton.x, evt.mouseButton.y));
                }
                else if (control)
                {
                	is_moving = true;
                }
                else
                {
                    is_placing = true;

                    line[0].position = window.mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
                    line[1].position = line[0].position;
                    start = Vector2f(evt.mouseButton.x, evt.mouseButton.y);
                    end = start;

                    Vector2f base = 
                    	window.mapPixelToCoords(Vector2i(evt.mouseButton.x - BASE_LINE, evt.mouseButton.y - BASE_LINE));
                    base_line.setPosition(base);
                }
            }

			if (evt.type == Event::MouseMoved)
			{
				if (is_placing)
				{
					line[1].position = window.mapPixelToCoords(Vector2i(evt.mouseMove.x, evt.mouseMove.y));
					end = Vector2f(evt.mouseMove.x, evt.mouseMove.y);
				}

				if (is_moving)
				{
					view.move(mousePosition - Vector2f(evt.mouseMove.x, evt.mouseMove.y));
					window.setView(view);
				}

				mousePosition = Vector2f(evt.mouseMove.x, evt.mouseMove.y);
			}

			if (evt.type == Event::MouseButtonReleased)
			{
				if (is_placing)
				{
					is_placing = false;

					Vector2f direction = end - start;

					Body p(Vector2f(0, 0), mass, direction);

					p.position = Vector2f(line[0].position.x,
							line[0].position.y);;

					universe.addPlanet(p);
				}

				if (is_moving)
				{
					is_moving = false;
				}
			}

			if (evt.type == Event::MouseWheelMoved)
			{
				float factor = (evt.mouseWheel.delta < 1 ? 1.10 : 0.90);
				view.zoom(factor);
				window.setView(view);
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
