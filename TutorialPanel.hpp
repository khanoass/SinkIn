#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Widget.hpp"

class TutorialPanel : public Widget
{
private:
	// Const
	const float _outMargin = 50.f;
	const int _charSize = 42;

	// Cosmetic
	sf::Vector2f _center;
	sf::Text _text;
	sf::Font* _font = nullptr;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_text);
	}

public:
	TutorialPanel(const sf::Vector2f& center, const std::string& msg)
	{
		_center = center;

		_text.setCharacterSize(_charSize);
		_text.setFillColor(sf::Color(255, 255, 255));
		_text.setString(msg);
	}

	void setFont(sf::Font* font)
	{
		_font = font;
		_text.setFont(*_font);
		auto gb = _text.getGlobalBounds();
		_text.setPosition({ _center.x - gb.width / 2, _center.y * 2 - gb.height - _outMargin });
	}
};