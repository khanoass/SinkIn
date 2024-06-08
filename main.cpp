#include <SFML/Graphics/RenderWindow.hpp>

#include "Game.hpp"
#include "ResManager.hpp"

int main()
{
	const sf::Vector2u size = { 1200, 800 };
	sf::Uint8 style = sf::Style::Titlebar | sf::Style::Close;

	//const sf::Vector2u size = { 1920, 1080 };
	//sf::Uint8 style = sf::Style::Fullscreen | sf::Style::Close;

	sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Sink In", style);
	window.setMouseCursorVisible(false);

	ResManager res;
	Game game({ (float)size.x / 2, (float)size.y / 2 }, &res);

	sf::Event event;
	sf::Clock clock, fpsClock;
	sf::Time currentTime, lastTime;

	Game::State state = Game::Start;

	while (window.isOpen())
	{
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
		state = game.state();

		// Updating
		if (state == Game::Start)
		{
			if (game.exit()) 
				window.close();
		}
		else if (state == Game::Play)
		{
			sf::Vector2f viPos = game.getPlayerScreenPosition();
			sf::Vector2i screenPosition = window.mapCoordsToPixel(viPos, game.view());
			game.setVignettePosition({ (float)screenPosition.x, (float)screenPosition.y });
		}
		else if (state == Game::Exit)
		{
			window.close();
		}

		window.clear(sf::Color::Transparent);

		if (state == Game::Play)
		{
			window.setView(game.view());
			window.draw(game);
			window.setView(game.guiView());
			window.draw(game.gui());
		}
		else if (state == Game::Story)
		{
			window.setView(game.guiView());
			window.draw(game);
		}
		else if (state == Game::PausePlay)
		{
			window.setView(game.view());
			window.draw(game);
			window.setView(game.guiView());
			window.draw(game.pauseScreen());
		}
		else if (state == Game::PauseStory)
		{
			window.draw(game);
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