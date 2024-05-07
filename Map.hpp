#pragma once

#include <iostream>
#include <string>
#include "Room.hpp"

class Map : public Entity
{
private:
	// Data
	Room* _current;
	std::vector<Room> _rooms;

	sf::Texture _boostTexture;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(*_current);
	}

public:
	Map(const std::string& filename, const sf::Vector2f& center)
	{
		_rooms.push_back(Room({ 50, 50, 50 }, { 1000, 600 }, center, "Center room"));
		_rooms.push_back(Room({ 50, 50, 50 }, { 1000, 600 }, center, "Up room"));
		_rooms.push_back(Room({ 50, 50, 50 }, { 1000, 600 }, center, "Right room"));
		_rooms.push_back(Room({ 50, 50, 50 }, { 1000, 600 }, center, "Down room"));
		_rooms.push_back(Room({ 50, 50, 50 }, { 1000, 600 }, center, "Left room"));

		// Next rooms
		_rooms[0].setNextRooms({ Up, Right, Down, Left }, { &_rooms[1], &_rooms[2], &_rooms[3], &_rooms[4] });
		_rooms[1].setNextRooms({ Down }, { &_rooms[0] });
		_rooms[2].setNextRooms({ Left }, { &_rooms[0] });
		_rooms[3].setNextRooms({ Up },	  { &_rooms[0] });
		_rooms[4].setNextRooms({ Right },{ &_rooms[0] });

		// Items
		_boostTexture.loadFromFile("assets/textures/item.png");
		_rooms[0].setItems({
			std::make_shared<Boost>(Boost({ 500, 300 }, _boostTexture)),
			std::make_shared<Boost>(Boost({ 100, 200 }, _boostTexture)),
			std::make_shared<Boost>(Boost({ 200, 200 }, _boostTexture)),
			std::make_shared<Boost>(Boost({ 150, 500 }, _boostTexture))
		});

		_current = &_rooms[0];
	}

	Room* currentRoom() const
	{
		return _current;
	}

	void exitRoom(Direction door)
	{
		_current = _current->nextRoom(door);
	}

	virtual void updateEvent(const sf::Event& event)
	{

	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{

	}
};