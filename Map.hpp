#pragma once

#include <iostream>
#include <string>
#include <random>
#include <stack>
#include <sstream>
#include "VeMa.hpp"
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

	sf::Vector2i getNextRoomOrigin(const sf::Vector2i& pixel, Direction direction) const
	{
		int h = 0, v = 0;
		Directions::getHV(h, v, direction);
		sf::Vector2i next = { pixel.x + h * 3, pixel.y + v * 3 };
		return next;
	}

	int iRand(int min, int max)
	{
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(min, max);
		return uni(rng);
	}

	void loadMapFromImage(const sf::Image& image, const sf::Vector2f& center)
	{
		// Data
		std::stack<sf::Vector2i> stack;

		const int doorsNb = 4;
		const sf::Vector2i doorsOffset[doorsNb] = { { 1, 0 },{ 2, 1 },{ 1, 2 },{ 0, 1 } };

		// Doors
		std::vector<sf::Vector2i> pixels;
		std::map<sf::Vector2i, std::vector<Direction>, vm::Vector2iComparator> pixelDoor;
		std::map<sf::Vector2i, Room*, vm::Vector2iComparator> roomIp;
		
		// Routing pixel
		sf::Color rounting = image.getPixel(0, 0);
		sf::Vector2i origin = { (int)rounting.r, (int)rounting.g };
		stack.push(origin);

		pixelDoor[origin] = std::vector<Direction>();

		std::cout << "First pass\n";

		int counter = 0;
		// First pass, door recon
		while (!stack.empty())
		{
			// Get start pixel
			sf::Vector2i startPx = stack.top();
			stack.pop();

			// Doors
			sf::Color doorsc[doorsNb];
			for (int i = 0; i < doorsNb; i++)
				doorsc[i] = image.getPixel(startPx.x + doorsOffset[i].x, startPx.y + doorsOffset[i].y);

			std::vector<Direction> doors;
			for (int i = 0; i < doorsNb; i++)
			{
				// Add door only if pixel is black
				if (doorsc[i] == sf::Color::Black)
				{
					Direction dir = (Direction)i;
					doors.push_back(dir);

					// Calculate door pixel
					sf::Vector2i pixel = { startPx.x + doorsOffset[i].x, startPx.y + doorsOffset[i].y };
					sf::Vector2i origin = getNextRoomOrigin(startPx, dir);

					// Add to the stack only if door not already contained in map
					auto it = pixelDoor.find(origin);
					if (it == pixelDoor.end())
					{
						pixelDoor[origin] = std::vector<Direction>();
						stack.push({ origin.x, origin.y });
					}
				}
			}

			// Insert room, pixel and directions
			auto it = pixelDoor.find(startPx);
			if (it != pixelDoor.end())
			{
				it->second = doors;
				std::cout << "+ Added pixelDoor at " << startPx.x << "," << startPx.y;
			}
			else
			{
				std::cout << "- Didn't find pixelDoor at " << startPx.x << "," << startPx.y << "";
			}

			std::stringstream ss;
			ss << counter << "st Room";
			_rooms.push_back(Room({ 50, 50, 50 }, { 1000, 600 }, center, ss.str()));

			std::cout << " and Added room " << counter << "\n";

			int k = _rooms.size() - 1;

			pixels.push_back(startPx);

			counter++;
		}

		// Adding the room pointers to the map, 
		// done sepately to ensure the correct pointer being passed
		for (int i = 0; i < _rooms.size(); i++)
		{
			sf::Vector2i px = pixels[i];
			roomIp[px] = &_rooms[i];
		}

		std::cout << "Second pass\n";

		// Second pass, next rooms
		for (int i = 0; i < _rooms.size(); i++)
		{
			sf::Vector2i ip = pixels[i];
			std::vector<Room*> nextRooms;
			std::vector<Direction> dir = pixelDoor[ip];

			for (const auto& d : dir)
			{
				// Add direction + room
				sf::Vector2i no = getNextRoomOrigin(ip, d);
				Room* next = roomIp[no];
				nextRooms.push_back(next);
			}

			_rooms[i].setNextRooms(pixelDoor.at(ip), nextRooms);

			std::cout << "Added next room pointers at " << ip.x << "," << ip.y << "\n";

			//////////////////////////////////////////////////////////////////////////////////////////////
/*
			_rooms.push_back(Room({ 50, 50, 50 }, { 1000, 600 }, center, "Center room"));

			_rooms[0].setNextRooms({ Up, Right, Down, Left }, { &_rooms[1], &_rooms[2], &_rooms[3], &_rooms[4] });
*/
			////////////////////////////////////////////////////////////////////////////////////////////////

			// Items
			int nbBoosts = (i != 0) ? iRand(0, 8) : 1;
			if (nbBoosts > 5) nbBoosts = 1;
			std::vector<std::shared_ptr<Item>> items;
			for (int i = 0; i < nbBoosts; i++)
			{
				sf::Vector2f pos = { 0, 0 };
				do
				{
					pos = { (float)iRand(150, 1050), (float)iRand(150, 650) };
				} while (vm::dist(pos, center) < 200.f);
				items.push_back(std::make_shared<Boost>(Boost(pos, _boostTexture)));
			}
			_rooms[i].setItems(items);
		}

		// 50 rooms
		std::cout << "Finished loading map, found " << _rooms.size() << " rooms.\n";
	}

public:
	Map(const std::string& filename, const sf::Vector2f& center)
		: _current(nullptr)
	{
		_boostTexture.loadFromFile("assets/textures/item.png");
		sf::Image buf;
		buf.loadFromFile(filename);
		loadMapFromImage(buf, center);
		_current = &_rooms[0];

		std::cout << "Spawned in " << _current->name() << "\n";
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