#pragma once

#include <vector>
#include <map>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Direction.hpp"
#include "Item.hpp"

class Room : public Entity
{
private:
	// Data
	std::vector<Direction> _doors;
	std::map<Direction, Room*> _nextRooms;
	sf::Vector2f _center;
	sf::Vector2f _size;
	std::string _name;
	
	// Cosmetic
	sf::Color _bg;
	sf::RectangleShape _shape;
	std::vector<sf::RectangleShape> _doorsShape;

	// Items
	std::vector<std::shared_ptr<Item>> _items;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_shape);
		for (const auto& ds : _doorsShape)
			target.draw(ds);
		for (const auto& i : _items)
			target.draw(*i);
	}

	void initDoorFromDirection(sf::RectangleShape& shape, Direction dir, const sf::Vector2f& center)
	{
		shape.setFillColor(_bg);

		const sf::Vector2f shapeSize = { 100, 100 };

		shape.setSize(shapeSize);
		shape.setOrigin({ shapeSize.x / 2, shapeSize.y / 2 });
		shape.setPosition(center);

		int h = 0, v = 0;
		Directions::getHV(h, v, dir);
		shape.move({ h * (_size.x / 2 + shapeSize.x / 2), v * (_size.y / 2 + (shapeSize.y / 2)) });
	}

public:
	Room() {}

	Room(const sf::Color& bg, const sf::Vector2f& size, const sf::Vector2f& center, const std::string& name = "")
	{
		_center = center;
		_bg = bg;
		_size = size;
		_name = name;

		_shape.setFillColor(_bg);
		_shape.setSize(_size);
		_shape.setOrigin({ _size.x / 2, _size.y / 2 });
		_shape.setPosition(_center);
	}

	void setItems(const std::vector<std::shared_ptr<Item>>& items)
	{
		_items = items;
	}

	void setNextRooms(const std::vector<Direction>& doors, const std::vector<Room*>& next)
	{
		_doors = doors;
		for (int i = 0; i < _doors.size(); i++)
		{
			Direction& d = _doors[i];
			if (d != None)
			{
				sf::RectangleShape shape;
				initDoorFromDirection(shape, d, _center);
				_doorsShape.push_back(shape);
				_nextRooms[d] = next[i];
			}
		}
	}

	bool pointInRoom(const sf::Vector2f& point)
	{
		return _shape.getGlobalBounds().contains(point);
	}

	bool pointInDoor(const sf::Vector2f& point, Direction& direction)
	{
		for (int i = 0; i < _doors.size(); i++)
		{
			if (_doorsShape[i].getGlobalBounds().contains(point))
			{
				direction = _doors[i];
				return true;
			}
		}
		return false;
	}

	bool pointInDoor(const sf::Vector2f& point)
	{
		Direction dir = None;
		return pointInDoor(point, dir);
	}

	sf::Vector2f center() const
	{
		return _center;
	}

	sf::Vector2f size() const
	{
		return _size;
	}

	std::string name() const
	{
		return _name;
	}

	sf::Vector2f doorBorder(Direction dir) const
	{
		sf::Vector2f offset = _center;

		int h = 0, v = 0;
		Directions::getHV(h, v, dir);
		
		offset.x -= h * (_size.x / 2);
		offset.y -= v * (_size.y / 2);

		return offset;
	}

	sf::Vector2f spawn(Direction dir) const
	{
		sf::Vector2f border = doorBorder(dir);

		int h = 0, v = 0;
		Directions::getHV(h, v, dir);

		border.x += h * 50;
		border.y += v * 50;

		return border;
	}

	Room* nextRoom(Direction dir)
	{
		return _nextRooms[dir];
	}

	std::shared_ptr<Item> closestReachableItem(const sf::Vector2f& position, float range)
	{
		for (const auto& i : _items)
		{
			if (!i->alive()) continue;
			if (vm::dist(i->position(), position) < i->range() + range)
				return i;
		}
		return nullptr;
	}

	virtual void updateEvent(const sf::Event& event)
	{
	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
	}
};