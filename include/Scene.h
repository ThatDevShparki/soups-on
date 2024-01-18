#pragma once

#include "SFML/Graphics.hpp"
#include "Game.h"
#include "EntityManager.h"
#include "AssetManager.h"
#include "ActionManager.h"

class Game;

class Scene : public sf::Drawable
{

protected:

	Game* m_game = nullptr;

	sf::View m_view;
	size_t   m_currentFrame = 0;
	bool     m_paused       = false;
	bool     m_ended        = false;

	EntityManager m_entities;
	AssetManager  m_assets;
	ActionManager m_actions;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

public:

	Scene();
	Scene(Game* game);
	virtual ~Scene();

	bool paused() const;
	void setPaused(const bool paused);
	void simulate(size_t frameCount);

	// action management
	void registerAction(
		const ActionSource& source,
		const uint16_t& code,
		const std::string& action
	);
	bool handlesAction(const ActionSource& source, const uint16_t& code) const;
	[[nodiscard]] const std::string& getActionFor(
		const ActionSource& source, const uint16_t& code
	) const;

	const sf::View& view() const;
	const EntityManager& entities() const;

	virtual void update(float delta) = 0;
	virtual void quit() = 0;
	virtual void doAction(const std::string& action, const ActionKind& kind) = 0;
	virtual void onResizeView(const Vec2& size) = 0;

};