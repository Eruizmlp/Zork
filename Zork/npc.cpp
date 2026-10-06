#include "npc.h"
#include "player.h"
#include "room.h"

NPC::NPC(const std::string& name, const std::string& description,
	const std::vector<Room*>& route, Entity* favoriteItem)
	: Creature(name, description), m_route(route), m_favoriteItem(favoriteItem)
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

void NPC::onPlayerSpotted(const Player* /*player*/)
{
}

std::string NPC::describePresence() const
{
	return getName() + " is here.";
}

void NPC::onTalkedTo(Player* /*player*/)
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

Entity* NPC::getFavoriteItem() const
{
	return m_favoriteItem;
}

Room* NPC::getNextRoom() const
{
	if (m_route.empty())
	{
		return nullptr;
	}

	return m_route[(m_routeStep + 1) % m_route.size()];
}

void NPC::update()
{
	followRoute();
}

// Goes to the next room of the route, back to the first one after the last.
// If the next room is the bathroom and someone is inside, the NPC waits at the door.
void NPC::followRoute()
{
	if (m_route.empty())
	{
		return;
	}

	Room* nextRoom = m_route[(m_routeStep + 1) % m_route.size()];
	if (nextRoom->getRoomType() == RoomType::BATHROOM && !nextRoom->getCreatures().empty())
	{
		++m_bathroomTries;
		onBathroomBlocked(nextRoom, m_bathroomTries);
		return;
	}

	m_bathroomTries = 0;
	m_routeStep = (m_routeStep + 1) % m_route.size();
	moveTo(nextRoom);
}

void NPC::onBathroomBlocked(Room* /*bathroom*/, int /*tries*/)
{
}

