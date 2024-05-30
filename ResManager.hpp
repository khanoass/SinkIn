#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>

class ResManager
{
public:
	struct Textures
	{
		sf::Texture
			cursor_yes, cursor_no, cursor_door, cursor_weapon, cursor_ontarget,
			player,
			boost,
			muzzle,
			eph,
			bullets,
			
			// Weapons
			glock_ground, glock_hold,
			shotgun_ground, shotgun_hold,
			smg_ground, smg_hold;
	};

	struct Fonts
	{
		sf::Font
			font;
	};

	struct Shaders
	{
		sf::Shader
			map;
	};

	Textures textures;
	Fonts fonts;
	Shaders shaders;

	ResManager()
	{
		textures.glock_ground.loadFromFile("assets/textures/glock_ground_2.png");
		textures.glock_hold.loadFromFile("assets/textures/glock_hold_2.png");
		textures.shotgun_ground.loadFromFile("assets/textures/shotgun_ground.png");
		textures.shotgun_hold.loadFromFile("assets/textures/shotgun_hold.png");
		textures.smg_ground.loadFromFile("assets/textures/smg_ground.png");
		textures.smg_hold.loadFromFile("assets/textures/smg_hold.png");
		textures.player.loadFromFile("assets/textures/placeholder_player_2.png");
		textures.muzzle.loadFromFile("assets/textures/muzzle.png");
		textures.cursor_no.loadFromFile("assets/textures/f2.png");
		textures.cursor_yes.loadFromFile("assets/textures/f1.png");
		textures.cursor_door.loadFromFile("assets/textures/f3.png");
		textures.cursor_weapon.loadFromFile("assets/textures/f4.png");
		textures.cursor_ontarget.loadFromFile("assets/textures/f5.png");
		textures.eph.loadFromFile("assets/textures/eph.png");
		textures.boost.loadFromFile("assets/textures/item.png");
		textures.bullets.loadFromFile("assets/textures/bullets.png");

		fonts.font.loadFromFile("assets/fonts/Sunny Spells Basic.ttf");

		shaders.map.loadFromFile("assets/shaders/room.vert", "assets/shaders/room.frag");
	}
};