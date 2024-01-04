#include "Game.h"

#include <fstream>
#include <iostream>


Game::Game() = default;

Game::Game(const std::string& path)
{
	init(path);
}


void Game::init(const std::string& path)
{
	m_window.create(
		sf::VideoMode::getDesktopMode(),
		"Soups On"
	);
	m_window.setFramerateLimit(60);

	// Load assets
	std::ifstream file(path);
	if (!file)
	{
		std::cerr << "Could not load manifest from path: " + path << std::endl;
		exit(-1);
	}

	std::string head;
	while (file >> head)
	{
		if (head == "font")
		{
			std::string n, p;
			file >> n >> p;
			m_assets.addFont(n, "assets/" + p);
		}
		else if (head == "texture")
		{
			std::string n, p;
			file >> n >> p;
			m_assets.addTexture(n, "assets/" + p);
		}
	}

	std::cout << "Loaded all assets from manifest " + path << std::endl;
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
	m_entities.update();

	sUserInput();
	sRender();

	m_currentFrame++;
}

void Game::sRender()
{
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
	}
}


void Game::quit()
{
	m_running = false;
}

const sf::RenderWindow& Game::window() const
{
	return m_window;
}

bool Game::isRunning() const
{
	return m_window.isOpen() && m_running;
}