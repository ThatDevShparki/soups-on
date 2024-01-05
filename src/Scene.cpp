#include "Scene.h"


Scene::Scene() = default;


Scene::Scene(Game* game)
	: m_game(game)
{
}

Scene::~Scene() = default;


bool Scene::paused() const
{
	return m_paused;
}

void Scene::setPaused(const bool paused)
{
	m_paused = paused;
}

void Scene::registerAction(
	const ActionSource& source,
	const uint16_t& code,
	const std::string& action
)
{
	m_actions.registerAction(source, code, action);
}