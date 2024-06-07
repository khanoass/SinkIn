#pragma once

#include "MenuCursor.hpp"
#include "Button.hpp"

class CreditScreen : public sf::Drawable
{
private:
	const float _logoOffset = -350.f;
	const float _offsetExit = 250.f;
	const sf::Vector2f _size = { 350.f, 150.f };
	const sf::Color _top = { 150, 150, 150 };
	const sf::Color _bot = { 30, 30, 30 };

	// Data
	sf::Vector2f _center;
	Button _exit;
	sf::Sprite _logo;
	sf::VertexArray _bg;
	MenuCursor _cursor;
	sf::Text _text;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_bg, states);
		target.draw(_logo, states);
		target.draw(_exit, states);
		target.draw(_text);
		target.draw(_cursor, states);
	}

public:
	CreditScreen(const sf::Vector2f& center, ResManager* res) :
		_exit({ center.x, center.y + _offsetExit }, _size, "Exit game", res),
		_cursor(res)
	{
		_center = center;

		_bg.setPrimitiveType(sf::Quads);
		_bg.append(sf::Vertex({ 0, 0 }, _top));
		_bg.append(sf::Vertex({ _center.x * 2, 0 }, _top));
		_bg.append(sf::Vertex({ _center.x * 2, _center.y * 2 }, _bot));
		_bg.append(sf::Vertex({ 0, _center.y * 2 }, _bot));

		_text.setFont(res->fonts.retro);
		_text.setCharacterSize(42.f);
		_text.setString("Made by Rafael Félix - 2024");
		_text.setFillColor(sf::Color::Black);
		auto tsize = _text.getGlobalBounds();
		_text.setPosition({ _center.x - tsize.width / 2, _center.y - tsize.height / 2 });

		_logo.setTexture(res->textures.logo);
		auto size = res->textures.logo.getSize();
		_logo.setOrigin((float)size.x / 2, (float)size.y / 2);
		_logo.setPosition({ _center.x, _center.y + _logoOffset });
	}

	bool exited()
	{
		bool c = _exit.clicked();
		if (c) _exit.reset();
		return c;
	}

	void updateEvent(const sf::Event& event)
	{
		_exit.updateEvent(event);
	}

	void update(const sf::Vector2f& mousePos)
	{
		_exit.update(mousePos);
		_cursor.update(mousePos);
	}
};