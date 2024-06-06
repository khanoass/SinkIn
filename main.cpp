#include <SFML/Graphics/RenderWindow.hpp>

#include "Game.hpp"
#include "ResManager.hpp"

int main()
{
	//const sf::Vector2u size = { 1200, 800 };
	//sf::Uint8 style = sf::Style::Titlebar | sf::Style::Close;

	const sf::Vector2u size = { 1920, 1080 };
	sf::Uint8 style = sf::Style::Fullscreen | sf::Style::Close;

	sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Sink In", style);
	window.setMouseCursorVisible(false);

	ResManager res;

	Game game({ (float)size.x / 2, (float)size.y / 2 }, &res);

	sf::Event event;
	sf::Clock clock, fpsClock;
	sf::Time currentTime, lastTime;

	while (window.isOpen())
	{
		// Update
		sf::Vector2f posf = window.mapPixelToCoords(sf::Mouse::getPosition(window), game.view());
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

		game.update(dt, posf, (int)fps);

		sf::Vector2f viPos = game.getPlayerScreenPosition();
		sf::Vector2i screenPosition = window.mapCoordsToPixel(viPos, game.view());
		game.setVignettePosition({ (float)screenPosition.x, (float)screenPosition.y });

		window.clear(sf::Color::Black);

		// Game
		if(game.state() == Game::Play)
			window.setView(game.view());
		window.draw(game);

		if (game.state() == Game::Play)
		{
			window.setView(game.guiView());
			window.draw(game.gui());
		}

		window.display();
	}

	return 0;
}