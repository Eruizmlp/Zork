#include "boss.h"
#include "room.h"
#include <iostream>

Boss::Boss(const std::string& name, const std::string& description,
	const std::vector<Room*>& route, const std::vector<Room*>& alertRoute)
	: NPC(name, description, route), m_alertRoute(alertRoute)
{
}

void Boss::alert()
{
	std::cout << "From far away you hear the boss: \"WHAT?! NOBODY LEAVES THIS OFFICE BEFORE FIVE!\"\n";
	startAlert(BossState::GUARDING_ENTRANCE, m_alertRoute);
}

void Boss::search(const std::vector<Room*>& route)
{
	std::cout << "You hear the boss: \"SOMEONE IS HIDING IN THE BATHROOM?! I'M COMING!\"\n";
	startAlert(BossState::SEARCHING_BATHROOM, route);
}

void Boss::startAlert(BossState state, const std::vector<Room*>& route)
{
	m_state = state;
	m_reactionTurns = REACTION_TURNS;
	m_alertStep = 0;
	m_activeRoute = route;
}

void Boss::onPlayerSpotted(const Player* /*player*/)
{
	m_hasCaughtPlayer = true;
}

void Boss::update()
{
	if (m_state == BossState::CALM)
	{
		followRoute();
		return;
	}

	if (m_reactionTurns > 0)
	{
		--m_reactionTurns;
		return;
	}

	if (m_alertStep < m_activeRoute.size())
	{
		moveTo(m_activeRoute[m_alertStep]);
		++m_alertStep;
	}
	else
	{
		// Route finished: he calms down and goes back to his usual route
		m_state = BossState::CALM;
		m_alertStep = 0;
	}
}

std::string Boss::describePresence() const
{
	if (isAlerted())
	{
		return "THE BOSS IS HERE, AND HE IS FURIOUS!";
	}
	return NPC::describePresence();
}

std::string Boss::talk() const
{
	if (isAlerted())
	{
		return "WHERE DO YOU THINK YOU ARE GOING?!";
	}
	return "Ah, perfect timing! Do you have five minutes?";
}
