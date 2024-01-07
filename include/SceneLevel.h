#pragma once


#include "SFML/Graphics.hpp"
#include "Scene.h"
#include <string>

class SceneLevel : public Scene
{

protected:

	float m_scale    = 1.0f;
	Vec2  m_tileSize = { 32.0f, 32.0f };

	bool     m_renderGrid = false;
	sf::Text m_gridText;

	void sDoAction(const std::string& action) override;
	void sRender() override;

	[[nodiscard]] const Vec2& gridSize() const;

	void initEntitiesFromMap(const std::string& mapName);
	void spawnTile(const Vec2& pos);

public:
	void init(const std::string& manifestPath);

	explicit SceneLevel(Game* game, const std::string& manifestPath);
	~SceneLevel() override;

	void update() override;
	void quit() override;

	Vec2 tileSize() const;
};