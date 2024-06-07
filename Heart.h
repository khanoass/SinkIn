#pragma once

#include "Item.hpp"

class Heart : public Item
{
private:
	const float amount = 15.f;

protected:
	virtual void picked(const std::shared_ptr<Player>& _player) override;

public:
	Heart(const sf::Vector2f& position, ResManager* res)
		: Item(position, { 32, 32 }, & res->textures.heart)
	{}
};