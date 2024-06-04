#pragma once

#include "Shadow.hpp"
#include "Bullets.hpp"
#include "Items.hpp"

class Map;
class Room;

class Enemies : public LiveEntity
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::map<std::string, std::vector<std::shared_ptr<Enemy>>> _enemiesRoom;
	std::vector<std::shared_ptr<Enemy>>* _currentEnemies = nullptr;

	std::shared_ptr<Player> _player;
	std::shared_ptr<Bullets> _bullets;
	std::shared_ptr<Items> _items;
	std::shared_ptr<Map> _map;

public:
	Enemies(const std::shared_ptr<Player>& player, const std::shared_ptr<Bullets>& bullets, const std::shared_ptr<Items>& items);

	void setMap(const std::shared_ptr<Map>& _map);
	void addRoom(const std::string& name);
	void add(const std::shared_ptr<Enemy>& enemy, const std::shared_ptr<Room>& room);
	void setCurrentRoom(const std::string& name);

	virtual void update(float dt, const sf::Vector2f& mousePos) override;
};