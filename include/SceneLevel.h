#pragma once


#include "SFML/Graphics.hpp"
#include "Scene.h"
#include <string>


class SceneLevel : public Scene
{

protected:

	float m_scale    = 3.0f;
	Vec2  m_tileSize = { 32.0f, 32.0f };

	bool     m_renderGrid = false;
	sf::Text m_gridText;

	void sRender() override;
	void sInput();
	void sMovement();

	void initEntitiesFromMap(const std::string& mapName);
	void spawnEntrance(const Vec2& pos);
	void spawnExit(const Vec2& pos);
	void spawnTile(const Vec2& pos);
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

	std::shared_ptr<Entity> player() const;
};