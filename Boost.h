#pragma once

#include "Item.hpp"

class Boost : public Item
{
protected:
	virtual void picked(Player* _player) override;

public:
	Boost(const sf::Vector2f& position, const sf::Texture& texture)
		: Item(position, {32, 32}, texture, 20.f)
	{
	}
};