#pragma once

#include "Game.h"
#include "EntityManager.h"
#include "AssetManager.h"
#include "ActionManager.h"

class Game;

class Scene
{

protected:

	Game* m_game = nullptr;

	size_t m_currentFrame = 0;
	bool   m_paused       = false;
	bool   m_ended        = false;

	EntityManager m_entities;
	AssetManager  m_assets;
	ActionManager m_actions;

	virtual void sDoAction(const std::string& action) = 0;
	virtual void sRender() = 0;

public:

	Scene();
	Scene(Game* game);
	virtual ~Scene();

	bool paused() const;
	void setPaused(const bool paused);
	void simulate(size_t frameCount);

	// action management
	void doAction(const std::string& action);
	void registerAction(
		const ActionSource& source,
		const uint16_t& code,
		const std::string& action
	);
	
	virtual void update() = 0;
	virtual void quit() = 0;

};