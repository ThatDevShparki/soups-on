#pragma once

#include "SFML/Graphics.hpp"

enum ActionSource
{
	keyboard, mouse, gamepad
};

enum ActionKind
{
	start, stop
};

class Action
{
	ActionSource m_source;
	ActionKind   m_kind;
	size_t       m_code{};

public:

	Action();
	Action(const ActionSource& source, const ActionKind& kind, size_t code);

	[[nodiscard]] const ActionSource& source() const;
	[[nodiscard]] const ActionKind& kind() const;
	[[nodiscard]] size_t code() const;

};