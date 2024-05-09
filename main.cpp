#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/View.hpp>

#include "Game.hpp"

int main()
{
	const sf::Vector2u size = { 1200, 800 };

	// Smooth camera
	const float camFactor = 1.75f;

	sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Sink In", sf::Style::Titlebar | sf::Style::Close);
	window.setMouseCursorVisible(false);
	
	Game game({ (float)size.x / 2, (float)size.y / 2 });
	GUI gui(game.getMapPtr());

	sf::Event event;
	sf::Clock clock;

	sf::View view(sf::FloatRect(0, 0, (float)size.x, (float)size.y));
	view.zoom(0.75f);
	sf::View guiView(sf::FloatRect(0, 0, (float)size.x, (float)size.y));

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

		// Smooth camera TODO: Encapsulation
		sf::Vector2f playerPos = game.getPlayerPosition();
		sf::Vector2f movement = playerPos - view.getCenter();
		if (vm::norm(movement) < 0.2f)
			view.setCenter(playerPos);
		else if (game.getMapPtr()->changedRoom())
			view.setCenter(playerPos); // TODO: Change to match direction
		else
			view.setCenter(view.getCenter() + (movement * dt * camFactor));
			

		window.clear();

		// Game
		window.setView(view);
		window.draw(game);

		// Gui
		window.setView(guiView);
		window.draw(gui);

		window.display();
	}

	return 0;
}