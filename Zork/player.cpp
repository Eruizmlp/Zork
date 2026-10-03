#include "player.h"
#include "room.h"
#include <iostream>

Player::Player(const std::string& name, const std::string& description)
	: Creature(name, description)
{
}

// Displays everything the player can see
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

void Player::inventory() const
{
	if (m_contains.empty())
	{
		std::cout << "You are not carrying anything.\n";
		return;
	}

	std::cout << "You are carrying:";
	for (const Entity* item : m_contains)
	{
		std::cout << " " << item->getName();
	}
	std::cout << "\n";
}

// Picks up an item lying in the current room
bool Player::take(const std::string& itemName)
{
	Room* room = getCurrentRoom();
	if (room == nullptr)
	{
		return false;
	}

	Entity* item = room->findByName(itemName, EntityType::ITEM);
	if (item == nullptr)
	{
		std::cout << "There is no " << itemName << " here.\n";
		return false;
	}

	item->moveTo(this);
	std::cout << "You take the " << itemName << ".\n";
	return true;
}

// Leaves an item the player is carrying in the current room
bool Player::drop(const std::string& itemName)
{
	Room* room = getCurrentRoom();
	Entity* item = findByName(itemName, EntityType::ITEM);

	if (item == nullptr || room == nullptr)
	{
		std::cout << "You don't have any " << itemName << ".\n";
		return false;
	}

	item->moveTo(room);
	std::cout << "You drop the " << itemName << ".\n";
	return true;
}

void Player::update()
{
}
