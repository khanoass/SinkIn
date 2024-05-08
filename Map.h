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

	sf::Texture _boostTexture;

	Player* _player;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2i getNextRoomOrigin(const sf::Vector2i& pixel, Direction direction) const;

	int iRand(int min, int max);

	bool loadMapFromImage(const sf::Image& image, const sf::Vector2f& center);

public:
	Map(const std::string& filename, const sf::Vector2f& center);

	void setPlayer(Player* player);

	bool generate();

	Room* currentRoom() const;

	void exitRoom(Direction door);

	virtual void updateEvent(const sf::Event& event);

	virtual void update(float dt, const sf::Vector2f& mousePos);
};