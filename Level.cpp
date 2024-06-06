#include "Player.h"
#include "Map.h"
#include "Level.h"

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*_map, states);
	target.draw(*_cursor, states);
}

Level::Level(const std::string& mapfilename, const sf::Vector2f& screenCenter, ResManager* res)
{
	_screenCenter = screenCenter;
	_mapFilename = mapfilename;
	_res = res;
	init();
}

void Level::init()
{
	// Init all level's stuff
	_map = std::make_shared<Map>(_mapFilename, _screenCenter, _res);
	_player = std::make_shared<Player>(_res);
	_cursor = std::make_shared<Cursor>(_player, _res);
	_items = std::make_shared<Items>();
	_bullets = std::make_shared<Bullets>();
	_enemies = std::make_shared<Enemies>(_player, _bullets, _items);
}

void Level::start()
{
	// Init map
	_map->setPlayer(_player);
	_map->setContents(_items, _enemies, _bullets);
	_map->generate();

	// Pass pointers to other pointers
	_player->setMap(_map);
	_player->setLevel(shared_from_this());
	_enemies->setMap(_map);
	_cursor->setMap(_map);
	_cursor->setEnemies(_enemies);
}

void Level::restart()
{
	init();
	start();
}

std::shared_ptr<Player> Level::player()
{
	return _player;
}

std::shared_ptr<Map> Level::map()
{
	return _map;
}

void Level::updateEvent(const sf::Event& event, float dt, const sf::Vector2f& mousePos)
{
	_player->updateEvent(event, dt, mousePos);
}

void Level::update(float dt, const sf::Vector2f& mousePos)
{
	_map->update(dt, mousePos);
	_items->update(dt, mousePos);
	_enemies->update(dt, mousePos);
	_bullets->update(dt, mousePos);
	_cursor->update(dt, mousePos);
	_player->update(dt, mousePos);
}