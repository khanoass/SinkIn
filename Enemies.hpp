#pragma once

#include "Shadow.hpp"
#include "Bullets.hpp"
#include "Items.hpp"

class Enemies : public LiveEntity
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_currentEnemies == nullptr) return;
		for (auto& i : *_currentEnemies)
		{
			if (i != nullptr)
				target.draw(*i, states);
		}
	}

	std::map<std::string, std::vector<std::shared_ptr<Enemy>>> _enemiesRoom;
	std::vector<std::shared_ptr<Enemy>>* _currentEnemies = nullptr;

	std::shared_ptr<Player> _player;
	std::shared_ptr<Bullets> _bullets;
	std::shared_ptr<Items> _items;

public:
	Enemies(const std::shared_ptr<Player>& player, const std::shared_ptr<Bullets>& bullets, const std::shared_ptr<Items>& items)
	{
		_player = player;
		_bullets = bullets;
		_items = items;
	}

	void addRoom(const std::string& name)
	{
		_enemiesRoom[name] = std::vector<std::shared_ptr<Enemy>>();
	}

	void add(const std::shared_ptr<Enemy>& enemy, const std::string& room)
	{
		_enemiesRoom[room].push_back(enemy);
	}

	void setCurrentRoom(const std::string& name)
	{
		_currentEnemies = &_enemiesRoom[name];
	}

	virtual void update(float dt, const sf::Vector2f& mousePos) override
	{
		if (_currentEnemies == nullptr) return;
		for (auto& i : *_currentEnemies)
		{
			if (i != nullptr)
				i->update(dt, mousePos, _player, _bullets->getBullets(), _items->currentWeapons());
		}
	}
};