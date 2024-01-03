#include "Game.h"


Game::Game()
{
	m_window.create(
		sf::VideoMode::getDesktopMode(),
		"Soups On"
	);
	m_window.setFramerateLimit(60);
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
	sUserInput();
	sRender();
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