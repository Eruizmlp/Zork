#include "npc.h"
#include "player.h"
#include "room.h"

NPC::NPC(const std::string& name, const std::string& description, const std::vector<Room*>& route)
	: Creature(name, description), m_route(route)
{
}

bool NPC::spottedPlayer(const Player* player) const
{
	if (player == nullptr)
	{
		return false;
	}

	return getCurrentRoom() == player->getCurrentRoom();
}

void NPC::onPlayerSpotted()
{	
}

std::string NPC::talk() const
{
	return "Hi! Busy day, huh?";
}

bool NPC::receiveItem(Entity* /*item*/)
{
	return false;
}

void NPC::update()
{
	followRoute();
}

// Goes to the next room of the route, back to the first one after the last
void NPC::followRoute()
{
	if (m_route.empty())
	{
		return;
	}

	m_routeStep = (m_routeStep + 1) % m_route.size();
	moveTo(m_route[m_routeStep]);
}