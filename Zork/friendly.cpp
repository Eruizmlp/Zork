#include "friendly.h"
#include "boss.h"
#include "room.h"
#include "player.h"
#include <iostream>

Friendly::Friendly(const std::string& name, const std::string& description,
	const std::vector<Room*>& route, const std::string& tip, Boss* boss,
	Entity* favoriteItem, const std::string& secret, Entity* giftItem)
	: NPC(name, description, route, favoriteItem), m_tip(tip), m_boss(boss),
	m_giftItem(giftItem), m_secret(secret)
{
}

void Friendly::onBathroomBlocked(Room* bathroom, int tries)
{
	if (tries < BATHROOM_PATIENCE)
	{
		std::cout << "Someone knocks on the bathroom door... (" << tries << "/" << BATHROOM_PATIENCE << ")\n";
	}
	else if (tries == BATHROOM_PATIENCE)
	{
		std::cout << getName() << " shouts: \"I'VE BEEN WAITING FOREVER! I'M TELLING THE BOSS SOMEONE IS HIDING IN THERE!\"\n";

		Room* door = getCurrentRoom();
		m_boss->search({ door, bathroom, door });
	}
}

bool Friendly::receiveItem(Entity* item)
{
	if (item == nullptr || item != getFavoriteItem())
	{
		return false;
	}

	item->moveTo(this);
	m_hasGift = true;
	return true;
}


void Friendly::onTalkedTo(Player* player)
{
	if (m_giftItem == nullptr || !contains(m_giftItem))
	{
		return;
	}

	m_giftItem->moveTo(player);
	std::cout << getName() << " hands you the " << m_giftItem->getName() << ".\n";
}

std::string Friendly::talk() const
{
	// Once they get what they wanted, they share their secret instead of the usual tip
	std::string answer = m_tip;
	if (m_hasGift)
	{
		answer = m_secret;
	}

	// The news about the boss changes depending on what he is doing
	switch (m_boss->getState())
	{
	case BossState::SEARCHING_BATHROOM:
		return answer + " AND GET AWAY FROM THE BATHROOM! THE BOSS KNOWS SOMEONE IS HIDING THERE!";
	case BossState::GUARDING_ENTRANCE:
		return answer + " AND RUN! SOMEONE TOLD THE BOSS AND HE'S GUARDING THE ENTRANCE!";
	case BossState::SMOKING_OUTSIDE:
		return answer + " The boss went outside to smoke a cigar, right in front of the door. Don't even think about leaving now!";
	case BossState::CALM:
		break;
	}

	const Room* bossRoom = m_boss->getCurrentRoom();
	if (bossRoom == nullptr)
	{
		return answer;
	}

	if (isNextTo(bossRoom))
	{
		answer += " Shh! The boss is right next door, in the " + bossRoom->getName();
	}
	else
	{
		answer += " The boss is in the " + bossRoom->getName() + " right now";
	}

	const Room* nextRoom = m_boss->getNextRoom();
	if (nextRoom != nullptr && nextRoom != bossRoom)
	{
		answer += ", and I think he is going to the " + nextRoom->getName() + " next";
	}

	return answer + ".";
}

// True if the given room is connected to the room this NPC is in
bool Friendly::isNextTo(const Room* room) const
{
	const Room* myRoom = getCurrentRoom();
	if (myRoom == nullptr)
	{
		return false;
	}

	for (const Exit* exit : myRoom->getExits())
	{
		if (exit->getDestination() == room)
		{
			return true;
		}
	}

	return false;
}

