#pragma once

#include <string>
#include "Cursor.hpp"

class Map;
class Player;

class Level : public LiveEntity, public std::enable_shared_from_this<Level>
{
private:
	// Data
	sf::Vector2f _screenCenter;
	std::string _mapFilename;
	ResManager* _res;

	std::shared_ptr<Map> _map;
	std::shared_ptr<Cursor> _cursor;
	std::shared_ptr<Player> _player;
	std::shared_ptr<Items> _items;
	std::shared_ptr<Enemies> _enemies;
	std::shared_ptr<Bullets> _bullets;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	Level(const std::string& mapfilename, const sf::Vector2f& screenCenter, ResManager* res);

	void init();
	void start();
	void restart();

	std::shared_ptr<Player> player();
	std::shared_ptr<Map> map();

	void updateEvent(const sf::Event& event, float dt, const sf::Vector2f& mousePos);

	virtual void update(float dt, const sf::Vector2f& mousePos);
};