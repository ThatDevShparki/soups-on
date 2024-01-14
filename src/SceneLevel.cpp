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

	m_gridText.setCharacterSize(9);
	m_gridText.setFont(m_assets.getFont("font"));

	// init entities (updates entities!)
	initEntitiesFromMap("map_001");

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

	// initialize view
	onResizeView(m_game->view());
}

void SceneLevel::update()
{
	m_entities.update();

	sRender();
	sInput();
	sMovement();
	sCamera();

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
			m_player->getComponent<CInput>().up = true;
		}
		else if (action == "down")
		{
			m_player->getComponent<CInput>().down = true;
		}
		else if (action == "right")
		{
			m_player->getComponent<CInput>().right = true;
		}
		else if (action == "left")
		{
			m_player->getComponent<CInput>().left = true;
		}
	}
	else if (kind == ActionKind::released)
	{
		if (action == "up")
		{
			m_player->getComponent<CInput>().up = false;
		}
		else if (action == "down")
		{
			m_player->getComponent<CInput>().down = false;
		}
		else if (action == "right")
		{
			m_player->getComponent<CInput>().right = false;
		}
		else if (action == "left")
		{
			m_player->getComponent<CInput>().left = false;
		}
	}
}

void SceneLevel::sRender()
{
	for (const auto& e: m_entities.getEntities())
	{
		auto& transform = e->getComponent<CTransform>();
		auto& sprite    = e->getComponent<CSprite>();

		if (transform.has && sprite.has)
		{
			sprite.sprite.setPosition(transform.pos.x, transform.pos.y);
			m_game->window().draw(sprite.sprite);
		}
	}
}

void SceneLevel::sInput()
{

	auto& input     = m_player->getComponent<CInput>();
	auto& transform = m_player->getComponent<CTransform>();

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

void SceneLevel::sMovement()
{
	auto& transform = m_player->getComponent<CTransform>();

	transform.vel += transform.acc;
	if (transform.vel.mag() > transform.maxVel)
	{
		transform.vel = transform.vel.norm() * transform.maxVel;
	}
	transform.vel   = transform.vel * m_game->delta();

	transform.pos += transform.vel;

}

void SceneLevel::sCamera()
{
	auto& transform = m_player->getComponent<CTransform>();
	auto& sprite    = m_player->getComponent<CSprite>();
	auto& map       = m_assets.getMap(m_map);

	Vec2 viewSize   = Vec2(m_game->view().getSize());
	Vec2 viewOrigin = Vec2(transform.pos) +
					  Vec2(sprite.sprite.getGlobalBounds().getSize()) / 2;

	if (viewOrigin.x < viewSize.x / 2)
	{
		viewOrigin.x = viewSize.x / 2;
	}
	else if (viewOrigin.x > map.width() * m_tileSize.x - viewSize.x / 2)
	{
		viewOrigin.x = map.width() * m_tileSize.x - viewSize.x / 2;
	}

	if (viewOrigin.y < viewSize.y / 2)
	{
		viewOrigin.y = viewSize.y / 2;
	}
	else if (viewOrigin.y > map.height() * m_tileSize.y - viewSize.y / 2)
	{
		viewOrigin.y = map.height() * m_tileSize.y - viewSize.y / 2;
	}

	std::cout
		<< std::to_string(viewOrigin.x) + ", " + std::to_string(viewOrigin.y) + "-" +
		   std::to_string(map.height() * m_tileSize.y - viewSize.y / 2)
		<< std::endl;

	auto& view = m_game->view();
	view.setCenter(sf::Vector2f(viewOrigin));
}


void SceneLevel::initEntitiesFromMap(const std::string& mapName)
{
	const Map& map = m_assets.getMap(mapName);

	for (int j = 0; j < map.height(); j++)
	{
		for (int i = 0; i < map.width(); i++)
		{
			const Vec2 pos{ float(i), float(j) };
			const Vec2 tilePos{
				m_tileSize.x * pos.x,
				map.height() * m_tileSize.y - m_tileSize.y * (pos.y + 1)
			};
			const auto& layer = map.at(pos);

			for (const auto& e: layer)
			{
				if (e == 139)
				{
					spawnEntrance(tilePos);
				}
				else if (e != -1)
				{
					spawnTile(e, tilePos);
				}
			}
		}
	}

	m_entities.update();
	m_map = mapName;

	std::cout << "Loaded all entities from map: " + mapName << std::endl;
}

void SceneLevel::spawnEntrance(const Vec2& pos)
{
	auto entrance = m_entities.addEntity("entrance");
	entrance->addComponent<CTransform>(pos);
	entrance->addComponent<CShape>(m_tileSize, sf::Color(49, 162, 242));
}

void SceneLevel::spawnTile(size_t i, const Vec2& pos)
{
	auto tile = m_entities.addEntity("tile");
	tile->addComponent<CTransform>(pos);
	tile->addComponent<CSprite>(m_assets.getSprite("level_debug", i));
}

void SceneLevel::spawnPlayer()
{
	auto& spawnTile    = m_entities.getEntities("entrance")[0];
	auto& spawnTilePos = spawnTile->getComponent<CTransform>().pos;
	auto player = m_entities.addEntity("player");
	player->addComponent<CTransform>(
		Vec2{
			spawnTilePos.x, spawnTilePos.y + m_tileSize.y - 1.25f * m_tileSize.y
		},
		Vec2{ 0.0f, 0.0f },
		Vec2{ 0.0f, 0.0f },
		Vec2{ 0.0f, 0.0f },
		0.0f,
		600.0f,
		0.0f
	);
	player->addComponent<CSprite>(
		m_assets.getSprite("player_debug", 0)
	);
	player->addComponent<CInput>();
	m_player = player;
}

void SceneLevel::onResizeView(sf::View& view)
{
	float aspectRatio = view.getSize().y / view.getSize().x;
	float height      = m_tileSize.y * m_tileZoom;
	float width       = height / aspectRatio;
	view.setSize({ width, height });

	sCamera(); // make sure the camera is located correctly
}


float SceneLevel::height() const
{
	return float(m_game->window().getSize().y);
}