#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Widget.hpp"

class BoostCounter : public Widget
{
private:
	// Const
	const sf::Vector2f _position = { 10, 300 };
	const sf::Vector2f _size = { 254, 60 };
	const sf::Vector2f _padding = { 5, 5 };

	// Cosmetic
	sf::VertexArray _panel;
	const sf::Color _bg = { 80, 80, 80, 100 };
	const sf::Color _fg = { 200, 200, 200, 100 };
	bool _boosted = false;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_panel);
	}

public:
	BoostCounter()
	{
		_panel.setPrimitiveType(sf::Quads);
		_panel.append(sf::Vertex(_position, _bg));
		_panel.append(sf::Vertex({ _position.x + _size.x, _position.y }, _bg));
		_panel.append(sf::Vertex({ _position.x + _size.x, _position.y + _size.y }, _bg));
		_panel.append(sf::Vertex({ _position.x, _position.y + _size.y }, _bg));
		_panel.append(sf::Vertex({ _position.x + _padding.x, _position.y + _padding.y }, _fg));
		_panel.append(sf::Vertex({ _position.x + _padding.x, _position.y + _padding.y }, _fg));
		_panel.append(sf::Vertex({ _position.x + _padding.x, _position.y + _size.y - _padding.y }, _fg));
		_panel.append(sf::Vertex({ _position.x + _padding.x, _position.y + _size.y - _padding.y }, _fg));
	}

	void setTime(float seconds, float max)
	{
		_boosted = seconds > 0.f;
		_panel[5].position.x = _position.x + _padding.x + _boosted * (_size.x - (seconds / max)*_size.x);
		_panel[6].position.x = _position.x + _padding.x + _boosted * (_size.x - (seconds / max)*_size.x);
	}

	virtual void update(float dt, const sf::Vector2f& mousePos) override
	{
	}
};