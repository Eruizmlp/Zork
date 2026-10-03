#include "boss.h"

Boss::Boss(const std::string& name, const std::string& description, const std::vector<Room*>& route)
	: NPC(name, description, route)
{
}

void Boss::onPlayerSpotted()
{
	m_hasCaughtPlayer = true;
}