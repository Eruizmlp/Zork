#include "snitch.h"
#include "boss.h"
#include "player.h"
#include <iostream>

Snitch::Snitch(const std::string& name, const std::string& description,
	const std::vector<Room*>& route, Boss* boss, const Entity* suspiciousItem)
	: NPC(name, description, route), m_boss(boss), m_suspiciousItem(suspiciousItem)
{
}

void Snitch::onPlayerSpotted(const Player* player)
{
	if (m_isBribed || m_boss->isAlerted() || !player->contains(m_suspiciousItem))
	{
		return;
	}

	std::cout << getName() << " notices the " << m_suspiciousItem->getName()
		<< " in your hands and runs to tell the boss!\n";
	m_boss->alert();
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
	if (item == nullptr || item->getName() != "chocolate")
	{
		return false;
	}

	item->moveTo(this);
	m_isBribed = true;
	return true;
}
