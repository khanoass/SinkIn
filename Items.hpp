#pragma once

#include "Weapon.h"
#include "Boost.h"
#include "LiveEntity.hpp"

class Boost;
class Glock;

class Items : public LiveEntity
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_currentItems == nullptr) return;
		for (auto& i : *_currentItems)
		{
			if(i != nullptr)
				target.draw(*i, states);
		}
	}

	std::map<std::string, std::vector<std::shared_ptr<Item>>> _itemsRoom;
	std::vector<std::shared_ptr<Item>>* _currentItems;

	std::map<std::string, std::vector<std::shared_ptr<Weapon>>> _weaponsRoom; // Items already contains all the weapons, this is to update them separately
	std::vector<std::shared_ptr<Weapon>>* _currentWeapons;

public:
	Items()
	{
	}

	void addRoom(const std::string& name)
	{
		_itemsRoom[name] = std::vector<std::shared_ptr<Item>>();
	}

	// Add 2 times if it's a weapon!
	void add(const std::shared_ptr<Item>& item, const std::string& room, const std::shared_ptr<Weapon>& weapon = nullptr)
	{
		_itemsRoom[room].push_back(item);
		if(weapon != nullptr)
			_weaponsRoom[room].push_back(weapon);
	}

	void setCurrentRoom(const std::string& name)
	{
		_currentItems = &_itemsRoom[name];
		_currentWeapons = &_weaponsRoom[name];
	}

	void changeItemRoom(const std::shared_ptr<Item>& item, const std::string& oldRoom, const std::string& newRoom)
	{
		std::vector<std::shared_ptr<Item>>& ri = _itemsRoom[oldRoom];
		auto it = std::find(ri.begin(), ri.end(), item);
		if(it != ri.end())
			ri.erase(it);
		std::vector<std::shared_ptr<Item>>& rn = _itemsRoom[newRoom];
		rn.push_back(item);
	}

	std::shared_ptr<Item> closestReachableItem(const sf::Vector2f& position, float range)
	{
		for (const auto& i : *_currentItems)
		{
			if (i != nullptr)
			{
				if (!i->alive()) continue;
				if (vm::dist(i->position(), position) < i->range() + range)
					return i;
			}
		}
		return nullptr;
	}

	virtual void updateEvent(const sf::Event& event) override
	{
	}

	virtual void update(float dt, const sf::Vector2f& mousePos) override
	{
		if (_currentWeapons == nullptr) return;
		for (auto& i : *_currentWeapons)
		{
			if (i != nullptr)
				i->updateDrop(dt, mousePos);
		}
	}
};