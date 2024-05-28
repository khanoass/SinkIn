#include <SFML/Graphics/RenderWindow.hpp>

#include "Game.hpp"
#include "Camera.hpp"
#include "ResManager.hpp"

int main()
{
	const sf::Vector2u size = { 1200, 800 };

	sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Sink In", sf::Style::Titlebar | sf::Style::Close);
	window.setMouseCursorVisible(false);

	ResManager res;

	Game game({ (float)size.x / 2, (float)size.y / 2 }, &res);
	GUI gui(game.getMapPtr(), &res);
	
	Camera camera(size, 0.9f), guiCamera(size);

	sf::Event event;
	sf::Clock clock, fpsClock;
	sf::Time currentTime, lastTime;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			game.updateEvent(event);
		}

		// Update
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		sf::Vector2f posf = { (float)pos.x, (float)pos.y };
		float dt = clock.restart().asSeconds();

		// FPS
		currentTime = fpsClock.getElapsedTime();
		float fps = 1.f / (currentTime.asSeconds() - lastTime.asSeconds());
		gui.setFPS((int)fps);
		lastTime = currentTime;

		camera.updatePlayerSmooth(dt, game.getPlayerPosition(), game.getMapPtr()->changedRoom());
		game.update(dt, posf);

		gui.setAmmo(game.getPlayerAmmo());
		gui.update(dt, posf);

		sf::Vector2f viPos = camera.playerSmoothCenter(dt, game.getPlayerPosition(), game.getMapPtr()->changedRoom());
		sf::Vector2i screenPosition = window.mapCoordsToPixel(viPos, camera.view());
		game.setVignettePosition({ (float)screenPosition.x, (float)screenPosition.y });

		window.clear();

		// Game
		window.setView(camera.view());
		window.draw(game);

		// Gui
		window.setView(guiCamera.view());
		window.draw(gui);

		window.display();
	}

	return 0;
}