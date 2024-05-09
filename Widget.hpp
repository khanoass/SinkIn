#pragma once

#include "LiveEntity.hpp"

class Widget : public LiveEntity
{
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
	}

public:
	virtual void updateEvent(const sf::Event& event)
	{
	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
	}
};