#pragma once

#include <iostream>
#include <string>
#include <random>
#include <stack>
#include <sstream>

#include "VeMa.hpp"
#include "Room.hpp"
#include "Logger.hpp"

class Player;
class Boost;

class Map : public Entity
{
private:
	// Data
	std::string _filename;
	sf::Vector2f _center;

	Room* _current;
	std::vector<Room> _rooms;
	std::vector<sf::Vector2i> _pixelRoom;
	std::map<sf::Vector2i, Room*, vm::Vector2iComparator> _pixelRoomMap;

	sf::Vector2i _textureSize;

	sf::Texture _boostTexture;

	Player* _player;

	bool _changedRoom = false;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2i getNextRoomOrigin(const sf::Vector2i& pixel, Direction direction) const;

	int iRand(int min, int max);

	bool loadMapFromImage(const sf::Image& image, const sf::Vector2f& center);

public:
	Map(const std::string& filename, const sf::Vector2f& center);

	void setPlayer(Player* player);

	bool generate();

	Room* currentRoom() const;

	Room* atPixel(const sf::Vector2i& px);

	sf::Vector2i textureSize() const;

	std::vector<sf::Vector2i> pixelRooms() const;

	void exitRoom(Direction door);

	void resetChangedRoom();

	bool changedRoom() const;
};