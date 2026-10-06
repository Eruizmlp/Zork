#include "boss.h"
#include "room.h"

Boss::Boss(const std::string& name, const std::string& description,
	const std::vector<Room*>& route, const std::vector<Room*>& alertRoute)
	: NPC(name, description, route), m_alertRoute(alertRoute)
{
}

void Boss::alert()
{
	m_isAlerted = true;
	m_reactionTurns = REACTION_TURNS;
	m_alertStep = 0;
}

void Boss::onPlayerSpotted(const Player* /*player*/)
{
	m_hasCaughtPlayer = true;
}

void Boss::update()
{
	if (!m_isAlerted)
	{
		followRoute();
		return;
	}

	if (m_reactionTurns > 0)
	{
		--m_reactionTurns;
		return;
	}

	if (m_alertStep < m_alertRoute.size())
	{
		moveTo(m_alertRoute[m_alertStep]);
		++m_alertStep;
	}
	else
	{
		// Alert route finished: he calms down and goes back to his usual route
		m_isAlerted = false;
		m_alertStep = 0;
	}
}

std::string Boss::talk() const
{
	return "Ah, perfect timing! Do you have five minutes?";
}

