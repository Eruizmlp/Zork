#include "snitch.h"
#include "boss.h"
#include "player.h"
#include <iostream>

Snitch::Snitch(const std::string& name, const std::string& description,
	const std::vector<Room*>& route, Boss* boss, const std::vector<const Entity*>& suspiciousItems,
	Entity* favoriteItem)
	: NPC(name, description, route, favoriteItem), m_boss(boss), m_suspiciousItems(suspiciousItems)
{
}

void Snitch::onPlayerSpotted(const Player* player)
{
	if (m_isBribed || m_boss->isAlerted())
	{
		return;
	}

	const Entity* seenItem = findSuspiciousItem(player);
	if (seenItem == nullptr)
	{
		return;
	}

	std::cout << getName() << " notices the " << seenItem->getName()
		<< " in your hands and runs to tell the boss!\n";
	m_boss->alert();
}

const Entity* Snitch::findSuspiciousItem(const Player* player) const
{
	for (const Entity* item : m_suspiciousItems)
	{
		if (player->contains(item))
		{
			return item;
		}
	}

	return nullptr;
}

std::string Snitch::talk() const
{
	if (m_isBribed)
	{
		return "Mmm, chocolate... I haven't seen anything.";
	}
	return "Leaving already? Interesting... very interesting.";
}

bool Snitch::receiveItem(Entity* item)
{
	if (item == nullptr || item != getFavoriteItem())
	{
		return false;
	}

	item->moveTo(this);
	m_isBribed = true;
	return true;
}

