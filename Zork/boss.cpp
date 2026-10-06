#include "boss.h"
#include "room.h"
#include <iostream>

Boss::Boss(const std::string& name,
    const std::string& description,
    const std::vector<Room*>& route,
    const std::vector<Room*>& alertRoute,
    const std::vector<Room*>& smokeRoute,
    Entity* favoriteItem)
    : NPC(name, description, route, favoriteItem),
    m_alertRoute(alertRoute), m_smokeRoute(smokeRoute)
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

void Boss::onPlayerSpotted(const Player* )
{
	m_hasCaughtPlayer = true;
}

void Boss::update()
{
    // While following a special route (alerted or smoking) he ignores his usual one
    if (m_state != BossState::CALM)
    {
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
            m_state = BossState::CALM;
            m_alertStep = 0;
        }

        return;
    }

    // He can't resist his favorite item: if someone left the cigars in his way, he goes outside to smoke
    Room* room = getCurrentRoom();
    Entity* favorite = getFavoriteItem();

    if (room != nullptr && favorite != nullptr && room->contains(favorite))
    {
        std::cout << "You hear the boss: \"A box of Cuban cigars?! Well, five minutes outside won't hurt...\"\n";
        favorite->moveTo(this);
        startAlert(BossState::SMOKING_OUTSIDE, m_smokeRoute);
        return;
    }

    followRoute();
}

std::string Boss::talk() const
{
	if (isAlerted())
	{
		return "WHERE DO YOU THINK YOU ARE GOING?!";
	}
	return "Ah, perfect timing! Do you have five minutes?";
}


// When alerted, the boss is impossible to miss
std::string Boss::describePresence() const
{
	if (isAlerted())
	{
		return "THE BOSS IS HERE, AND HE IS FURIOUS!";
	}
	return NPC::describePresence();
}
