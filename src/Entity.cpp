#include "Entity.h"

Entity::Entity(size_t id, const std::string& tag)
	: m_id(id), m_tag(tag)
{
	m_components = { CTransform(), CBoundingBox() };
}


bool Entity::isActive() const
{
	return m_active;
}

size_t Entity::id() const
{
	return m_id;
}

const std::string& Entity::tag() const
{
	return m_tag;
}


template<typename T>
T& Entity::getComponent()
{
	return std::get<T>(m_components);
}

template<typename T>
const T& Entity::getComponent() const
{
	return std::get<T>(m_components);
}

template<typename T>
bool Entity::hasComponent() const
{
	return getComponent<T>().has;
}

template<typename T, typename... TArgs>
T& Entity::addComponent(TArgs& ...args)
{
	auto& component = getComponent<T>();
	component = T(std::forward<TArgs>(args)...);
	component.has = true;
	return component;
}

template<typename T>
void Entity::removeComponent()
{
	getComponent<T>() = T();
}


