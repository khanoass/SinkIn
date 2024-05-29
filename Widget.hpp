#pragma once

#include "LiveEntity.hpp"

class Widget : public LiveEntity
{
	bool _visible = true;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
	}

public:

	virtual void update(float dt, const sf::Vector2f& mousePos) override
	{
	}

	void setVisibility(bool visibility)
	{
		_visible = visibility;
	}

	bool visible() const
	{
		return _visible;
	}
};