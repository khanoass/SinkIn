#include "Heart.h"
#include "Player.h"

void Heart::picked(const std::shared_ptr<Player>& player)
{
	if (alive())
	{
		if (player != nullptr)
			player->health(amount);
	}
}
