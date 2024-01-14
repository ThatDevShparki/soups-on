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
	initEntitiesFromMap("map_004");

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
	onResizeView(Vec2(m_game->view().getSize()));
}

void SceneLevel::update(float delta)
{
	m_entities.update();
	
	sInput();
	sMovement(delta);
	sCamera();

	m_currentFrame++;
}

void SceneLevel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& e: m_entities.getEntities())
	{
		auto& transform = e->getComponent<CTransform>();
		auto& sprite    = e->getComponent<CSprite>();

		if (transform.has && sprite.has)
		{
			sprite.sprite.setPosition(transform.pos.x, transform.pos.y);
			target.draw(sprite.sprite);
		}
	}
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

void SceneLevel::sMovement(float delta)
{
	auto& transform = m_player->getComponent<CTransform>();

	transform.vel += transform.acc;
	if (transform.vel.mag() > transform.maxVel)
	{
		transform.vel = transform.vel.norm() * transform.maxVel;
	}
	transform.vel   = transform.vel * delta;

	transform.pos += transform.vel;

}

void SceneLevel::sCamera()
{
	auto& transform = m_player->getComponent<CTransform>();
	auto& sprite    = m_player->getComponent<CSprite>();
	auto& map       = m_assets.getMap(m_map);

	Vec2 viewSize   = Vec2(m_view.getSize());
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

	m_view.setCenter(sf::Vector2f(viewOrigin));
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
				if (e == 173)
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
	tile->addComponent<CSprite>(m_assets.getSprite("background", i));
}

void SceneLevel::spawnPlayer()
{
	auto& spawnTile = m_entities.getEntities("entrance")[0];
	auto spawnTilePos =
			 spawnTile->getComponent<CTransform>().pos +
			 Vec2(0.0f, m_tileSize.y);

	auto player = m_entities.addEntity("player");
	player->addComponent<CSprite>(
		m_assets.getSprite("dude", 0)
	);
	auto& sprite = player->getComponent<CSprite>().sprite;
	player->addComponent<CTransform>(
		Vec2{
			spawnTilePos.x,
			spawnTilePos.y - sprite.getGlobalBounds().height
		},
		Vec2{ 0.0f, 0.0f },
		Vec2{ 0.0f, 0.0f },
		Vec2{ 0.0f, 0.0f },
		0.0f,
		300.0f,
		0.0f
	);
	player->addComponent<CInput>();
	m_player = player;
}

void SceneLevel::onResizeView(const Vec2& size)
{
	float aspectRatio = size.y / size.x;
	float height      = m_tileSize.y * m_tileZoom;
	float width       = height / aspectRatio;
	m_view.setSize({ width, height });

	sCamera(); // make sure the camera is located correctly
}
