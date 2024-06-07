#pragma once

#include "MenuCursor.hpp"
#include "Button.hpp"

class GameOverScreen : public sf::Drawable
{
private:
	const float _offsetImg = -200.f;
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
	GameOverScreen(const sf::Vector2f& center, ResManager* res) :
		_start({ center.x, center.y + _offsetStart }, _size, "Retry", res),
		_exit({ center.x, center.y + _offsetExit }, _size, "Ragequit", res),
		_cursor(res)
	{
		_center = center;

		_bg.setPrimitiveType(sf::Quads);
		_bg.append(sf::Vertex({ 0, 0 }, _top));
		_bg.append(sf::Vertex({ _center.x * 2, 0 }, _top));
		_bg.append(sf::Vertex({ _center.x * 2, _center.y * 2 }, _bot));
		_bg.append(sf::Vertex({ 0, _center.y * 2 }, _bot));

		_logo.setTexture(res->textures.gameover);
		auto size = res->textures.gameover.getSize();
		_logo.setOrigin((float)size.x / 2, (float)size.y / 2);
		_logo.setPosition({ _center.x, _center.y + _offsetImg });
	}

	bool started()
	{
		bool c = _start.clicked();
		if (c) _start.reset();
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
	}
};