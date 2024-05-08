#include "Boost.h"
#include "Player.h"

void Boost::picked(Player* player)
{
	if (alive())
	{
		if (player != nullptr)
			player->boost();
	}
}
