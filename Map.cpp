#include "Player.h"
#include "Enemies.h"
#include "Map.h"

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

bool Map::loadMapFromImage(const sf::Image& image, const sf::Vector2f& screenCenter, int level, int mobHostility)
{
	// Data
	std::stack<sf::Vector2i> stack;

	const int doorsNb = 4;
	const sf::Vector2i doorsOffset[doorsNb] = { { 1, 0 },{ 2, 1 },{ 1, 2 },{ 0, 1 } };
	const sf::Vector2i sizeOffset = { 0, 0 };
	const sf::Vector2i enemyOffset = { 1, 1 };
	const sf::Vector2i keyOffset = { 0, 2 };

	// Doors
	std::vector<sf::Vector2i> pixels;
	std::map<sf::Vector2i, std::vector<Direction>, vm::Vector2iComparator> pixelDoor;

	// Routing pixel
	sf::Color rounting = image.getPixel(0, 0);
	sf::Vector2i origin = { (int)rounting.r, (int)rounting.g };
	stack.push(origin);
	_pixelRoom.push_back(origin);

	pixelDoor[origin] = std::vector<Direction>();
	int counter = 0;

	// Enemies
	std::vector<int> roomEnemies;

	// Keys
	std::vector<bool> roomKeys;

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

		// Room size from encoded image
		sf::Color sizeenc = image.getPixel(startPx.x + sizeOffset.x, startPx.y + sizeOffset.y);
		int x = 10 * sizeenc.r;
		int y = 10 * sizeenc.g;
		sf::Vector2f roomSize = { (float)x, (float)y };

		// Enemies amount from encoded image
		sf::Color enenc = image.getPixel(startPx.x + enemyOffset.x, startPx.y + enemyOffset.y);
		if (enenc == sf::Color::White)
			roomEnemies.push_back(0);
		else
		{
			int nb = enenc.r;
			roomEnemies.push_back(nb);
		}

		// Keys from encoded image
		sf::Color kenc = image.getPixel(startPx.x + keyOffset.x, startPx.y + keyOffset.y);
		if (kenc == sf::Color::White)
			roomKeys.push_back(false);
		else
		{
			roomKeys.push_back(true);
			_keys++;
		}

		_rooms.push_back(std::make_shared<Room>(Room(_shaderTex, roomSize, screenCenter, startPx, name.str())));

		_items->addRoom(name.str());
		_enemies->addRoom(name.str());

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

		// Encoded population
		sf::Vector2f pos = { 0, 0 };

		// Enemies
		int hp = 100;
		if (level == 0)
			hp = 1;

		for (int j = 0; j < roomEnemies[i]; j++)
			_enemies->add(std::make_shared<Shadow>(getRandomPositionInRoom(pos, room->center(), room->size(), _spawnBigMargin), hp, mobHostility, _res), room);

		// Keys
		if (roomKeys[i])
			_items->add(std::make_shared<Key>(getRandomPositionInRoom(pos, room->center(), room->size(), _spawnSmallMargin), _res), room->name());

		// Not first room or tutorial
		if (i > 0 && level > 0)
		{
			// RNG
			int nbBoosts = Random::iRand(0, 8);
			if (nbBoosts > 5) nbBoosts = 1;
			int r1 = Random::iRand(0, 100);
			int r2 = Random::iRand(0, 100);
			int r3 = Random::iRand(0, 100);
			int r4 = Random::iRand(0, 100);
			int nbPistols = (r1 < 75) ? 0 : 1;
			if (nbPistols == 1 && r1 > 95) nbPistols = 2;
			int nbSMG = (r2 < 80) ? 0 : 1;
			int nbShotgun = (r3 < 90) ? 0 : 1;
			int nbRifles = (r4 < 90) ? 0 : 1;
			if (level < 2 && r4 < 98) nbRifles = 0;

			// Items & weapons generation

			for (int j = 0; j < nbBoosts; j++)	_items->add(std::make_shared<Boost>(getRandomPositionInRoom(pos, room->center(), room->size(), _spawnBigMargin), _res), room->name());
			for (int j = 0; j < nbPistols; j++) _items->addWeapon(std::make_shared<Pistol>(getRandomPositionInRoom(pos, room->center(), room->size(), _spawnSmallMargin), _res), room->name(), _bullets);
			for (int j = 0; j < nbSMG; j++)		_items->addWeapon(std::make_shared<SMG>(getRandomPositionInRoom(pos, room->center(), room->size(), _spawnSmallMargin), _res), room->name(), _bullets);
			for (int j = 0; j < nbShotgun; j++)	_items->addWeapon(std::make_shared<Shotgun>(getRandomPositionInRoom(pos, room->center(), room->size(), _spawnSmallMargin), _res), room->name(), _bullets);
			for (int j = 0; j < nbRifles; j++)	_items->addWeapon(std::make_shared<Rifle>(getRandomPositionInRoom(pos, room->center(), room->size(), _spawnSmallMargin), _res), room->name(), _bullets);
		
			Logger::log({ room->name(), " has size ", std::to_string(room->size().x), ",", std::to_string(room->size().y) });
		}
		
		// Tutorial
		if (level == 0)
		{
			if(i == 1)
				_items->addWeapon(std::make_shared<Pistol>(room->center(), _res), room->name(), _bullets);
		}
		else if (i == 0)
			_items->addWeapon(std::make_shared<Pistol>(room->center(), _res), room->name(), _bullets);

		_rooms[i]->setContents(_player, _items, _enemies, _bullets);
	}

	Logger::log({ "Finished loading map, found ", std::to_string(_rooms.size()), " rooms." });
	return true;
}

