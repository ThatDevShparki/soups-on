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
	m_window.setFramerateLimit(m_targetFps);
	m_view = m_window.getDefaultView();

	// load assets
	m_assets = AssetManager(manifestPath);

	// register scene
	changeScene("level", std::make_shared<SceneLevel>(this, manifestPath));
}


void Game::run()
{
	while (isRunning())
	{
		m_clock.restart();

		update();

		m_frames++;
		m_delta = m_clock.getElapsedTime().asSeconds();
		m_totalDelta += m_delta;
		if (m_frames >= m_targetFps)
		{
			m_frames -= m_targetFps;
			m_avgDelta   = m_totalDelta / m_targetFps;
			m_fps        = 1 / m_avgDelta;
			m_totalDelta = 0;
		}
	}
}

void Game::update()
{
	m_window.clear(sf::Color::White);

	sUserInput();

	// render scene content
	m_window.setView(currentScene()->view());
	currentScene()->update(m_delta);
	m_window.draw(*currentScene());

	// render game content
	m_window.setView(m_view);
	if (m_showDelta)
	{
		sShowDebug();
	}

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

		// check for global events

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				quit();
			}
			else if (
				event.key.code == sf::Keyboard::R
				)
			{
				m_showDelta = !m_showDelta;
			}
		}

		if (event.type == sf::Event::Resized)
		{
			m_view = sf::View(
				{
					0.0f,
					0.0f,
					float(event.size.width),
					float(event.size.height)
				}
			);
			currentScene()->onResizeView(Vec2(m_view.getSize()));
		}

		// pass keyboard events to current scene

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


void Game::sShowDebug()
{
	float margin = 16.0f;

	sf::RectangleShape debugBox;
	debugBox.setFillColor(sf::Color(0, 0, 0, 125));

	std::vector<sf::Text> debugTexts;

	sf::Text avgFpsText;
	avgFpsText.setString("FPS: " + std::to_string(m_fps));
	debugTexts.push_back(avgFpsText);

	sf::Text deltaText;
	deltaText.setString("delta: " + std::to_string(m_avgDelta));
	debugTexts.push_back(deltaText);

	float maxWidth  = 0;
	float maxHeight = 0;
	for (auto& e: debugTexts)
	{
		e.setCharacterSize(24.0f);
		e.setFont(m_assets.getFont("font"));
		e.setFillColor(sf::Color::White);

		float width = e.getGlobalBounds().width;
		if (width > maxWidth)
		{
			maxWidth = width;
		}

		float height = e.getGlobalBounds().height;
		if (height > maxHeight)
		{
			maxHeight = height;
		}
	}

	debugBox.setSize(
		sf::Vector2f(
			maxWidth + 2 * margin,
			margin + (maxHeight + margin) * debugTexts.size()
		)
	);
	debugBox.setPosition(
		m_window.getSize().x - (maxWidth + 3 * margin),
		margin
	);

	for (int i = 0; i < debugTexts.size(); i++)
	{
		auto& e = debugTexts[i];
		e.setPosition(
			m_window.getSize().x - (maxWidth + 2 * margin),
			1.25 * margin + (margin + e.getGlobalBounds().height) * i
		);

	}

	m_window.draw(debugBox);
	for (const auto& e: debugTexts)
	{
		m_window.draw(e);
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

sf::View& Game::view()
{
	return m_view;
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
