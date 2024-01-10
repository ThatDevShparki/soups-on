#pragma once


#include "SFML/Graphics.hpp"
#include "Scene.h"
#include <string>


class SceneLevel : public Scene
{

protected:

	Vec2                    m_tileSize   = { 32.0f, 32.0f };
	bool                    m_renderGrid = false;
	sf::Text                m_gridText;
	std::shared_ptr<Entity> m_player;
	float                   m_offset     = 0.0f;
	std::string             m_map;

	void sRender() override;
	void sInput();
	void sMovement();
	void sCamera();

	void initEntitiesFromMap(const std::string& mapName);
	void spawnEntrance(const Vec2& pos);
	void spawnExit(const Vec2& pos);
	void spawnTile(const size_t i, const Vec2& pos);
	void spawnClimbableTile(const Vec2& pos);
	void spawnPlayer();


public:
	void init(const std::string& manifestPath);

	explicit SceneLevel(Game* game, const std::string& manifestPath);
	~SceneLevel() override;

	void update() override;
	void quit() override;
	void doAction(const std::string& action, const ActionKind& kind) override;

	Vec2 tileSize() const;

	float width() const;
	float height() const;
};