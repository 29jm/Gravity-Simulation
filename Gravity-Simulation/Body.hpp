#ifndef BODY_HPP
#define BODY_HPP

#include <SFML/Graphics.hpp>

#define G 1
#define DENSITY 100 // kg/m^3

class Body
{
public:
    Body(sf::Vector2f pos, uint64_t m, sf::Vector2f dir, bool with_path);

	void move(float dt);
	void applyGravityOf(const Body& b, float dt);
	void draw(sf::RenderWindow& window);
    bool collideWith(const Body& b) const;
    bool contains(const sf::Vector2f& point) const;
    void setPathEnabled(bool state);
    sf::VertexArray getPath() const;

    static float radiusForMass(uint64_t mass);

    sf::Vector2f position;
    sf::Vector2f direction;
    uint64_t mass;
    float radius;
    bool show_path;

private:
	float getDistanceTo(const Body& b) const;
    sf::Vector2f getCenter() const;

	sf::CircleShape shape;
    sf::VertexArray path;
};

// Handy helper functions

// ratio is a number between 0.0 and 1.0
template <typename T, typename Real>
T interpolate(T a, T b, Real ratio)
{
    return (b - a) * ratio + a;
}

template <typename Real>
sf::Color interpolate(sf::Color a, sf::Color b, Real ratio)
{
    sf::Color c;
    c.r = (b.r-a.r) * ratio + a.r;
    c.g = (b.g-a.g) * ratio + a.g;
    c.b = (b.b-a.b) * ratio + a.b;

    return c;
}

#endif // BODY_HPP
