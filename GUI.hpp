#pragma once

#include "Minimap.hpp"

class GUI : public LiveEntity
{
private:
	// Data
	std::vector<Widget*> _widgets;
	Map* _map;
	Minimap _minimap;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for(const auto& w : _widgets)
			target.draw(*w);
	}

public:
	GUI(Map* map) :
		_map(map), _minimap(_map)
	{
		// Minimap
		_minimap.setRooms(_map->pixelRooms(), _map->textureSize());
		_minimap.setActiveRoom(_map->currentRoom()->pixel());
		_widgets.push_back(&_minimap);
	}

	virtual void updateEvent(const sf::Event& event)
	{
		for (const auto& w : _widgets)
			w->updateEvent(event);
	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
		for (const auto& w : _widgets)
			w->update(dt, mousePos);
	}
};