#pragma once

#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "Map.h"
#include "Widget.hpp"

class Minimap : public Widget
{
private:
	// Const
	const sf::Vector2f _position = { 10, 10 };
	const sf::Vector2f _size = { 250, 250 };

	const float _outlineBorder = 3.f;
	const float _lineLength = 12.f;

	// Data
	std::vector<sf::Vector2i> _rooms;
	sf::Vector2i _current;
	std::shared_ptr<Map> _map;

	// Cosmetic
	sf::VertexArray _minimap;
	sf::VertexArray _doors;
	sf::VertexArray _panel;
	sf::RenderTexture _texture;
	sf::RectangleShape _bg;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_panel, &_texture.getTexture());
	}

	void resetTexture()
	{
		_texture.clear(sf::Color::Transparent);
		_texture.draw(_bg);
		_texture.draw(_doors);
		_texture.draw(_minimap);
		_texture.display();
	}

	void foundRoom(const sf::Vector2i& room)
	{
		auto it = std::find(_rooms.begin(), _rooms.end(), room);
		if (it != _rooms.end())
		{
			int v = int((it - _rooms.begin()) * 4);
			for (int i = v; i < v + 4; i++)
				_minimap[i].color = sf::Color::Cyan;
		}

		resetTexture();
	}

public:
	Minimap(const std::shared_ptr<Map>& map)
	{
		_map = map;

		_panel.setPrimitiveType(sf::Quads);
		_minimap.setPrimitiveType(sf::Quads);
		_doors.setPrimitiveType(sf::Lines);
		
		_bg.setPosition({ _outlineBorder, _outlineBorder });
		_bg.setFillColor(sf::Color(80, 80, 80, 128));
		_bg.setOutlineColor(sf::Color(120, 120, 120, 128));
		_bg.setOutlineThickness(_outlineBorder);

		_texture.create((unsigned int)_size.x, (unsigned int)_size.y);
		_bg.setSize({ _size.x - _outlineBorder * 2, _size.y - _outlineBorder * 2 });

		_panel.append(sf::Vertex({ _position.x, _position.y }, { 0, 0 }));
		_panel.append(sf::Vertex({ _position.x + _size.x + _outlineBorder * 2, _position.y}, { _size.x, 0 }));
		_panel.append(sf::Vertex({ _position.x + _size.x + _outlineBorder * 2, _position.y + _size.y + _outlineBorder * 2 }, { _size.x, _size.y }));
		_panel.append(sf::Vertex({ _position.x, _position.y + _size.y + _outlineBorder * 2 }, { 0, _size.y }));
	}

	void setRooms(const std::vector<sf::Vector2i>& rooms, const sf::Vector2i& textureSize)
	{
		_rooms = rooms;
		sf::Vector2f ratio = { _size.x / textureSize.y , _size.y / textureSize.y };

		for (int i = 0; i < _rooms.size(); i++)
		{
			sf::Vector2f p = {
				ratio.x * (float)_rooms[i].x + _outlineBorder,
				ratio.y * (float)_rooms[i].y + _outlineBorder
			};

			_minimap.append(sf::Vertex({ p.x,			p.y },			 sf::Color::White));
			_minimap.append(sf::Vertex({ p.x + ratio.x,	p.y },			 sf::Color::White));
			_minimap.append(sf::Vertex({ p.x + ratio.x,	p.y + ratio.y }, sf::Color::White));
			_minimap.append(sf::Vertex({ p.x,			p.y + ratio.y }, sf::Color::White));

			// Doors
			std::vector<Direction> dirs = _map->atPixel(_rooms[i])->doors();
			for (const auto& d : dirs) 
			{
				int h = 0, v = 0;
				Directions::getHV(h, v, d);

				_doors.append(sf::Vertex({ p.x + ratio.x / 2, p.y + ratio.y / 2 }, sf::Color::White));
				_doors.append(sf::Vertex({ p.x + ratio.x / 2 + h * _lineLength,  p.y + ratio.y / 2 + v * _lineLength }, sf::Color::White));
			}
		}

		resetTexture();
	}

	void setActiveRoom(const sf::Vector2i& room)
	{
		auto currentRoom = std::find(_rooms.begin(), _rooms.end(), _current);
		auto newRoom = std::find(_rooms.begin(), _rooms.end(), room);

		if (currentRoom != _rooms.end())
			foundRoom({ currentRoom->x, currentRoom->y });

		if (newRoom != _rooms.end())
		{
			int v = int((newRoom - _rooms.begin()) * 4);
			for (int i = v; i < v + 4; i++)
				_minimap[i].color = sf::Color(230, 20, 20);
			_current = { newRoom->x, newRoom->y };
		}

		resetTexture();
	}

	virtual void update(float dt, const sf::Vector2f& mousePos) override
	{
		if (_map->changedRoom())
		{
			foundRoom(_map->currentRoom()->pixel());
			setActiveRoom(_map->currentRoom()->pixel());
		}
	}
};