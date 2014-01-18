#include <iostream>
#include "Body.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
	RenderWindow window(VideoMode(800, 600), "Gravity Simulation");

	Body earth(10, Vector2f(400, 300), 0);
	Body mun(2, Vector2f(300, 300), 2);

	Clock timer;
	bool running = true;
	while (running)
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				running = false;
				   break;
			default:
				break;
			}
		}

		earth.applyGravityOf(mun, timer.restart().asSeconds());
		mun.applyGravityOf(earth, timer.restart().asSeconds());

		window.clear();
		earth.draw(window);
		mun.draw(window);
		window.display();

		earth.update(mun, timer.restart().asSeconds());
		mun.update(earth, timer.restart().asSeconds());
	}

	return 0;
}

