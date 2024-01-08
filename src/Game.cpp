#include "Game.h"

#include <fstream>
#include <iostream>

#include "SceneLevel.h"


Game::Game() = default;

Game::Game(const std::string& path)
{
	init(path);
}


void Game::init(const std::string& manifestPath)
{
	// setup game window
	m_window.create(
		sf::VideoMode::getDesktopMode(),
		"Soups On"
	);
	m_window.setFramerateLimit(60);

	// register scene
	changeScene("level", std::make_shared<SceneLevel>(this, manifestPath));

}


void Game::run()
{
	while (isRunning())
	{
		update();
	}
}

void Game::update()
{
	m_window.clear(sf::Color(88, 181, 167));


	sUserInput();

	currentScene()->update();

	m_window.display();
}

void Game::sUserInput()
{
	sf::Event event{};
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			quit();
		}

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				quit();
			}
		}

		if (event.type == sf::Event::KeyPressed ||
			event.type == sf::Event::KeyReleased)
		{
			if (!currentScene()->handlesAction(
				ActionSource::keyboard,
				event.key.code
			))
			{
				continue;
			}

			const ActionKind kind = (event.type == sf::Event::KeyPressed)
									? ActionKind::pressed : ActionKind::released;

			currentScene()->doAction(
				currentScene()->getActionFor(
					ActionSource::keyboard,
					event.key.code
				),
				kind
			);
		}
	}
}


void Game::quit()
{
	m_running = false;
}

sf::RenderWindow& Game::window()
{
	return m_window;
}

bool Game::isRunning() const
{
	return m_window.isOpen() && m_running;
}


/* scene management */


std::shared_ptr<Scene> Game::currentScene() const
{
	return m_scenes.at(m_currentScene);
}

void Game::changeScene(const std::string& tag)
{
	m_currentScene = tag;
}

void Game::changeScene(const std::string& tag, std::shared_ptr<Scene> scene)
{
	m_scenes[tag] = scene;
	m_currentScene = tag;
}
