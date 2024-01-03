#include "EntityManager.h"

EntityManager::EntityManager() = default;

void EntityManager::update()
{
	// Add all entities queued for addition
	for (auto& entity: m_entitiesToAdd)
	{
		m_entities.push_back(entity);
		m_entityMap[entity->tag()].push_back(entity);
	}

	// Remove all entities that are no longer alive
	removeDeadEntities(m_entities);
	for (auto& [_, entities]: m_entityMap)
	{
		removeDeadEntities(entities);
	}
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	vec.erase(
		std::remove_if(
			vec.begin(), vec.end(), [](const auto& entity)
			{
				return !entity->isActive();
			}
		), vec.end());
}


std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_entitiesToAdd.push_back(entity);

	return entity;
}

const EntityVec& EntityManager::getEntities() const
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag) const
{
	return m_entityMap.at(tag);
}

const EntityMap& EntityManager::getEntityMap() const
{
	return m_entityMap;
}
