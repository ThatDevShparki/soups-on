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

bool Scene::handlesAction(const ActionSource& source, const uint16_t& code) const
{
	return m_actions.hasAction(source, code);
}

const std::string&
Scene::getActionFor(const ActionSource& source, const uint16_t& code) const
{
	return m_actions.getAction(source, code);
}

const sf::View& Scene::view() const
{
	return m_view;
}

const EntityManager& Scene::entities() const
{
	return m_entities;
}