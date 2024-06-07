#pragma once

#include "MenuCursor.hpp"
#include "Button.hpp"

class StartScreen : public sf::Drawable
{
private:
	const sf::Vector2f _logoYBounds = { 250, 350 };
	const float _offsetStart = 100.f;
	const float _offsetExit = 300.f;
	const sf::Vector2f _size = { 350.f, 150.f };
	const sf::Color _top = { 150, 150, 150 };
	const sf::Color _bot = { 30, 30, 30 };

	// Data
	sf::Vector2f _center;
	Button _start;
	Button _exit;
	sf::Sprite _logo;
	sf::VertexArray _bg;
	sf::Clock _logoMotionClock;
	MenuCursor _cursor;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_bg, states);
		target.draw(_logo, states);
		target.draw(_start, states);
		target.draw(_exit, states);
		target.draw(_cursor, states);
	}

public:
	StartScreen(const sf::Vector2f& center, ResManager* res) :
		_start({ center.x, center.y + _offsetStart }, _size, "Start Game", res),
		_exit({ center.x, center.y + _offsetExit }, _size, "Quit to Desktop", res),
		_cursor(res)
	{
		_center = center;

		_bg.setPrimitiveType(sf::Quads);
		_bg.append(sf::Vertex({ 0, 0 }, _top));
		_bg.append(sf::Vertex({ _center.x * 2, 0 }, _top));
		_bg.append(sf::Vertex({ _center.x * 2, _center.y * 2 }, _bot));
		_bg.append(sf::Vertex({ 0, _center.y * 2 }, _bot));

		_logo.setTexture(res->textures.logo);
		auto size = res->textures.logo.getSize();
		_logo.setOrigin((float)size.x / 2, (float)size.y / 2);
	}

	bool started()
	{
		bool c = _start.clicked();
		if (c)
		{
			_start.reset();
			_start.changeString("Continue Game");
		}
		return c;
	}

	bool exited()
	{
		bool c = _exit.clicked();
		if (c) _exit.reset();
		return c;
	}

	void updateEvent(const sf::Event& event)
	{
		_start.updateEvent(event);
		_exit.updateEvent(event);
	}

	void update(const sf::Vector2f& mousePos)
	{
		_start.update(mousePos);
		_exit.update(mousePos);
		_cursor.update(mousePos);

		// Logo move up and down
		float t = _logoMotionClock.getElapsedTime().asSeconds();
		float min = _logoYBounds.x;
		float max = _logoYBounds.y;
		float y = min + 0.5 * (max - min) * (1 + std::sin(t));
		_logo.setPosition({	_center.x, y });
	}
};