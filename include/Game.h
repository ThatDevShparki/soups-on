#pragma once

#include "SFML/Graphics.hpp"

class Game
{
	sf::RenderWindow m_window;
	bool             m_running = true;

	void update();

	void sRender();
	void sUserInput();

public:

	Game();

	void quit();
	void run();

	const sf::RenderWindow& window() const;
	bool isRunning() const;
};
