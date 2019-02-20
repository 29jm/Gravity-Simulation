#include "Interface.hpp"

#include <SFML/Graphics.hpp>

using namespace sf;

Interface::Interface(RenderWindow& win, Universe& verse)
	: modifiers{false, false, false, false},
	universe(verse), window(win),
	view(Vector2f(0, 0), Vector2f(win.getSize())),
	base_line(BASE_LINE), line(Lines, 2),
	mass(MASS), num_planets(0)
{
	window.setView(view);

	line[0].color = Color::Blue;
	line[1].color = Color::Green;

	base_line.setFillColor(Color::Blue);

	font.loadFromFile("DejaVuSans.ttf");

	mass_text.setFont(font);
	// mass_text.setFillColor(Color::White);
	num_planets_text.setFont(font);
	// num_planets_text.setFillColor(Color::White);
	paths_text.setFont(font);
}

void Interface::handle_event(Event evt)
{
	if (evt.type == Event::Closed)
	{
		window.close();
	}
	else if (evt.type == Event::Resized)
	{
		view.setSize(evt.size.width, evt.size.height);
		window.setView(view);
	}
	else if (evt.type == Event::KeyPressed)
	{
		if (evt.key.code == Keyboard::Escape)
		{
			window.close();
		}

		switch (evt.key.code)
		{
		case Keyboard::PageUp:
		case Keyboard::Add:
			mass *= 10;
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
			break;
		case Keyboard::T:
			universe.togglePath();
			break;
		case Keyboard::Space:
			modifiers.is_placing = false;
			break;
		case Keyboard::LControl:
			modifiers.control = true;
			break;
		case Keyboard::LShift:
			modifiers.shift = true;
			break;
		case Keyboard::C:
			universe.createProtodisk(CLUSTER_NUMBER, CLUSTER_RADIUS, CLUSTER_MASS, view.getCenter());
			break;
		case Keyboard::Delete:
			universe.eraseAll();
			break;
		default:
			break;
		}
	}
	else if (evt.type == Event::KeyReleased)
	{
		switch (evt.key.code)
		{
		case Keyboard::LControl:
			modifiers.control = false;
			break;
		case Keyboard::LShift:
			modifiers.shift = false;
			break;
		default:
			break;
		}
	}
	else if (evt.type == Event::MouseButtonPressed)
	{
		if (modifiers.shift)
		{
			universe.eraseAt(window.mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y)));
		}
		else if (modifiers.control)
		{
			modifiers.is_moving = true;
		}
		else
		{
			modifiers.is_placing = true;

			line[0].position = window.mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
			line[1].position = line[0].position;
			start = Vector2f(evt.mouseButton.x, evt.mouseButton.y);
			end = start;

			Vector2f base_pos = Vector2f(line[0].position.x-BASE_LINE, line[0].position.y-BASE_LINE);
			base_line.setPosition(base_pos);
		}
	}
	else if (evt.type == Event::MouseMoved)
	{
		if (modifiers.is_placing)
		{
			line[1].position = window.mapPixelToCoords(Vector2i(evt.mouseMove.x, evt.mouseMove.y));
			end = Vector2f(evt.mouseMove.x, evt.mouseMove.y);
		}

		if (modifiers.is_moving)
		{
			view.move(mousePosition - window.mapPixelToCoords(Vector2i(evt.mouseMove.x, evt.mouseMove.y)));
			window.setView(view);
		}

		mousePosition = window.mapPixelToCoords(Vector2i(evt.mouseMove.x, evt.mouseMove.y));
	}

	if (evt.type == Event::MouseButtonReleased)
	{
		if (modifiers.is_placing)
		{
			modifiers.is_placing = false;

			Vector2f direction = end - start;
			float radius = Body::radiusForMass(mass);
			Vector2f position = Vector2f(line[0].position.x-radius,
				line[0].position.y-radius);
			
			universe.addPlanet(position, mass, direction);
		}

		if (modifiers.is_moving)
		{
			modifiers.is_moving = false;
		}
	}
	else if (evt.type == Event::MouseWheelMoved)
	{
		float factor = (evt.mouseWheel.delta < 1 ? 1.10 : 0.90);
		view.zoom(factor);
		window.setView(view);
	}

	mass_text.setPosition(Vector2f());
	uint32_t csize = mass_text.getCharacterSize();
	num_planets_text.setPosition(Vector2f(0, csize));
	paths_text.setPosition(Vector2f(0, csize*2));
}

void Interface::draw()
{
	if (modifiers.is_placing)
	{
		window.draw(line);
		window.draw(base_line);
	}

	num_planets = universe.getPlanetNumber();

	mass_text.setString("Mass: "+std::to_string(mass));
	num_planets_text.setString("Number of planets: "+std::to_string(num_planets));
	paths_text.setString(std::string("Paths: ")+(universe.isPathEnabled() ? "True" : "False"));

	// Reset view to draw text in window coordinates
	View v = window.getView();
	window.setView(window.getDefaultView());

	window.draw(mass_text);
	window.draw(num_planets_text);
	window.draw(paths_text);

	window.setView(v);
}
