#pragma once

#include <vector>
#include <map>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "Direction.hpp"
#include "LiveEntity.hpp"
#include "Items.hpp"

class Room : public LiveEntity
{
private:
	// Data
	std::vector<Direction> _doors;
	std::map<Direction, Room*> _nextRooms;
	sf::Vector2f _center;
	sf::Vector2f _size;
	std::string _name;
	sf::Vector2i _pixel;
	std::shared_ptr<sf::RenderTexture> _buffer;
	std::shared_ptr<sf::Shader> _shaderTexture;
	
	// Cosmetic
	sf::Color _bg;
	sf::RectangleShape _shape;
	std::vector<sf::RectangleShape> _doorsShape;
	sf::VertexArray _canvas;

	// Items
	Items* _items;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		_shaderTexture->setUniform("texture", sf::Shader::CurrentTexture);

		sf::RenderStates finalState = states;
		finalState.texture = &_buffer->getTexture();
		finalState.shader = _shaderTexture.get();

		target.draw(_canvas, finalState);
	}

	void initDoorFromDirection(sf::RectangleShape& shape, Direction dir, const sf::Vector2f& center)
	{
		shape.setFillColor(_bg);

		const sf::Vector2f shapeSize = { 100, 100 };

		shape.setSize(shapeSize);
		shape.setOrigin({ shapeSize.x / 2, shapeSize.y / 2 });
		shape.setPosition(center);

		int h = 0, v = 0;
		Directions::getHV(h, v, dir);
		shape.move({ h * (_size.x / 2 + shapeSize.x / 2), v * (_size.y / 2 + (shapeSize.y / 2)) });
	}

public:
	Room(sf::Shader* texShader, const sf::Vector2f& size, const sf::Vector2f& center, const sf::Vector2i& pixel, const std::string& name = "")
		: _shaderTexture(texShader), _size(size), _center(center), _name(name), _pixel(pixel)
	{
		_bg = { 50, 50, 50 };
		_shape.setFillColor(_bg);
		_shape.setSize(_size);
		_shape.setOrigin({ _size.x / 2, _size.y / 2 });
		_shape.setPosition(_center);

		_buffer = std::make_shared<sf::RenderTexture>();
		_buffer->create((unsigned int)_center.x * 2, (unsigned int)_center.y * 2);

		_canvas.setPrimitiveType(sf::Quads);
		_canvas.append(sf::Vertex({ 0, 0 },							{ 0, 0 }));
		_canvas.append(sf::Vertex({ _center.x * 2, 0 },				{ _center.x * 2, 0 }));
		_canvas.append(sf::Vertex({ _center.x * 2, _center.y * 2 }, { _center.x * 2, _center.y * 2 }));
		_canvas.append(sf::Vertex({ 0, _center.y * 2 },				{ 0, _center.y * 2 }));
	}

	void setItems(Items* items)
	{
		_items = items;
	}

	sf::Vector2i pixel() const
	{
		return _pixel;
	}

	std::vector<Direction> doors() const
	{
		return _doors;
	}

	virtual void updateEvent(const sf::Event& event)
	{

	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
		_buffer->clear();

		sf::RenderStates bufferstates;
		bufferstates.blendMode = sf::BlendAlpha;

		_buffer->draw(_shape, bufferstates);
		for (const auto& ds : _doorsShape)
			_buffer->draw(ds, bufferstates);
		_buffer->draw(*_items, bufferstates);

		_buffer->display();
	}

	void setNextRooms(const std::vector<Direction>& doors, const std::vector<Room*>& next)
	{
		_doors = doors;
		for (int i = 0; i < _doors.size(); i++)
		{
			Direction& d = _doors[i];
			if (d != None)
			{
				sf::RectangleShape shape;
				initDoorFromDirection(shape, d, _center);
				_doorsShape.push_back(shape);
				_nextRooms[d] = next[i];
			}
		}
	}

	bool pointInRoom(const sf::Vector2f& point)
	{
		return _shape.getGlobalBounds().contains(point);
	}

	bool pointInDoor(const sf::Vector2f& point, Direction& direction)
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

	bool pointInDoor(const sf::Vector2f& point)
	{
		Direction dir = None;
		return pointInDoor(point, dir);
	}

	sf::Vector2f center() const
	{
		return _center;
	}

	sf::Vector2f size() const
	{
		return _size;
	}

	std::string name() const
	{
		return _name;
	}

	sf::Vector2f doorBorder(Direction dir) const
	{
		sf::Vector2f offset = _center;

		int h = 0, v = 0;
		Directions::getHV(h, v, dir);
		
		offset.x -= h * (_size.x / 2);
		offset.y -= v * (_size.y / 2);

		return offset;
	}

	sf::Vector2f spawn(Direction dir) const
	{
		sf::Vector2f border = doorBorder(dir);

		int h = 0, v = 0;
		Directions::getHV(h, v, dir);

		border.x += h * 50;
		border.y += v * 50;

		return border;
	}

	Room* nextRoom(Direction dir)
	{
		return _nextRooms[dir];
	}
};