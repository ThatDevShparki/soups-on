#pragma once

#include "Component.h"

#include <string>

class Entity
{
	friend class EntityManager;

	bool           m_active = true;
	size_t         m_id     = 0;
	std::string    m_tag    = "default";
	ComponentTuple m_components;

	Entity(size_t id, const std::string& tag);

public:

	[[nodiscard]] bool isActive() const;
	[[nodiscard]] size_t id() const;
	[[nodiscard]] const std::string& tag() const;

	template<typename T>
	T& getComponent();

	template<typename T>
	const T& getComponent() const;

	template<typename T>
	bool hasComponent() const;

	template<typename T, typename... TArgs>
	T& addComponent(TArgs& ... args);

	template<typename T>
	void removeComponent();
};
