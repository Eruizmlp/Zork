#include "player.h"
#include "room.h"
#include <iostream>

Player::Player(const std::string& name, const std::string& description)
	: Creature(name, description)
{
}

//Display everything the player can see.
void Player::look() const
{
	const Room* room = getCurrentRoom();
	if (room == nullptr)
	{
		return;
	}

	std::cout << room->getName() << "\n" << room->getDescription() << "\n";

	for (const Entity* creature : room->getCreatures())
	{
		if (creature != this)
		{
			std::cout << creature->getName() << " is here.\n";
		}
	}

	const std::vector<Entity*> items = room->getItems();
	if (!items.empty())
	{
		std::cout << "You see:";
		for (const Entity* item : items)
		{
			std::cout << " " << item->getName();
		}
		std::cout << "\n";
	}

	for (const Exit* exit : room->getExits())
	{
		std::cout << exit->getDescription() << " (" << directionToString(exit->getDirection()) << ")\n";
	}
}

void Player::update()
{
}
