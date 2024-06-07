#pragma once

#include "Item.hpp"

class Key : public Item
{
protected:
	virtual void picked(const std::shared_ptr<Player>& _player) override;

public:
	Key(const sf::Vector2f& position, ResManager* res)
		: Item(position, { 32, 32 }, & res->textures.book)
	{}
};