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

	ResManager res;
	Game game({ (float)size.x / 2, (float)size.y / 2 }, &res);

	sf::Event event;
	sf::Clock clock, fpsClock;
	sf::Time currentTime, lastTime;

	while (window.isOpen())
	{
		Game::State state = game.state();

		// Update
		sf::Vector2f posf = (state == Game::Play)
			? window.mapPixelToCoords(sf::Mouse::getPosition(window), game.view())
			: window.mapPixelToCoords(sf::Mouse::getPosition(window));

		float dt = clock.restart().asSeconds();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			game.updateEvent(event, dt, posf);
		}

		// FPS
		currentTime = fpsClock.getElapsedTime();
		float fps = 1.f / (currentTime.asSeconds() - lastTime.asSeconds());
		lastTime = currentTime;

		game.update(dt, posf, (int)fps);

		// Check for exit
		if (state == Game::Start)
		{
			if (game.exit()) 
				window.close();
		}

		if (state == Game::Play)
		{
			sf::Vector2f viPos = game.getPlayerScreenPosition();
			sf::Vector2i screenPosition = window.mapCoordsToPixel(viPos, game.view());
			game.setVignettePosition({ (float)screenPosition.x, (float)screenPosition.y });
		}

		window.clear(sf::Color::Black);

		if (state == Game::Play)
		{
			window.setView(game.view());
			window.draw(game);
			window.setView(game.guiView());
			window.draw(game.gui());
		}
		else if (state == Game::Pause)
		{
			window.setView(game.view());
			window.draw(game);
			window.setView(game.guiView());
			window.draw(game.pauseScreen());
		}
		else
		{
			window.draw(game);
		}

		window.display();
	}

	return 0;
}