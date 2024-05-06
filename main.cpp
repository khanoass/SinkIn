#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include "Game.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Sink In", sf::Style::Titlebar | sf::Style::Close);
	sf::Clock clock;
	Game game;
	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			game.updateEvent(event);
		}

		game.update(clock.restart().asSeconds());

		window.clear();
		window.draw(game);
		window.display();
	}

	return 0;
}