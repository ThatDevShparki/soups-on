#pragma once

#include "SFML/Graphics.hpp"

#include "Vec2.h"
#include "Scene.h"

#include <string>
#include <map>

class Scene;

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class Game
{
	sf::RenderWindow m_window;
	bool             m_running = true;
	SceneMap         m_scenes;
	std::string      m_currentScene;


	void init(const std::string& path);
	void update();

	void sUserInput();

public:

	Game();
	explicit Game(const std::string& manifestPath);

	void quit();
	void run();

	sf::RenderWindow& window();
	bool isRunning() const;

	void drawLine(const Vec2& a, const Vec2& b)
	{
		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(a.x, a.y)),
			sf::Vertex(sf::Vector2f(b.x, b.y))
		};
		m_window.draw(line, 2, sf::Lines);
	}


	// scene management
	[[nodiscard]] std::shared_ptr<Scene> currentScene() const;
	void changeScene(const std::string& tag);
	void changeScene(const std::string& tag, std::shared_ptr<Scene> scene);

};
