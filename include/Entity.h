#pragma once

#include "Component.h"

#include <string>

typedef std::tuple<
	CTransform,
	CBoundingBox,
	CShape,
	CInput,
	CSprite,
	CState,
	CPrevState,
	CAnimation,
	CSound
> ComponentTuple;

class Entity
{
	friend class EntityManager;

	bool           m_active = true;
	size_t         m_id     = 0;
	std::string    m_tag    = "default";
	ComponentTuple m_components;

	Entity(size_t id, const std::string& tag);

public:

	void destroy();

	[[nodiscard]] bool isActive() const;
	[[nodiscard]] size_t id() const;
	[[nodiscard]] const std::string& tag() const;

	template<typename T>
	[[nodiscard]] bool hasComponent() const
	{
		return getComponent<T>().has;
	}

	template<typename T, typename... TArgs>
	[[nodiscard]]T& addComponent(TArgs&& ... mArgs)
	{
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template<typename T>
	[[nodiscard]] T& getComponent()
	{
		return std::get<T>(m_components);
	}

	template<typename T>
	[[nodiscard]] const T& getComponent() const
	{
		return std::get<T>(m_components);
	}

	template<typename T>
	void removeComponent()
	{
		getComponent<T>() = T();
	}
};
