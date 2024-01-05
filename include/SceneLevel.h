#pragma once


#include "SFML/Graphics.hpp"
#include "Scene.h"
#include <string>

class SceneLevel : public Scene
{

protected:

	float m_scale    = 3.0;
	Vec2  m_tileSize = { 32.0f, 32.0f };

	bool     m_renderGrid = false;
	sf::Text m_gridText;

	void sDoAction(const std::string& action) override;
	void sRender() override;

	[[nodiscard]] const Vec2& gridSize() const;

public:
	void init(const std::string& manifestPath);

	explicit SceneLevel(Game* game, const std::string& manifestPath);
	~SceneLevel() override;

	void update() override;
	void quit() override;
};