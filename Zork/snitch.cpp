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
	if (!m_boss->isAlerted())
	{
		std::cout << getName() << " sees you sneaking around and runs to tell the boss!\n";
		m_boss->alert();
	}
}