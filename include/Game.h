#pragma once

#include "SFML/Graphics.hpp"

#include "AssetManager.h"
#include "EntityManager.h"

class Game
{
	sf::RenderWindow m_window;
	EntityManager    m_entities;
	AssetManager     m_assets;
	bool             m_running      = true;
	size_t           m_currentFrame = 0;

	void init(const std::string& path);
	void update();

	void sRender();
	void sUserInput();

public:

	Game();
	explicit Game(const std::string& path);

	void quit();
	void run();

	const sf::RenderWindow& window() const;
	bool isRunning() const;
};
