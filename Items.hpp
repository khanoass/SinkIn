#pragma once

#include "Weapon.h"
#include "Boost.h"
#include "LiveEntity.hpp"

class Boost;
class Glock;

class Items : public Entity
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_currentItems == nullptr) return;
		for (auto& i : *_currentItems)
			target.draw(*i, states);
	}

	std::map<std::string, std::vector<std::shared_ptr<Item>>> _itemsRoom;
	std::vector<std::shared_ptr<Item>>* _currentItems;

public:
	Items()
	{
	}

	void addRoom(const std::string& name)
	{
		_itemsRoom[name] = std::vector<std::shared_ptr<Item>>();
	}

	void add(const std::shared_ptr<Item>& item, const std::string& room)
	{
		_itemsRoom[room].push_back(item);
	}

	void setCurrentRoom(const std::string& name)
	{
		_currentItems = &_itemsRoom[name];
	}

	std::shared_ptr<Item> closestReachableItem(const sf::Vector2f& position, float range)
	{
		for (const auto& i : *_currentItems)
		{
			if (!i->alive()) continue;
			if (vm::dist(i->position(), position) < i->range() + range)
				return i;
		}
		return nullptr;
	}
};