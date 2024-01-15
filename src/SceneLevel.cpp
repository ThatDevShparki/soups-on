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
		ActionSource::keyboard, sf::Keyboard::R, "toggleDebug"
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
	sState();
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

	if (m_renderDebug)
	{
		for (const auto& e: m_entities.getEntities())
		{
			auto& sprite = e->getComponent<CSprite>();

			sf::RectangleShape spriteShape;
			spriteShape.setSize(sprite.sprite.getGlobalBounds().getSize());
			spriteShape.setOutlineThickness(1.0f);
			spriteShape.setFillColor(sf::Color(0, 255, 255, 32));
			spriteShape.setOutlineColor(sf::Color::Cyan);
			spriteShape.setPosition(sprite.sprite.getPosition());

			target.draw(spriteShape);
		}
		for (const auto& e: m_entities.getEntities())
		{
			auto& transform = e->getComponent<CTransform>();

			sf::CircleShape posShape;
			posShape.setRadius(0.01f);a
			posShape.setOutlineThickness(2.0f);
			posShape.setFillColor(sf::Color::Magenta);
			posShape.setOutlineColor(sf::Color::Magenta);
			posShape.setPosition(sf::Vector2f(transform.pos));

			target.draw(posShape);
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
		if (action == "toggleDebug")
		{
			m_renderDebug = !m_renderDebug;
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
	auto& state     = m_player->getComponent<CState>();

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
	auto& map       = m_assets.getMap(m_map);

	Vec2 viewSize   = Vec2(m_view.getSize());
	Vec2 viewOrigin = transform.pos;

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

void SceneLevel::sState()
{
	auto& vel    = m_player->getComponent<CTransform>().vel;
	auto& state  = m_player->getComponent<CState>();
	auto& sprite = m_player->getComponent<CSprite>();

	if (vel.x != 0)
	{
		state.running = true;
		if (vel.x > 0)
		{
			state.facingLeft = false;
		}
		else
		{
			state.facingLeft = true;
		}
	}
	else
	{
		state.running = false;
	}
	if (vel.y != 0)
	{
		state.jumping = true;
	}
	else
	{
		state.jumping = false;
	}

	if (state.running)
	{
		sprite.sprite = m_assets.getSprite("dudeRunning", 0);
	}
	if (state.jumping)
	{
		sprite.sprite = m_assets.getSprite("dudeJumping", 0);
	}
	if (!state.running && !state.jumping)
	{
		sprite.sprite = m_assets.getSprite("dudeIdle", 0);
	}

	if (state.facingLeft)
	{
		sprite.sprite.setOrigin(
			sprite.sprite.getOrigin() + sf::Vector2f(
				sprite.sprite.getGlobalBounds().width,
				0
			)
		);
		sprite.sprite.setScale(-1.0f, 1.0f);
	}
	else
	{
		sprite.sprite.setScale(1.0f, 1.0f);
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
		m_assets.getSprite("dudeIdle", 0)
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
	auto& pos = player->getComponent<CTransform>().pos;

	player->addComponent<CInput>();
	player->addComponent<CState>();

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
