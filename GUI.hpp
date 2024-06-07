#pragma once

#include "Minimap.hpp"
#include "FpsCounter.hpp"
#include "AmmoCounter.hpp"
#include "BoostCounter.hpp"
#include "HPcounter.hpp"
#include "KeyCounter.hpp"
#include "TutorialPanel.hpp"
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

	// Tutorial
	TutorialPanel _wasd, _gun, _enemy, _book;
	int _stage = -1;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (const auto& w : _widgets)
		{
			if(w->visible())
				target.draw(*w);
		}
	}

public:
	GUI(const std::shared_ptr<Map>& map, const sf::Vector2f& center, ResManager* res) :
		_map(map),
		_fps({ center.x * 2, center.y * 2 }),
		_key({ center.x * 2, center.y * 2 }),
		_wasd(center, "[WASD] to move"),
		_gun(center, "[Left click] to shoot\n[Right click] to drop"),
		_enemy(center, "Beware of creatures\nThey are usually friendly, but you never know"),
		_book(center, "Collect all books to finish a level")
	{
		// Minimap
		_minimap.generate(_map);
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

		// Tutorial
		_wasd.setFont(&res->fonts.font);
		_wasd.setVisibility(false);
		_gun.setFont(&res->fonts.font);
		_wasd.setVisibility(false);
		_enemy.setFont(&res->fonts.font);
		_wasd.setVisibility(false);
		_book.setFont(&res->fonts.font);
		_wasd.setVisibility(false);
		_widgets.push_back(&_wasd);
		_widgets.push_back(&_gun);
		_widgets.push_back(&_enemy);
		_widgets.push_back(&_book);
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

	void setTutorialStage(int stage)
	{
		_stage = stage;
		_wasd.setVisibility(false);
		_gun.setVisibility(false);
		_enemy.setVisibility(false);
		_book.setVisibility(false);

		if (_stage > -1)
		{
			switch (_stage)
			{
			case 0: _wasd.setVisibility(true); break;
			case 1: _gun.setVisibility(true); break;
			case 2: _enemy.setVisibility(true); break;
			case 3: _book.setVisibility(true); break;
			}
		}
	}

	void resetMinimap(const std::shared_ptr<Map>& map)
	{
		_map = map;
		_minimap.generate(_map);
		_minimap.setActiveRoom(_map->currentRoom()->pixel());
	}

	virtual void update(float dt, const sf::Vector2f& mousePos)
	{
		for (const auto& w : _widgets)
			w->update(dt, mousePos);
	}
};