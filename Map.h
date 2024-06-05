#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <sstream>

#include "VeMa.hpp"
#include "Room.h"
#include "Logger.hpp"
#include "ResManager.hpp"
#include "Items.hpp"
#include "Bullets.hpp"
#include "Boost.h"
#include "Pistol.hpp"
#include "Shotgun.hpp"
#include "SMG.hpp"
#include "Shadow.hpp"

class Player;
class Enemy;

class Map : public LiveEntity
{
private:
	const float _spawnBigMargin = 100.f;
	const float _spawnSmallMargin = 20.f;

	// Data
	std::string _filename;
	sf::Vector2f _screenCenter;
	ResManager* _res;

	std::shared_ptr<Items> _items;
	std::shared_ptr<Enemies> _enemies;
	std::shared_ptr<Bullets> _bullets;

	std::shared_ptr<Room> _current;
	std::vector<std::shared_ptr<Room>> _rooms;
	std::vector<sf::Vector2i> _pixelRoom;
	std::map<sf::Vector2i, std::shared_ptr<Room>, vm::Vector2iComparator> _pixelRoomMap;

	sf::Vector2i _textureSize;

	std::shared_ptr<Player> _player;
	std::shared_ptr<sf::Shader> _shaderTex;

	bool _changedRoom = false;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2i getNextRoomOrigin(const sf::Vector2i& pixel, Direction direction) const;

	bool loadMapFromImage(const sf::Image& image, const sf::Vector2f& screenCenter);

	sf::Vector2f getRandomPositionInRoom(const sf::Vector2f& pos, const sf::Vector2f& roomCenter, const sf::Vector2f& roomSize, float margin);

	void updateBulletBounds();

public:
	Map(const std::string& filename, const sf::Vector2f& center, ResManager* res);

	void setPlayer(const std::shared_ptr<Player>& player);

	bool generate();

	std::shared_ptr<Room> currentRoom() const;

	void setContents(const std::shared_ptr<Items>& items, const std::shared_ptr<Enemies>& enemies, const std::shared_ptr<Bullets>& bullets);
	std::shared_ptr<Items> items();
	std::shared_ptr<Enemies> enemies();
	std::shared_ptr<Bullets> bullets();

	sf::Vector2f screenCenter() const;

	sf::Vector2f getPlayerPosition() const;

	std::shared_ptr<Room> atPixel(const sf::Vector2i& px);

	std::shared_ptr<sf::Shader> getTexShaderPtr();

	sf::Vector2i textureSize() const;

	std::vector<sf::Vector2i> pixelRooms() const;

	void exitRoom(Direction door);

	void resetChangedRoom();

	bool changedRoom() const;

	virtual void update(float dt, const sf::Vector2f& mousePos) override;
};