#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Widget.hpp"

class AmmoCounter : public Widget
{
private:
	// Const
	const sf::Vector2f _position = { 10, 710 };
	const sf::Vector2f _size = { 150, 80 };
	const sf::Vector2f _margin = { 80, 18 };
	const sf::Vector2f _texOffset = { 10, 10 };
	const sf::Vector2f _texSize = { 32, 32 };
	const float _texScale = 2.f;
	const int _charSize = 32;

	// Cosmetic
	sf::VertexArray _panel;
	sf::VertexArray _texPanel;
	sf::Texture* _tex;
	sf::Text _text;
	sf::Font* _font;
	const sf::Color _bg = { 80, 80, 80, 100 };

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_panel);
		target.draw(_texPanel, _tex);
		target.draw(_text);
	}

public:
	AmmoCounter()
	{
		_panel.setPrimitiveType(sf::Quads);
		_panel.append(sf::Vertex(_position, _bg));
		_panel.append(sf::Vertex({ _position.x + _size.x, _position.y }, _bg));
		_panel.append(sf::Vertex({ _position.x + _size.x, _position.y + _size.y }, _bg));
		_panel.append(sf::Vertex({ _position.x, _position.y + _size.y }, _bg));

		_texPanel.setPrimitiveType(sf::Quads);
		_texPanel.append(sf::Vertex({ _position.x + _texOffset.x, _position.y + _texOffset.y }, { 0, 0 }));
		_texPanel.append(sf::Vertex({ _position.x + (_texSize.x*_texScale) + _texOffset.x, _position.y + _texOffset.y }, { _texSize.x, 0 }));
		_texPanel.append(sf::Vertex({ _position.x + (_texSize.x*_texScale) + _texOffset.x, _position.y + (_texSize.y * _texScale) + _texOffset.y }, { _texSize.x, _texSize.y } ));
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

	void setAmmo(int ammo)
	{
		_text.setString(std::to_string(ammo));
		auto gb = _text.getGlobalBounds();
		_panel[1].position.x = gb.width + _margin.x + _texOffset.x * 2;
		_panel[2].position.x = gb.width + _margin.x + _texOffset.x * 2;
	}

	virtual void update(float dt, const sf::Vector2f& mousePos) override
	{
	}
};