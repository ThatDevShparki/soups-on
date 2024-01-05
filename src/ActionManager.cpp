#include "ActionManager.h"

void ActionManager::registerAction(
	const std::string& tag,
	const ActionSource& source,
	const uint16_t& key
)
{
	m_actions[tag] = { source, key };
}

const ActionValue& ActionManager::getAction(const std::string& tag) const
{
	return m_actions.at(tag);
}
