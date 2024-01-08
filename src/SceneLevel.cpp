#include "SceneLevel.h"
#include <iostream>


SceneLevel::SceneLevel(Game* game, const std::string& manifestPath)
	: Scene(game)
{
	init(manifestPath);
}

SceneLevel::~SceneLevel() = default;


void SceneLevel::init(const std::string& manifestPath)
{
	// initialize placeholders
	m_assets = AssetManager(manifestPath);

	m_gridText.setCharacterSize(ceil(9 * m_scale));
	m_gridText.setFont(m_assets.getFont("font"));

	// init entities
	initEntitiesFromMap("map_002");

	// register actions
	registerAction(keyboard, sf::Keyboard::W, "up");
	registerAction(keyboard, sf::Keyboard::Up, "up");
	registerAction(keyboard, sf::Keyboard::S, "down");
	registerAction(keyboard, sf::Keyboard::Down, "down");
	registerAction(keyboard, sf::Keyboard::A, "left");
	registerAction(keyboard, sf::Keyboard::Left, "left");
	registerAction(keyboard, sf::Keyboard::D, "right");
	registerAction(keyboard, sf::Keyboard::Right, "right");
	registerAction(keyboard, sf::Keyboard::Space, "jump");
	registerAction(keyboard, sf::Keyboard::E, "interact");
	registerAction(keyboard, sf::Keyboard::G, "toggleGrid");
}

void SceneLevel::update()
{
	m_entities.update();

	sRender();

	m_currentFrame++;
}

void SceneLevel::quit()
{
}


void SceneLevel::sDoAction(const std::string& action)
{
	if (action == "toggleGrid")
	{
		m_renderGrid = !m_renderGrid;
	}
}

void SceneLevel::sRender()
{
	for (const auto& e: m_entities.getEntities())
	{
		auto& transform = e->getComponent<CTransform>();
		auto& shape     = e->getComponent<CShape>();

		if (transform.has && shape.has)
		{
			shape.shape.setPosition(transform.pos.x, transform.pos.y);
			shape.shape.setOutlineColor(shape.color);
			shape.shape.setFillColor(shape.color);
			m_game->window().draw(shape.shape);
		}
	}

	if (m_renderGrid)
	{
		Vec2  s         = gridSize();
		float width     = float(m_game->window().getSize().x);
		float height    = float(m_game->window().getSize().y);
		float leftX     = m_game->window().getView().getCenter().x - width / 2.0f;
		float rightX    = leftX + width + s.x;
		float nextGridX = leftX - float(int(leftX) % int(s.x));

		for (float x = nextGridX; x <= rightX; x += s.x)
		{
			m_game->drawLine({ x, 0 }, { x, height });
		}
		for (float y = 0; y < height; y += s.y)
		{
			m_game->drawLine({ 0, y }, { width, y });

			for (float x = nextGridX; x < rightX; x += s.x)
			{
				std::string xCell = std::to_string((int)x / (int)s.x);
				std::string yCell = std::to_string((int)y / (int)s.y);
				m_gridText.setString("(" + xCell + "," + yCell + ")");
				m_gridText.setPosition(x + 3, height - y - s.y);
				m_game->window().draw(m_gridText);
			}
		}
	}
}

const Vec2& SceneLevel::gridSize() const
{
	float scale = float(m_game->window().getSize().y) / (16 * m_tileSize.y);
	return m_tileSize * scale;
}


void SceneLevel::initEntitiesFromMap(const std::string& mapName)
{
	const Map& map = m_assets.getMap(mapName);

	for (int j = 0; j < map.height(); j++)
	{
		for (int i = 0; i < map.width(); i++)
		{
			const Vec2 pos{ float(i), float(j) };
			const auto& layer = map.at(pos);

			for (const auto& e: layer)
			{
				if (e == 2)
				{
					spawnTile(pos + Vec2{ 0.0f, 1.0f });
				}
			}
		}
	}
}

void SceneLevel::spawnTile(const Vec2& pos)
{
	auto tile = m_entities.addEntity("tile");
	tile->addComponent<CTransform>(
		Vec2{
			tileSize().x * pos.x,
			float(m_game->window().getSize().y) - tileSize().y * pos.y
		}
	);
	tile->addComponent<CShape>(tileSize(), sf::Color::Black);
}


Vec2 SceneLevel::tileSize() const
{
	float scale = float(m_game->window().getSize().y) / (16 * m_tileSize.y);
	return m_tileSize * scale;
}


