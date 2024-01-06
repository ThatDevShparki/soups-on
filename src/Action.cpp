#include "Action.h"

Action::Action() = default;

Action::Action(const ActionSource& source, const ActionKind& kind, size_t code)
	: m_source(source), m_kind(kind), m_code(code)
{
}


const ActionSource& Action::source() const
{
	return m_source;
}

const ActionKind& Action::kind() const
{
	return m_kind;
}

size_t Action::code() const
{
	return m_code;
}