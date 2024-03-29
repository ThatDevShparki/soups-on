#pragma once

#include "Entity.h"
#include "Vec2.h"

#include <map>
#include <string>
#include <vector>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec>     EntityMap;

class EntityManager
{
	EntityVec m_entities;
	EntityVec m_entitiesToAdd;
	EntityMap m_entityMap;
	size_t    m_totalEntities = 0;

	static void removeDeadEntities(EntityVec& vec);

public:

	EntityManager();

	void update();

	std::shared_ptr<Entity> addEntity(const std::string& tag);
	[[nodiscard]] const EntityVec& getEntities() const;
	[[nodiscard]] const EntityVec& getEntities(const std::string& tag) const;
	[[nodiscard]] const EntityMap& getEntityMap() const;
};