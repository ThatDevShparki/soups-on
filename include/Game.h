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
	bool             m_renderGrid   = false;
	size_t           m_currentFrame = 0;
	Vec2             m_tileSize     = { 32.0f, 32.0f };
	float            m_scale        = 2.0f;
	sf::Text         m_gridText;

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

	std::shared_ptr<Entity> player() const;

	[[nodiscard]] const Vec2& gridSize() const
	{
		return m_tileSize * m_scale;
	}

	void drawLine(const Vec2& a, const Vec2& b)
	{
		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(a.x, a.y)),
			sf::Vertex(sf::Vector2f(b.x, b.y))
		};
		m_window.draw(line, 2, sf::Lines);
	}
};
