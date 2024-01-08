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
ActionManager::getAction(
	const ActionSource& source,
	const uint16_t& code
) const
{
	return getAction({ source, code });
}


bool ActionManager::hasAction(const ActionKey& action) const
{
	return m_actions.find(action) != m_actions.end();
}

bool ActionManager::hasAction(const ActionSource& source, const uint16_t& code) const
{
	return hasAction({ source, code });
}