#include "snitch.h"
#include "boss.h"
#include <iostream>

Snitch::Snitch(const std::string& name, const std::string& description,
	const std::vector<Room*>& route, Boss* boss)
	: NPC(name, description, route), m_boss(boss)
{
}

void Snitch::onPlayerSpotted()
{
	if (!m_isBribed && !m_boss->isAlerted())
	{
		std::cout << getName() << " sees you sneaking around and runs to tell the boss!\n";
		m_boss->alert();
	}
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