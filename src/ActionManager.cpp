#include "ActionManager.h"

void ActionManager::registerAction(const ActionKey& action, const std::string& tag)
{
	m_actions[action] = tag;
}

void ActionManager::registerAction(
	const ActionSource& source,
	const uint16_t& code,
	const std::string& tag
)
{
	registerAction({ source, code }, tag);
}

const std::string& ActionManager::getAction(const ActionKey& action) const
{
	return m_actions.at(action);
}

const std::string&
ActionManager::getAction(const ActionSource& source, const uint16_t& code) const
{
	return getAction({ source, code });
}