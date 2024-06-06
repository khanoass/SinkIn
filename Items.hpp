#pragma once

#include "Weapon.h"

class Items : public sf::Drawable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_currentItems == nullptr) return;
		for (auto& i : *_currentItems)
		{
			if (i != nullptr)
				target.draw(*i, states);
		}
	}

	std::map<std::string, std::vector<std::shared_ptr<Item>>> _itemsRoom;
	std::vector<std::shared_ptr<Item>>* _currentItems = nullptr;

	std::map<std::string, std::vector<std::shared_ptr<Weapon>>> _weaponsRoom; // Items already contains all the weapons, this is to update them separately
	std::vector<std::shared_ptr<Weapon>>* _currentWeapons = nullptr;

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

	void addWeapon(const std::shared_ptr<Weapon>& weapon, const std::string& room, const std::shared_ptr<Bullets>& bullets)
	{
		weapon->setBullets(bullets);
		_itemsRoom[room].push_back(weapon);
		_weaponsRoom[room].push_back(weapon);
	}

	void setCurrentRoom(const std::string& name)
	{
		_currentItems = &_itemsRoom[name];
		_currentWeapons = &_weaponsRoom[name];
	}

	void changeWeaponRoom(const std::shared_ptr<Weapon>& weapon, const std::string& oldRoom, const std::string& newRoom)
	{
		std::vector<std::shared_ptr<Item>>& rio = _itemsRoom[oldRoom];
		std::vector<std::shared_ptr<Weapon>>& rwo = _weaponsRoom[oldRoom];

		auto iti = std::find(rio.begin(), rio.end(), weapon);
		auto itw = std::find(rwo.begin(), rwo.end(), weapon);

		if(iti != rio.end())
			rio.erase(iti);
		if (itw != rwo.end())
			rwo.erase(itw);

		_itemsRoom[newRoom].push_back(weapon);
		_weaponsRoom[newRoom].push_back(weapon);
	}

	std::vector<std::shared_ptr<Weapon>>* currentWeapons()
	{
		return _currentWeapons;
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

	void update(float dt, const sf::Vector2f& mousePos)
	{
		if (_currentWeapons == nullptr) return;
		for (auto& i : *_currentWeapons)
		{
			if (i != nullptr)
				i->updateDrop(dt, mousePos);
		}
	}
};