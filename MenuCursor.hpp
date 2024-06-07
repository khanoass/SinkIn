#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Mouse.hpp>
#include "ResManager.hpp"

class MenuCursor : public sf::Drawable
{
private:
	// Data
	sf::Vector2f _size;

	// Cosmetic
	sf::Sprite _sprite;
	sf::Texture* _tex;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_sprite, states);
	}

public:
	MenuCursor(ResManager* res)
	{
		_size = { 32, 32 };
		_sprite.setOrigin({ _size.x / 2, _size.y / 2 });
		sf::Vector2i pos = sf::Mouse::getPosition();
		_sprite.setPosition((float)pos.x, (float)pos.y);
		_sprite.setScale(3.f, 3.f);

		_tex = &res->textures.cursor_menu;
		_sprite.setTexture(*_tex);
	}

	void update(const sf::Vector2f& mousePos)
	{
		_sprite.setPosition(mousePos);
	}
};