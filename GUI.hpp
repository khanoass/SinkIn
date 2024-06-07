#pragma once

#include "Minimap.hpp"
#include "FpsCounter.hpp"
#include "AmmoCounter.hpp"
#include "BoostCounter.hpp"
#include "HPcounter.hpp"
#include "KeyCounter.hpp"
#include "ResManager.hpp"

class GUI : public sf::Drawable
{
private:
	// Data
	std::vector<Widget*> _widgets;
	std::shared_ptr<Map> _map;

	// Widgets
	Minimap _minimap;
	FpsCounter _fps;
	AmmoCounter _ammo;
	BoostCounter _boost;
	HPCounter _hp;
	KeyCounter _key;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (const auto& w : _widgets)
		{
			if(w->visible())
				target.draw(*w);
		}
	}

public:
	GUI(const std::shared_ptr<Map>& map, ResManager* res) :
		_map(map), _minimap(_map)
	{
		// Minimap
		_minimap.setRooms(_map->pixelRooms(), _map->textureSize());
		_minimap.setActiveRoom(_map->currentRoom()->pixel());
		_widgets.push_back(&_minimap);

		// FPS
		_fps.setFont(&res->fonts.font);
		_widgets.push_back(&_fps);

		// Ammo
		_ammo.setFont(&res->fonts.font);
		_ammo.setTexture(&res->textures.bullets);
		_widgets.push_back(&_ammo);
		_ammo.setVisibility(false);

		// Boost
		_widgets.push_back(&_boost);

		// HP
		_widgets.push_back(&_hp);

		// Keys
		_key.setFont(&res->fonts.font);
		_key.setTexture(&res->textures.book);
		_widgets.push_back(&_key);
	}

	void setFPS(int fps)
	{
		_fps.setFPS(fps);
	}

	void setAmmo(int ammo)
	{
		_ammo.setAmmo(ammo);
		_ammo.setVisibility(ammo >= 0);
	}

	void setKeys(int keys, int max)
	{
		_key.setKeys(keys, max);
	}

	void setBoostTime(float seconds, float max)
	{
		_boost.setTime(seconds, max);
	}

	void setHP(float hp, float max)
	{
		_hp.setHP(hp, max);
	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
		for (const auto& w : _widgets)
			w->update(dt, mousePos);
	}
};