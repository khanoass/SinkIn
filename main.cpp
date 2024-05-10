#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/View.hpp>

#include "Game.hpp"
#include "Camera.hpp"

int main()
{
	const sf::Vector2u size = { 1200, 800 };

	sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Sink In", sf::Style::Titlebar | sf::Style::Close);
	window.setMouseCursorVisible(false);
	
	Game game({ (float)size.x / 2, (float)size.y / 2 });
	GUI gui(game.getMapPtr());

	Camera camera(size), guiCamera(size);

	sf::Event event;
	sf::Clock clock;

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
		game.update(dt, posf);
		gui.update(dt, posf);
		camera.updatePlayerSmooth(dt, game.getPlayerPosition(), game.getMapPtr()->changedRoom());

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