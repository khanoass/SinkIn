#include "Enemies.h"
#include "Room.h"

void Room::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	_shaderTexture->setUniform("texture", sf::Shader::CurrentTexture);

	sf::RenderStates finalState = states;
	finalState.texture = &_buffer->getTexture();
	finalState.shader = _shaderTexture.get();

	target.draw(_canvas, finalState);
}

void Room::initDoorFromDirection(sf::RectangleShape& shape, Direction dir)
{
	shape.setFillColor(_bg);

	shape.setSize(_doorSize);
	shape.setOrigin({ _doorSize.x / 2, _doorSize.y / 2 });
	shape.setPosition(_center);

	int h = 0, v = 0;
	Directions::getHV(h, v, dir);
	shape.move({ h * (_size.x / 2 + _doorSize.x / 2), v * (_size.y / 2 + _doorSize.y / 2) });
}

Room::Room(std::shared_ptr<sf::Shader> texShader, const sf::Vector2f& size, const sf::Vector2f& center, const sf::Vector2i& pixel, const std::string& name)
	: _shaderTexture(texShader), _size(size), _center(center), _name(name), _pixel(pixel)
{
	sf::Vector2f ss = { _center.x * 2, _center.y * 2 };

	_buffer = std::make_shared<sf::RenderTexture>();
	_buffer->create((unsigned int)ss.x, (unsigned int)ss.y);

	_canvas.setPrimitiveType(sf::Quads);
	_canvas.append(sf::Vertex({ 0, 0 }, { 0, 0 }));
	_canvas.append(sf::Vertex({ ss.x, 0 }, { ss.x, 0 }));
	_canvas.append(sf::Vertex({ ss.x, ss.y }, { ss.x, ss.y }));
	_canvas.append(sf::Vertex({ 0, ss.y }, { 0, ss.y }));

	_bg = { 50, 50, 50 };
	_shape.setFillColor(_bg);
	_shape.setSize(_size);
	_shape.setOrigin({ _size.x / 2, _size.y / 2 });
	_shape.setPosition(_center);
	_shape.setOutlineColor(sf::Color::White);
	_shape.setOutlineThickness(4);
}

void Room::setContents(const std::shared_ptr<Items>& items, const std::shared_ptr<Enemies>& enemies, const std::shared_ptr<Bullets>& bullets)
{
	_items = items;
	_enemies = enemies;
	_bullets = bullets;
}

sf::Vector2i Room::pixel() const
{
	return _pixel;
}

std::vector<Direction> Room::doors() const
{
	return _doors;
}

void Room::update(float dt, const sf::Vector2f& mousePos)
{
	_buffer->clear();

	sf::RenderStates bufferstates;
	bufferstates.blendMode = sf::BlendAlpha;

	_buffer->draw(_shape, bufferstates);
	for (const auto& ds : _doorsShape)
		_buffer->draw(ds, bufferstates);
	_buffer->draw(*_items, bufferstates);
	_buffer->draw(*_bullets, bufferstates);
	_buffer->draw(*_enemies, bufferstates);

	_buffer->display();
}

void Room::setNextRooms(const std::vector<Direction>& doors, const std::vector<std::shared_ptr<Room>>& next)
{
	_doors = doors;
	for (int i = 0; i < _doors.size(); i++)
	{
		Direction& d = _doors[i];
		if (d != None)
		{
			sf::RectangleShape shape;
			initDoorFromDirection(shape, d);
			_doorsShape.push_back(shape);
			_nextRooms[d] = next[i];
		}
	}
}

bool Room::pointInRoom(const sf::Vector2f& point)
{
	return _shape.getGlobalBounds().contains(point);
}

bool Room::pointInDoor(const sf::Vector2f& point, Direction& direction)
{
	for (int i = 0; i < _doors.size(); i++)
	{
		if (_doorsShape[i].getGlobalBounds().contains(point))
		{
			direction = _doors[i];
			return true;
		}
	}
	return false;
}

bool Room::pointInDoor(const sf::Vector2f& point)
{
	Direction dir = None;
	return pointInDoor(point, dir);
}

sf::Vector2f Room::center() const
{
	return _center;
}

sf::Vector2f Room::size() const
{
	return _size;
}

std::string Room::name() const
{
	return _name;
}

sf::Vector2f Room::doorBorder(Direction dir) const
{
	sf::Vector2f offset = _center;

	int h = 0, v = 0;
	Directions::getHV(h, v, dir);

	offset.x -= h * (_size.x / 2);
	offset.y -= v * (_size.y / 2);

	return offset;
}

sf::Vector2f Room::spawn(Direction dir) const
{
	sf::Vector2f border = doorBorder(dir);

	int h = 0, v = 0;
	Directions::getHV(h, v, dir);

	border.x += h * 50;
	border.y += v * 50;

	return border;
}

std::shared_ptr<Room> Room::nextRoom(Direction dir)
{
	return _nextRooms[dir];
}