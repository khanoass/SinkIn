#include "Player.h"
#include "Map.h"
#include "Boost.h"
#include "Pistol.hpp"

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*_current, states);
}

sf::Vector2i Map::getNextRoomOrigin(const sf::Vector2i& pixel, Direction direction) const
{
	int h = 0, v = 0;
	Directions::getHV(h, v, direction);
	sf::Vector2i next = { pixel.x + h * 3, pixel.y + v * 3 };
	return next;
}

bool Map::loadMapFromImage(const sf::Image& image, const sf::Vector2f& center)
{
	// Data
	std::stack<sf::Vector2i> stack;

	const int doorsNb = 4;
	const sf::Vector2i doorsOffset[doorsNb] = { { 1, 0 },{ 2, 1 },{ 1, 2 },{ 0, 1 } };

	// Doors
	std::vector<sf::Vector2i> pixels;
	std::map<sf::Vector2i, std::vector<Direction>, vm::Vector2iComparator> pixelDoor;

	// Routing pixel
	sf::Color rounting = image.getPixel(0, 0);
	sf::Vector2i origin = { (int)rounting.r, (int)rounting.g };
	stack.push(origin);
	_pixelRoom.push_back(origin);

	pixelDoor[origin] = std::vector<Direction>();

	Logger::log({ "First pass" });
	int counter = 0;

	// First pass, door recon
	while (!stack.empty())
	{
		// Get start pixel
		sf::Vector2i startPx = stack.top();
		stack.pop();

		// Room name
		std::stringstream name;

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
					_pixelRoom.push_back(origin);
				}
			}
		}

		// Insert room, pixel and directions
		auto it = pixelDoor.find(startPx);
		if (it != pixelDoor.end())
			it->second = doors;
		else
			Logger::log({ "- Didn't find pixelDoor at ", std::to_string(startPx.x), ",", std::to_string(startPx.y) });

		name << counter << Logger::afterNumber(counter) << " Room";
		_rooms.push_back(std::make_shared<Room>(Room(_shaderTex, { 1000, 600 }, center, startPx, name.str())));
		_items->addRoom(name.str());

		Logger::log({ "+ Added pixelDoor at ", std::to_string(startPx.x), ",", std::to_string(startPx.y), " and added ", name.str() });

		pixels.push_back(startPx);
		counter++;
	}

	// Adding the room pointers to the map, 
	// done sepately to ensure the correct pointer being passed
	for (int i = 0; i < _rooms.size(); i++)
	{
		sf::Vector2i px = pixels[i];
		_pixelRoomMap[px] = _rooms[i];
	}

	Logger::log({ "Second pass" });

	// Second pass, next rooms
	for (int i = 0; i < _rooms.size(); i++)
	{
		std::shared_ptr<Room> room = _rooms[i];

		sf::Vector2i ip = pixels[i];
		std::vector<std::shared_ptr<Room>> nextRooms;
		std::vector<Direction> dir = pixelDoor[ip];

		for (const auto& d : dir)
		{
			// Add direction + room
			sf::Vector2i no = getNextRoomOrigin(ip, d);
			std::shared_ptr<Room> next = _pixelRoomMap[no];
			nextRooms.push_back(next);
		}

		_rooms[i]->setNextRooms(pixelDoor.at(ip), nextRooms);

		Logger::log({ "Added next room pointers at ", std::to_string(ip.x), ",", std::to_string(ip.y) });

		// Items

		// 1st level
		if (i == 0)
		{
			// Boost
			_items->add(std::make_shared<Boost>(Boost({ center.x, center.y - 100 }, _res)), room->name());

			// Pistol
			_items->add(std::make_shared<Pistol>(Pistol({ center.x, center.y + 100 }, _res)), room->name());
		}

		// Rest
		else
		{
			// RNG
			int nbBoosts = (i != 0) ? Random::iRand(0, 8) : 1;
			if (nbBoosts > 5) nbBoosts = 1;
			int r1 = Random::iRand(0, 100);
			int r2 = Random::iRand(0, 100);
			int nbPistols = (r1 < 85) ? 0 : 1;
			int nbSMG = (r2 < 95) ? 0 : 1;

			// Boosts
			for (int i = 0; i < nbBoosts; i++)
			{
				sf::Vector2f pos = { 0, 0 };
				do
					pos = { (float)Random::iRand(150, 1050), (float)Random::iRand(150, 650) };
				while (vm::dist(pos, center) < 200.f);
				_items->add(std::make_shared<Boost>(Boost(pos, _res)), room->name());
			}
			
			// Pistols
			for (int i = 0; i < nbPistols; i++)
			{
				sf::Vector2f pos = { 0, 0 };
				do
					pos = { (float)Random::iRand(150, 1050), (float)Random::iRand(150, 650) };
				while (vm::dist(pos, center) < 200.f);
				_items->add(std::make_shared<Pistol>(Pistol(pos, _res)), room->name());
			}

			// SMG
			for (int i = 0; i < nbSMG; i++)
			{
				// TODO
			}
		}
		_rooms[i]->setItems(_items);
	}

	Logger::log({ "Finished loading map, found ", std::to_string(_rooms.size()), " rooms." });
	return true;
}

Map::Map(const std::string& filename, const sf::Vector2f& center, ResManager* res)
	: _current(nullptr), _player(nullptr), _shaderTex(&res->shaders.map), _res(res), _items(nullptr)
{
	_filename = filename;
	_center = center;
}

void Map::setPlayer(const std::shared_ptr<Player>& player)
{
	_player = player;
}

bool Map::generate()
{
	sf::Image buf;
	buf.loadFromFile(_filename);
	if (!loadMapFromImage(buf, _center))
		return false;
	_current = _rooms[0];
	_items->setCurrentRoom(_current->name());
	sf::Vector2u size = buf.getSize();
	_textureSize = { (int)size.x, (int)size.y };
	Logger::log({ "Spawned in ", _current->name() });
	return true;
}

std::shared_ptr<Room> Map::currentRoom() const
{
	return _current;
}

std::shared_ptr<Room> Map::atPixel(const sf::Vector2i& px)
{
	return _pixelRoomMap[px];
}

std::vector<sf::Vector2i> Map::pixelRooms() const
{
	return _pixelRoom;
}

void Map::setItems(const std::shared_ptr<Items>& items)
{
	_items = items;
}

std::shared_ptr<Items> Map::items()
{
	if (_items == nullptr) return nullptr;
	return _items;
}

void Map::exitRoom(Direction door)
{
	auto old = _current;
	_current = _current->nextRoom(door);
	_changedRoom = true;
	_items->setCurrentRoom(_current->name());
	_items->changeItemRoom(_player->activeWeapon(), old->name(), _current->name());
}

sf::Vector2f Map::getPlayerPosition() const
{
	return _player->position();
}

bool Map::changedRoom() const
{
	return _changedRoom;
}

std::shared_ptr<sf::Shader> Map::getTexShaderPtr()
{
	return _shaderTex;
}

void Map::resetChangedRoom()
{
	_changedRoom = false;
}

void Map::updateEvent(const sf::Event& event)
{
	for (const auto& r : _rooms)
		r->updateEvent(event);
}

void Map::update(float dt, const sf::Vector2f& mousePos)
{
	for (const auto& r : _rooms)
		r->update(dt, mousePos);
}

sf::Vector2i Map::textureSize() const
{
	return _textureSize;
}