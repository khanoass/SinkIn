#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <sstream>

#include "Widget.hpp"

class KeyCounter : public Widget
{
private:
	// Const
	const sf::Vector2f _position = { 10, 990 };
	const sf::Vector2f _size = { 150, 80 };
	const sf::Vector2f _margin = { 80, 18 };
	const sf::Vector2f _texOffset = { 10, 10 };
	const sf::Vector2f _texSize = { 32, 32 };
	const float _texScale = 2.f;
	const int _charSize = 32;
	const float _rightMargin = 10.f;

	// Cosmetic
	sf::VertexArray _panel;
	sf::VertexArray _texPanel;
	sf::Texture* _tex = nullptr;
	sf::Text _text;
	sf::Font* _font = nullptr;
	const sf::Color _bg = { 80, 80, 80, 100 };

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_panel);
		target.draw(_texPanel, _tex);
		target.draw(_text);
	}

public:
	KeyCounter()
	{
		_panel.setPrimitiveType(sf::Quads);
		_panel.append(sf::Vertex(_position, _bg));
		_panel.append(sf::Vertex({ _position.x + _size.x, _position.y }, _bg));
		_panel.append(sf::Vertex({ _position.x + _size.x, _position.y + _size.y }, _bg));
		_panel.append(sf::Vertex({ _position.x, _position.y + _size.y }, _bg));

		_texPanel.setPrimitiveType(sf::Quads);
		_texPanel.append(sf::Vertex({ _position.x + _texOffset.x, _position.y + _texOffset.y }, { 0, 0 }));
		_texPanel.append(sf::Vertex({ _position.x + (_texSize.x * _texScale) + _texOffset.x, _position.y + _texOffset.y }, { _texSize.x, 0 }));
		_texPanel.append(sf::Vertex({ _position.x + (_texSize.x * _texScale) + _texOffset.x, _position.y + (_texSize.y * _texScale) + _texOffset.y }, { _texSize.x, _texSize.y }));
		_texPanel.append(sf::Vertex({ _position.x + _texOffset.x, _position.y + (_texSize.y * _texScale) + _texOffset.y }, { 0, _texSize.y }));

		_text.setCharacterSize(_charSize);
		_text.setFillColor(sf::Color(255, 255, 255, 128));
		_text.setPosition({ _position.x + _margin.x, _position.y + _margin.y });
	}

	void setFont(sf::Font* font)
	{
		_font = font;
		_text.setFont(*_font);
	}

	void setTexture(sf::Texture* tex)
	{
		_tex = tex;
	}

	void setKeys(int keys, int max)
	{
		std::stringstream ss;
		ss << keys << " / " << max;
		_text.setString(ss.str());
		auto gb = _text.getGlobalBounds();
		_panel[1].position.x = gb.width + _margin.x + _texOffset.x * 2 + _rightMargin;
		_panel[2].position.x = gb.width + _margin.x + _texOffset.x * 2 + _rightMargin;
	}
};