sf::Vector2f Map::getRandomPositionInRoom(const sf::Vector2f& pos, const sf::Vector2f& roomCenter, const sf::Vector2f& roomSize, float margin)
{
	sf::Vector2f out = pos;
	
	out = {
	Random::fRand(roomCenter.x - roomSize.x / 2 + margin, roomCenter.x + roomSize.x / 2 - margin),
	Random::fRand(roomCenter.y - roomSize.y / 2 + margin, roomCenter.y + roomSize.y / 2 - margin)
	};

	return out;
}

void Map::updateBulletBounds()
{
	_bullets->setBounds(
		{ _current->center().x - _current->size().x / 2, _current->center().x + _current->size().x / 2 },
		{ _current->center().y - _current->size().y / 2, _current->center().y + _current->size().y / 2 }
	);
}

Map::Map(const std::string& filename, const sf::Vector2f& center, ResManager* res)
	: _current(nullptr), _player(nullptr), _shaderTex(res->shaders.map), _res(res), _items(nullptr)
{
	_filename = filename;
	_screenCenter = center;
}

void Map::setPlayer(const std::shared_ptr<Player>& player)
{
	_player = player;
}

bool Map::generate(int level, int mobHostility)
{
	sf::Image buf;
	buf.loadFromFile(_filename);
	if (!loadMapFromImage(buf, _screenCenter, level, mobHostility))
		return false;
	_current = _rooms[0];
	_items->setCurrentRoom(_current->name());
	_enemies->setCurrentRoom(_current->name());
	updateBulletBounds();
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

void Map::setContents(const std::shared_ptr<Items>& items, const std::shared_ptr<Enemies>& enemies, const std::shared_ptr<Bullets>& bullets)
{
	_items = items;
	_enemies = enemies;
	_bullets = bullets;
}

std::shared_ptr<Items> Map::items()
{
	return _items;
}

std::shared_ptr<Enemies> Map::enemies()
{
	return _enemies;
}

std::shared_ptr<Bullets> Map::bullets()
{
	return _bullets;
}

sf::Vector2f Map::screenCenter() const
{
	return _screenCenter;
}

void Map::exitRoom(Direction door)
{
	std::string oldname = _current->name();
	_current = _current->nextRoom(door);
	_changedRoom = true;
	_items->setCurrentRoom(_current->name());
	_enemies->setCurrentRoom(_current->name());
	//_enemies->resetPositionsForRoom(oldname);
	_items->changeWeaponRoom(_player->activeWeapon(), oldname, _current->name());
	_bullets->clearBullets();
	updateBulletBounds();
}

sf::Vector2f Map::getPlayerPosition() const
{
	return _player->position();
}

bool Map::changedRoom() const
{
	return _changedRoom;
}

int Map::keys()
{
	return _keys;
}

std::shared_ptr<sf::Shader> Map::getTexShaderPtr()
{
	return _shaderTex;
}

void Map::resetChangedRoom()
{
	_changedRoom = false;
}

void Map::update(float dt, const sf::Vector2f& mousePos)
{
	_current->update(dt, mousePos);
}

sf::Vector2i Map::textureSize() const
{
	return _textureSize;
}