#include "SceneLevel.h"
#include <iostream>
#include <numeric>


SceneLevel::SceneLevel(Game* game, const std::string& manifestPath)
	: Scene(game)
{
	init(manifestPath);
}

SceneLevel::~SceneLevel() = default;


void SceneLevel::init(const std::string& manifestPath)
{
	// initialize placeholders
	m_assets.init(manifestPath);

	m_gridText.setCharacterSize(9);
	m_gridText.setFont(m_assets.getFont("font"));

	// init background
	m_background = "daytime";
	initBackground();

	// init entities (updates entities!)
	m_map = "map_004";
	initEntitiesFromMap();

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

	// TODO: Load these from file
	m_music = "main_theme";
	initSounds();
}

void SceneLevel::update(float delta)
{
	m_entities.update();

	sInput();
	sMovement(delta);
	sState();
	sCollisions();

	sAnimations(delta);
	sCamera();
	sParallax(delta);

	sSounds();

	m_currentFrame++;
}

void SceneLevel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// render scene
	target.setView(m_view);
	for (const auto& e: m_entities.getEntities())
	{
		auto& transform = e->getComponent<CTransform>();

		if (transform.has)
		{
			auto& sprite = e->getComponent<CSprite>();

			if (sprite.has)
			{
				sprite.sprite.setPosition(transform.pos.x, transform.pos.y);
				target.draw(sprite.sprite, states);
			}
		}
	}

	if (m_renderDebug)
	{
		for (const auto& e: m_entities.getEntities())
		{
			auto& transform   = e->getComponent<CTransform>();
			auto& boundingBox = e->getComponent<CBoundingBox>();

			if (boundingBox.has && transform.has)
			{
				sf::RectangleShape boundingShape;
				boundingShape.setSize(sf::Vector2f(boundingBox.size));
				boundingShape.setOutlineThickness(2.0f);
				boundingShape.setFillColor(sf::Color::Transparent);
				boundingShape.setOutlineColor(sf::Color::Black);
				boundingShape.setPosition(sf::Vector2f(transform.pos));

				target.draw(boundingShape, states);
			}
		}
		for (const auto& e: m_entities.getEntities())
		{
			auto& sprite = e->getComponent<CSprite>();

			if (sprite.has)
			{
				sf::RectangleShape spriteShape;
				spriteShape.setSize(sprite.sprite.getGlobalBounds().getSize());
				spriteShape.setOutlineThickness(1.0f);
				spriteShape.setFillColor(sf::Color(0, 255, 255, 32));
				spriteShape.setOutlineColor(sf::Color::Cyan);
				spriteShape.setPosition(sprite.sprite.getPosition());

				target.draw(spriteShape, states);
			}
		}
		for (const auto& e: m_entities.getEntities())
		{
			auto& transform = e->getComponent<CTransform>();

			if (transform.has)
			{
				sf::CircleShape posShape;
				posShape.setRadius(0.01f);
				posShape.setOutlineThickness(2.0f);
				posShape.setFillColor(sf::Color::Magenta);
				posShape.setOutlineColor(sf::Color::Magenta);
				posShape.setPosition(sf::Vector2f(transform.pos));

				target.draw(posShape, states);
			}
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

	transform.prevPos = transform.pos;
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
	auto& vel       = m_player->getComponent<CTransform>().vel;
	auto& state     = m_player->getComponent<CState>();
	auto& animation = m_player->getComponent<CAnimation>();

	m_player->addComponent<CPrevState>(state);
	auto& prevState = m_player->getComponent<CPrevState>();

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

	if (prevState != state)
	{
		if (state.running)
		{
			animation.animation = m_assets.getAnimation("dudeRunning");
		}
		if (state.jumping)
		{
			animation.animation = m_assets.getAnimation("dudeJumping");
		}
		if (!state.running && !state.jumping)
		{
			animation.animation = m_assets.getAnimation("dudeIdle");
		}

		auto& sprite = animation.animation.getCurrentFrame();
		if (state.facingLeft)
		{
			sprite.setOrigin(
				sprite.getOrigin() + sf::Vector2f(
					sprite.getGlobalBounds().width,
					0
				)
			);
			sprite.setScale(-1.0f, 1.0f);
		}
		else
		{
			sprite.setScale(1.0f, 1.0f);
		}
	}
}

void SceneLevel::sCollisions()
{
	const auto& pBox = m_player->getComponent<CBoundingBox>();
	const auto& pPos = m_player->getComponent<CTransform>().pos + pBox.halfSize;

	// Check for map collision
	if (pPos.x < pBox.halfSize.x ||
		pPos.x + pBox.halfSize.x > m_assets.getMap(m_map).width() * m_tileSize.x ||
		pPos.y < pBox.halfSize.y ||
		pPos.y + pBox.halfSize.y > m_assets.getMap(m_map).height() * m_tileSize.y)
	{
		m_player->getComponent<CTransform>().pos = m_player->getComponent<CTransform>().prevPos;
	}

	// Check for entity collision
	for (auto& e: m_entities.getEntities("boundingBox"))
	{
		const auto& eBox = e->getComponent<CBoundingBox>();
		const auto& ePos = e->getComponent<CTransform>().pos + eBox.halfSize;

		const Vec2  delta(abs(ePos.x - pPos.x), abs(ePos.y - pPos.y));
		const float ox = eBox.halfSize.x + pBox.halfSize.x - delta.x;
		const float oy = eBox.halfSize.y + pBox.halfSize.y - delta.y;

		if (ox > 0 && oy > 0)
		{
			m_player->getComponent<CTransform>().pos = m_player->getComponent<CTransform>().prevPos;
		}
	}
}

void SceneLevel::sParallax(float delta)
{
	auto& pvel = m_player->getComponent<CTransform>().vel;

	for (const auto& e: m_entities.getEntities())
	{
		auto& transform = e->getComponent<CTransform>();
		auto& sprite    = e->getComponent<CSprite>();
		auto& parallax  = e->getComponent<CParallax>();

		if (transform.has && parallax.has && sprite.has)
		{
			transform.vel = Vec2{ -parallax.factor * pvel.x, 0 };
			transform.pos += transform.vel;
		}

	}
}

void SceneLevel::sAnimations(float delta)
{
	for (auto& e: m_entities.getEntities())
	{
		auto& animation = e->getComponent<CAnimation>();

		if (animation.has)
		{
			animation.animation.update(delta);

			auto& sprite = e->getComponent<CSprite>();

			if (sprite.has)
			{
				sprite.sprite = animation.animation.getCurrentFrame();
			}
		}
	}
}

void SceneLevel::sSounds()
{
	for (auto& e: m_entities.getEntities("sound"))
	{
		auto& s = e->getComponent<CSound>();

		if (s.paused && (s.sound.getStatus() != sf::SoundSource::Paused ||
						 sf::SoundSource::Stopped))
		{
			s.sound.pause();
		}
		else if (!s.paused && s.sound.getStatus() != sf::SoundSource::Playing)
		{
			s.sound.play();
		}
	}
}


void SceneLevel::initEntitiesFromMap()
{
	const Map& map = m_assets.getMap(m_map);

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

	const Map& collisions = m_assets.getCollisions(m_map);
	for (int j = 0; j < collisions.height(); j++)
	{
		for (int i = 0; i < collisions.width(); i++)
		{
			const auto& l = collisions.at(i, j);
			if (std::reduce(l.begin(), l.end()) > 0)
			{
				spawnBoundingBox(
					{
						m_tileSize.x * i,
						collisions.height() * m_tileSize.y - m_tileSize.y * (j + 1)
					}
				);
			}
		}
	}

	m_entities.update();

	std::cout << "Loaded all entities from map: " + m_map << std::endl;
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

void SceneLevel::spawnBoundingBox(const Vec2& pos)
{
	auto boundingBox = m_entities.addEntity("boundingBox");
	boundingBox->addComponent<CTransform>(pos);
	boundingBox->addComponent<CBoundingBox>(m_tileSize);

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
	player->addComponent<CBoundingBox>(Vec2(sprite.getGlobalBounds().getSize()));
	player->addComponent<CAnimation>(m_assets.getAnimation("dudeIdle"));

	m_player = player;
}


void SceneLevel::initBackground()
{
	const std::vector<std::string> bgTags = {
		"bg_clouds_night", "bg_mountains_night"
	};


	for (int i = 0; i < bgTags.size(); i++)
	{
		const auto& tag = bgTags[i];

		auto bg = m_entities.addEntity("background");
		auto& bg_sprite = bg->addComponent<CSprite>(
			m_assets.getSprite(tag, 0)
		);
		bg_sprite.sprite.setTextureRect(
			sf::IntRect(
				0,
				0,
				bg_sprite.sprite.getGlobalBounds().width * 3,
				bg_sprite.sprite.getGlobalBounds().height
			)
		);

		float scale = (
			m_tileSize.y * 18 / bg_sprite.sprite.getGlobalBounds().height
		);

		bg_sprite.sprite.setScale({ scale, scale });
		bg->addComponent<CTransform>(
			Vec2{ -bg_sprite.sprite.getGlobalBounds().width / 3, 0 }
		);

		bg->addComponent<CParallax>(0.15f * float(i + 1));
	}

	const std::string& fgTag = "bg_clouds_floating_night";

	auto fg = m_entities.addEntity("backgroundDecor");
	auto& fg_sprite = fg->addComponent<CSprite>(
		m_assets.getSprite(fgTag, 0)
	);
	fg_sprite.sprite.setTextureRect(
		sf::IntRect(
			0,
			0,
			fg_sprite.sprite.getGlobalBounds().width * 3,
			fg_sprite.sprite.getGlobalBounds().height
		)
	);
	fg_sprite.sprite.setColor(sf::Color(255, 255, 255, 128));

	float scale = (m_tileSize.y * 18 / fg_sprite.sprite.getGlobalBounds().height) *
				  0.85f;

	fg_sprite.sprite.setScale({ scale, scale });
	fg->addComponent<CTransform>(
		Vec2{ -fg_sprite.sprite.getGlobalBounds().width / 3, 0 }
	);

	fg->addComponent<CParallax>(0.65f);
}


void SceneLevel::initSounds()
{
	auto theme = m_entities.addEntity("sound");
	theme->addComponent<CSound>(m_assets.getSound(m_music));

	auto& sound = theme->getComponent<CSound>().sound;
	sound.setVolume(15.0f);
	sound.setLoop(true);
}


void SceneLevel::onResizeView(const Vec2& size)
{
	float aspectRatio = size.y / size.x;
	float height      = m_tileSize.y * m_tileZoom;
	float width       = height / aspectRatio;
	m_view.setSize({ width, height });

	sCamera(); // make sure the camera is located correctly
}

