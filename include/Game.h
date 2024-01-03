#pragma once

#include "SFML/Graphics.hpp"

#include "EntityManager.h"

class Game
{
	sf::RenderWindow m_window;
	EntityManager    m_entities;
	bool             m_running      = true;
	size_t           m_currentFrame = 0;

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
