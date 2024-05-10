#pragma once

#include "Entity.hpp"

class LiveEntity : public Entity
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

public:
	virtual void updateEvent(const sf::Event& event) = 0;
	virtual void update(float dt, const sf::Vector2f& mousePos) = 0;
};