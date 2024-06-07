#pragma once

#include "Button.hpp"
#include <SFML/Graphics/Sprite.hpp>

class StartScreen : public sf::Drawable
{
private:
	const float _offsetLogo = -200.f;
	const float _offsetStart = 100.f;
	const float _offsetExit = 300.f;
	const sf::Vector2f _size = { 350.f, 150.f };
	const sf::Color _top = { 100, 100, 100 };
	const sf::Color _bot = { 0, 0, 0 };

	// Data
	Button _start;
	Button _exit;
	sf::Sprite _logo;
	sf::VertexArray _bg;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_bg);
		target.draw(_logo, states);
		target.draw(_start, states);
		target.draw(_exit, states);
	}

public:
	StartScreen(const sf::Vector2f& center, ResManager* res) : 
		_start({ center.x, center.y + _offsetStart }, _size, "Start Game", res), 
		_exit({ center.x, center.y + _offsetExit }, _size, "Quit to Desktop", res)
	{
		_bg.setPrimitiveType(sf::Quads);
		_bg.append(sf::Vertex({ 0, 0 }, _top));
		_bg.append(sf::Vertex({ center.x * 2, 0 }, _top));
		_bg.append(sf::Vertex({ center.x * 2, center.y * 2 }, _bot));
		_bg.append(sf::Vertex({ 0, center.y * 2 }, _bot));

		//_logo.setTexture(res->textures.logo);
		//auto size = res->textures.logo.getSize();
		//_logo.setOrigin(size / 2, size / 2);
		_logo.setPosition({ center.x, center.y + _offsetLogo });
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
	}
};