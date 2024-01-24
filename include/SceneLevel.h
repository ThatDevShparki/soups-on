#pragma once


#include "SFML/Graphics.hpp"
#include "Scene.h"
#include <string>


class SceneLevel : public Scene
{

protected:

	sf::Text                m_gridText;
	std::shared_ptr<Entity> m_player;
	std::string             m_map;
	std::string             m_background;
	bool                    m_renderDebug = false;
	Vec2                    m_tileSize    = { 32.0f, 32.0f };
	float                   m_tileZoom    = 16.0f;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void sInput();
	void sMovement(float delta);
	void sCamera();
	void sState();
	void sCollisions();
	void sAnimations(float delta);

	void initEntitiesFromMap(const std::string& mapName);
	void spawnEntrance(const Vec2& pos);
	void spawnExit(const Vec2& pos);
	void spawnTile(const size_t i, const Vec2& pos);
	void spawnBoundingBox(const Vec2& pos);
	void spawnClimbableTile(const Vec2& pos);
	void spawnPlayer();


public:
	void init(const std::string& manifestPath);

	explicit SceneLevel(Game* game, const std::string& manifestPath);
	~SceneLevel() override;

	void update(float delta) override;
	void quit() override;
	void doAction(const std::string& action, const ActionKind& kind) override;

	void onResizeView(const Vec2& size) override;

	const Background& background() const;
};