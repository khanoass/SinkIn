#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Widget.hpp"

class FpsCounter : public Widget
{
private:
	// Const
	const sf::Vector2f _position = { 1150, 10 };
	const sf::Vector2f _size = { 40, 48 };
	const sf::Vector2f _margin = { 8, 12 };
	const int _charSize = 24;

	// Cosmetic
	sf::VertexArray _panel;
	sf::Text _text;
	sf::Font* _font;
	const sf::Color _bg = { 80, 80, 80, 100 };

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_panel);
		target.draw(_text);
	}

public:
	FpsCounter()
	{
		_panel.setPrimitiveType(sf::Quads);
		_panel.append(sf::Vertex(_position, _bg));
		_panel.append(sf::Vertex({ _position.x + _size.x, _position.y }, _bg));
		_panel.append(sf::Vertex({ _position.x + _size.x, _position.y + _size.y }, _bg));
		_panel.append(sf::Vertex({ _position.x, _position.y + _size.y }, _bg));

		_text.setCharacterSize(_charSize);
		_text.setFillColor(sf::Color(255, 255, 255, 128));
		_text.setPosition({ _position.x + _margin.x, _position.y + _margin.y });
	}

	void setFont(sf::Font* font)
	{
		_font = font;
		_text.setFont(*_font);
	}

	void setFPS(int fps)
	{
		_text.setString(std::to_string(fps));
	}

	virtual void update(float dt, const sf::Vector2f& mousePos) override
	{
		
	}
};