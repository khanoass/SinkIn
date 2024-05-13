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
			cursor_yes, cursor_no, cursor_door,
			player,
			boost,
			attack,
			eph,
			glock_ground,
			glock_hold;
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
		textures.player.loadFromFile("assets/textures/placeholder_player.png");
		textures.attack.loadFromFile("assets/textures/attack.png");
		textures.cursor_no.loadFromFile("assets/textures/f2.png");
		textures.cursor_yes.loadFromFile("assets/textures/f1.png");
		textures.cursor_door.loadFromFile("assets/textures/f3.png");
		textures.eph.loadFromFile("assets/textures/eph.png");
		textures.boost.loadFromFile("assets/textures/item.png");

		fonts.font.loadFromFile("assets/fonts/Sunny Spells Basic.ttf");

		shaders.map.loadFromFile("assets/shaders/room.vert", "assets/shaders/room.frag");
	}
};