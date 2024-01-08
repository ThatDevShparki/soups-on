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

	// init entities (updates entities!)
	initEntitiesFromMap("map_002");

	// register actions
	registerAction(
		ActionSource::keyboard, sf::Keyboard::W, "up"
	);
	registerAction(
		ActionSource::keyboard, sf::Keyboard::S, "down"
	);
	registerAction(
		ActionSource::keyboard, sf::Keyboard::A, "left"
	);
	registerAction(
		ActionSource::keyboard, sf::Keyboard::D, "right"
	);
	registerAction(
		ActionSource::keyboard, sf::Keyboard::Space, "jump"
	);
	registerAction(
		ActionSource::keyboard, sf::Keyboard::E, "interact"
	);
	registerAction(
		ActionSource::keyboard, sf::Keyboard::G, "toggleGrid"
	);

	// Spawn player
	spawnPlayer();
}

void SceneLevel::update()
{
	m_entities.update();

	sRender();
	sInput();
	sMovement();

	m_currentFrame++;
}

void SceneLevel::quit()
{
}


void SceneLevel::doAction(const std::string& action, const ActionKind& kind)
{
	if (kind == ActionKind::pressed)
	{
		if (action == "toggleGrid")
		{
			m_renderGrid = !m_renderGrid;
		}
		else if (action == "up")
		{
			player()->getComponent<CInput>().up = true;
		}
		else if (action == "down")
		{
			player()->getComponent<CInput>().down = true;
		}
		else if (action == "right")
		{
			player()->getComponent<CInput>().right = true;
		}
		else if (action == "left")
		{
			player()->getComponent<CInput>().left = true;
		}
	}
	else if (kind == ActionKind::released)
	{
		if (action == "up")
		{
			player()->getComponent<CInput>().up = false;
		}
		else if (action == "down")
		{
			player()->getComponent<CInput>().down = false;
		}
		else if (action == "right")
		{
			player()->getComponent<CInput>().right = false;
		}
		else if (action == "left")
		{
			player()->getComponent<CInput>().left = false;
		}
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
		Vec2  s         = tileSize();
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
				m_gridText.setPosition(x + 3, height - y);
				m_game->window().draw(m_gridText);
			}
		}
	}
}

void SceneLevel::sInput()
{
	for (auto& entity: m_entities.getEntities())
	{
		if (entity->hasComponent<CInput>() && entity->hasComponent<CTransform>())
		{
			auto& input     = entity->getComponent<CInput>();
			auto& transform = entity->getComponent<CTransform>();

			Vec2 diff = { 0.0f, 0.0f };
			if (input.up)
			{
				diff -= { 0.0f, 1.0f };
			}
			if (input.down)
			{
				diff += { 0.0f, 1.0f };
			}
			if (input.right)
			{
				diff += { 1.0f, 0.0f };
			}
			if (input.left)
			{
				diff -= { 1.0f, 0.0f };
			}

			transform.vel = (diff * transform.maxVel);
		}
	}
}

void SceneLevel::sMovement()
{
	for (auto& entity: m_entities.getEntities())
	{
		if (entity->hasComponent<CTransform>())
		{
			auto& transform = entity->getComponent<CTransform>();
//			transform.vel += transform.acc;
//			if (transform.vel.mag() > transform.maxVel)
//			{
//				transform.vel = transform.vel.norm() * transform.maxVel;
//			}
			transform.pos += transform.vel;
		}
	}
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
				if (e == 0)
				{
					spawnEntrance(pos);
				}
				else if (e == 1)
				{
					spawnExit(pos);
				}
				else if (e == 2)
				{
					spawnTile(pos);
				}
				else if (e == 3)
				{
					spawnClimbableTile(pos);
				}
			}
		}
	}

	m_entities.update();
}

void SceneLevel::spawnEntrance(const Vec2& pos)
{
	auto entrance = m_entities.addEntity("entrance");
	entrance->addComponent<CTransform>(
		Vec2{
			tileSize().x * pos.x,
			float(m_game->window().getSize().y) - tileSize().y * (pos.y + 1)
		}
	);
	entrance->addComponent<CShape>(tileSize(), sf::Color(49, 162, 242));
}

void SceneLevel::spawnExit(const Vec2& pos)
{
	auto exit = m_entities.addEntity("exit");
	exit->addComponent<CTransform>(
		Vec2{
			tileSize().x * pos.x,
			float(m_game->window().getSize().y) - tileSize().y * (pos.y + 1)
		}
	);
	exit->addComponent<CShape>(tileSize(), sf::Color(235, 137, 49));
}

void SceneLevel::spawnTile(const Vec2& pos)
{
	auto tile = m_entities.addEntity("tile");
	tile->addComponent<CTransform>(
		Vec2{
			tileSize().x * pos.x,
			float(m_game->window().getSize().y) - tileSize().y * (pos.y + 1)
		}
	);
	tile->addComponent<CShape>(tileSize(), sf::Color::Black);
}

void SceneLevel::spawnClimbableTile(const Vec2& pos)
{
	auto tile = m_entities.addEntity("climbableTile");
	tile->addComponent<CTransform>(
		Vec2{
			tileSize().x * pos.x,
			float(m_game->window().getSize().y) - tileSize().y * (pos.y + 1)
		}
	);
	tile->addComponent<CShape>(tileSize(), sf::Color(68, 137, 26));
}

void SceneLevel::spawnPlayer()
{
	auto& spawnTile    = m_entities.getEntities("entrance")[0];
	auto& spawnTilePos = spawnTile->getComponent<CTransform>().pos;
	auto player = m_entities.addEntity("player");
	player->addComponent<CTransform>(
		Vec2{
			spawnTilePos.x, spawnTilePos.y + tileSize().y - 1.25f * tileSize().y
		},
		Vec2{ 0.0f, 0.0f },
		Vec2{ 0.0f, 0.0f },
		Vec2{ 0.0f, 0.0f },
		0.0f,
		5.0f / 60.0f,
		0.0f
	);
	player->addComponent<CShape>(
		Vec2{ 0.75f * tileSize().x, 1.25f * tileSize().y }, sf::Color::Magenta
	);
	player->addComponent<CInput>();
}


Vec2 SceneLevel::tileSize() const
{
	float scale = float(m_game->window().getSize().y) / (16 * m_tileSize.y);
	return m_tileSize * scale;
}

std::shared_ptr<Entity> SceneLevel::player() const
{
	return m_entities.getEntities("player")[0];
}
