#include <SFML/Graphics.hpp>

#include "Universe.hpp"
#include "Body.hpp"
#include "Interface.hpp"

#define MAX_DELTA_T (1.0f/30)

using namespace sf;
using namespace std;

int main()
{
	RenderWindow window(VideoMode(1024, 768), "Gravity Simulation by Johan");
	Universe universe;
	Interface interface(window, universe);

	// Logic vars
	Clock timer;
	float delta_t(0);

	while (window.isOpen())
	{
		Event evt;
		while (window.pollEvent(evt))
		{
			interface.handle_event(evt);
		}

		delta_t = timer.restart().asSeconds();

		if (delta_t > MAX_DELTA_T)
		{
			delta_t = MAX_DELTA_T;
		}

		universe.move(delta_t);

		window.clear();

		universe.draw(window);
		interface.draw();
		
		window.display();
	}

	return 0;
}
