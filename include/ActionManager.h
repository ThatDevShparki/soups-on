#pragma once

#include <string>
#include <map>
#include <tuple>

enum class ActionSource
{
	unknown  = 0,
	keyboard = 1,
	mouse    = 2,
	gamepad  = 3,
};

enum class ActionKind
{
	unknown  = 0,
	pressed  = 1,
	released = 2,
};

typedef std::tuple<ActionSource, uint16_t> ActionKey;
typedef std::map<ActionKey, std::string>   ActionMap;

class ActionManager
{
	ActionMap m_actions;

public:

	void registerAction(const ActionKey& action, const std::string& tag);
	void registerAction(
		const ActionSource& source,
		const uint16_t& code,
		const std::string& tag
	);

	[[nodiscard]] const std::string& getAction(const ActionKey& action) const;
	[[nodiscard]] const std::string& getAction(
		const ActionSource& source,
		const uint16_t& code
	) const;

	[[nodiscard]] bool hasAction(const ActionKey& action) const;
	[[nodiscard]] bool hasAction(
		const ActionSource& source,
		const uint16_t& code
	) const;
};