#include "friendly.h"
#include "boss.h"
#include "room.h"

Friendly::Friendly(const std::string& name, const std::string& description,
	const std::vector<Room*>& route, const std::string& tip, const Boss* boss)
	: NPC(name, description, route), m_tip(tip), m_boss(boss)
{
}

std::string Friendly::talk() const
{
	std::string answer = m_tip;

	if (m_boss->isAlerted())
	{
		return answer + " And watch out, the boss is in a hurry. I don't know where he's going!";
	}

	const Room* bossRoom = m_boss->getCurrentRoom();
	if (bossRoom == nullptr)
	{
		return answer;
	}

	answer += " The boss is in the " + bossRoom->getName() + " right now";

	const Room* nextRoom = m_boss->getNextRoom();
	if (nextRoom != nullptr && nextRoom != bossRoom)
	{
		answer += ", and I think he is going to the " + nextRoom->getName() + " next";
	}

	return answer + ".";
}
