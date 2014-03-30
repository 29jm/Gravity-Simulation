#ifndef BODY_HPP
#define BODY_HPP

#include <SFML/Graphics.hpp>

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

sf::Color interpolate(sf::Color a, sf::Color b, float delta_t);

#endif // BODY_HPP
