#include "Boost.h"
#include "Player.h"

void Boost::picked(const std::shared_ptr<Player>& player)
{
	if (alive())
	{
		if (player != nullptr)
			player->boost();
	}
}
