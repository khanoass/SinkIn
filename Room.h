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
#include "Items.hpp"

class Enemies;

class Room : public sf::Drawable
{
private:
	const sf::Vector2f _doorSize = { 100, 100 };

	bool _justEntered = true;

	// Data
	std::vector<Direction> _doors;
	std::map<Direction, std::shared_ptr<Room>> _nextRooms;
	sf::Vector2f _center, _screenCenter;
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

	std::shared_ptr<Player> _player;

	// Items
	std::shared_ptr<Items> _items;

	// Enemies
	std::shared_ptr<Enemies> _enemies;

	// Bullets
	std::shared_ptr<Bullets> _bullets;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void initDoorFromDirection(sf::RectangleShape& shape, Direction dir);

public:
	Room(std::shared_ptr<sf::Shader> texShader, const sf::Vector2f& size, const sf::Vector2f& screenCenter, const sf::Vector2i& pixel, const std::string& name = "");

	void setContents(const std::shared_ptr<Player>& player, const std::shared_ptr<Items>& items, const std::shared_ptr<Enemies>& enemies, const std::shared_ptr<Bullets>& bullets);

	sf::Vector2i pixel() const;
	std::vector<Direction> doors() const;

	virtual void update(float dt, const sf::Vector2f& mousePos);

	void setNextRooms(const std::vector<Direction>& doors, const std::vector<std::shared_ptr<Room>>& next);

	bool pointInRoom(const sf::Vector2f& point);
	bool pointInDoor(const sf::Vector2f& point, Direction& direction);
	bool pointInDoor(const sf::Vector2f& point);

	sf::Vector2f center() const;
	sf::Vector2f screenCenter() const;
	sf::Vector2f absoluteOffset() const;
	sf::Vector2f size() const;
	std::string name() const;
	bool justEntered() const;
	
	sf::Vector2f doorBorder(Direction dir) const;
	Direction doorDirectionFromPoint(const sf::Vector2f& point);
	sf::Vector2f doorBorderX(Direction dir);
	sf::Vector2f doorBorderY(Direction dir);

	sf::Vector2f spawn(Direction dir) const;
	void exit();
	std::shared_ptr<Room> nextRoom(Direction dir);
};