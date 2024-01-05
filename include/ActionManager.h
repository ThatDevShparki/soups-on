#pragma once

#include <string>
#include <map>
#include <tuple>

enum ActionSource
{
	unknown  = 0,
	keyboard = 1,
	mouse    = 2,
};
typedef std::tuple<ActionSource, uint16_t> ActionValue;
typedef std::map<std::string, ActionValue> ActionMap;

class ActionManager
{
	ActionMap m_actions;

public:

	void registerAction(
		const std::string& tag,
		const ActionSource& source,
		const uint16_t& key
	);
	[[nodiscard]] const ActionValue& getAction(const std::string& tag) const;
};