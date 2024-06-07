#include "Key.h"
#include "Player.h"

void Key::picked(const std::shared_ptr<Player>& player)
{
	if (alive())
	{
		if (player != nullptr)
			player->key();
	}
}
