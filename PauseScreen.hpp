#pragma once

#include "Button.hpp"
#include <SFML/Graphics/Sprite.hpp>

class PauseScreen : public sf::Drawable
{
private:
	const float _offsetStart = -150.f;
	const float _offsetExit = 150.f;
	const sf::Vector2f _size = { 350.f, 150.f };
	const sf::Color _bgCol = { 100, 100, 100, 100 };

	// Data
	Button _resume;
	Button _exit;
	sf::VertexArray _bg;
	MenuCursor _cursor;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_bg, states);
		target.draw(_resume, states);
		target.draw(_exit, states);
		target.draw(_cursor, states);
	}

public:
	PauseScreen(const sf::Vector2f& center, ResManager* res) :
		_resume({ center.x, center.y + _offsetStart }, _size, "Resume Game", res),
		_exit({ center.x, center.y + _offsetExit }, _size, "Quit to Menu", res),
		_cursor(res)
	{
		_bg.setPrimitiveType(sf::Quads);
		_bg.append(sf::Vertex({ 0, 0 }, _bgCol));
		_bg.append(sf::Vertex({ center.x * 2, 0 }, _bgCol));
		_bg.append(sf::Vertex({ center.x * 2, center.y * 2 }, _bgCol));
		_bg.append(sf::Vertex({ 0, center.y * 2 }, _bgCol));
	}

	bool resumed()
	{
		bool c = _resume.clicked();
		if (c) _resume.reset();
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
		_resume.updateEvent(event);
		_exit.updateEvent(event);
	}

	void update(const sf::Vector2f& mousePos)
	{
		_resume.update(mousePos);
		_exit.update(mousePos);
		_cursor.update(mousePos);
	}
};