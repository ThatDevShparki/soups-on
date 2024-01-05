#pragma once

#include <string>
#include <map>
#include <tuple>

enum ActionSource
{
	unknown  = 0,
	keyboard = 1,
	mouse    = 2,
	gamepad  = 3,
};
typedef std::tuple<ActionSource, uint16_t> ActionKey;
typedef std::map<ActionKey, std::string>   ActionMap;

class ActionManager
{
	ActionMap m_actions;

public:

	void registerAction(const ActionKey& action, const std::string& tag);
	void registerAction(
		const ActionSource& source, const uint16_t& code, const std::string& tag
	);

	[[nodiscard]] const std::string& getAction(const ActionKey& action) const;
	[[nodiscard]] const std::string& getAction(
		const ActionSource& source, const uint16_t& code
	) const;
};