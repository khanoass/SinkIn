#include <SFML/Graphics/RenderWindow.hpp>

#include "Game.hpp"
#include "Camera.hpp"
#include "ResManager.hpp"
#include "GUI.hpp"

int main()
{
	const sf::Vector2u size = { 1600, 800 };
	sf::Uint8 style = sf::Style::Titlebar | sf::Style::Close;

	//const sf::Vector2u size = { 1920, 1080 };
	//sf::Uint8 style = sf::Style::Fullscreen | sf::Style::Close;

	sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Sink In", style);
	window.setMouseCursorVisible(false);

	ResManager res;

	Game game({ (float)size.x / 2, (float)size.y / 2 }, &res);
	GUI gui(game.map(), &res);
	
	Camera camera(size, 0.9f), guiCamera(size);

	sf::Event event;
	sf::Clock clock, fpsClock;
	sf::Time currentTime, lastTime;

	while (window.isOpen())
	{
		// Update
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		sf::Vector2f posf = { (float)pos.x, (float)pos.y };
		float dt = clock.restart().asSeconds();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
				window.close();
			game.updateEvent(event, dt, posf);
		}

		// FPS
		currentTime = fpsClock.getElapsedTime();
		float fps = 1.f / (currentTime.asSeconds() - lastTime.asSeconds());
		lastTime = currentTime;

		camera.updatePlayerSmooth(dt, game.getPlayerScreenPosition(), game.map()->changedRoom());
		game.update(dt, posf);

		// GUI
		gui.setFPS((int)fps);
		if (game.playerHasWeapon())	gui.setAmmo(game.getPlayerAmmo());
		else						gui.setAmmo(-1);
		gui.setBoostTime(game.getPlayerBoostTime(), game.getPlayerMaxBoostTime());
		gui.setHP(game.getPlayerHP(), game.getPlayerMaxHP());
		gui.update(dt, posf);

		sf::Vector2f viPos = camera.playerSmoothCenter(dt, game.getPlayerScreenPosition(), game.map()->changedRoom());
		sf::Vector2i screenPosition = window.mapCoordsToPixel(viPos, camera.view());
		game.setVignettePosition({ (float)screenPosition.x, (float)screenPosition.y });

		window.clear();

		// Game
		window.setView(camera.view());
		window.draw(game);

		// Gui
		if (game.state() == Game::Play)
		{
			window.setView(guiCamera.view());
			window.draw(gui);
		}

		window.display();
	}

	return 0;
}