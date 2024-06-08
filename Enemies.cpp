#include "Enemies.h"
#include "Map.h"

void Enemies::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (_currentEnemies == nullptr) return;
	for (auto& i : *_currentEnemies)
	{
		if (i != nullptr)
			target.draw(*i, states);
	}
}

Enemies::Enemies(const std::shared_ptr<Player>& player, const std::shared_ptr<Bullets>& bullets, const std::shared_ptr<Items>& items)
{
	_player = player;
	_bullets = bullets;
	_items = items;
}

void Enemies::setMap(const std::shared_ptr<Map>& map)
{
	_map = map;
}

void Enemies::addRoom(const std::string& name)
{
	_enemiesRoom[name] = std::vector<std::shared_ptr<Enemy>>();
}

void Enemies::add(const std::shared_ptr<Enemy>& enemy, const std::shared_ptr<Room>& room)
{
	enemy->setRoomParams(room->center(), room->size(), room->name());
	enemy->setItems(_items);
	_enemiesRoom[room->name()].push_back(enemy);
}

void Enemies::setCurrentRoom(const std::string& name)
{
	_currentEnemies = &_enemiesRoom[name];
}

void Enemies::resetPositionsForRoom(const std::string& name)
{
	auto en = getAllInRoom(name);
	for (auto& i : en)
		i->resetPosition();
}

std::vector<std::shared_ptr<Enemy>>& Enemies::getAllInRoom(const std::string& name)
{
	return _enemiesRoom[name];
}

void Enemies::update(float dt, const sf::Vector2f& mousePos)
{
	if (_currentEnemies == nullptr) return;
	for (auto& i : *_currentEnemies)
	{
		if (i != nullptr)
			i->update(dt, mousePos, _player, _bullets->getBullets(), _items->currentWeapons(), _currentEnemies);
	}
}