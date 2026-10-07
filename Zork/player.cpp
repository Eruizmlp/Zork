#include "player.h"
#include "room.h"
#include "item.h"
#include "npc.h"
#include <iostream>

Player::Player(const std::string& name, const std::string& description)
	: Creature(name, description)
{
}

// The player can only walk through an exit of the current room
MoveResult Player::move(Direction direction)
{
	Room* room = getCurrentRoom();
	if (room == nullptr)
	{
		return MoveResult::NO_EXIT;
	}

	const Exit* exit = room->getExit(direction);
	if (exit == nullptr)
	{
		return MoveResult::NO_EXIT;
	}

	if (!exit->canPass(this))
	{
		return MoveResult::LOCKED;
	}

	moveTo(exit->getDestination());
	return MoveResult::MOVED;
}

// Displays everything the player can see
void Player::look() const
{
	const Room* room = getCurrentRoom();
	if (room == nullptr)
	{
		return;
	}

	std::cout << "\n--- " << room->getName() << " ---\n"
		<< room->getDescription() << "\n";

	// Every creature in a room except the player is an NPC
	bool someoneHere = false;
	for (const Entity* creature : room->getCreatures())
	{
		if (creature != this)
		{
			if (!someoneHere)
			{
				std::cout << "\n";
				someoneHere = true;
			}
			std::cout << static_cast<const NPC*>(creature)->describePresence() << "\n";
		}
	}

	const std::vector<Entity*> items = room->getItems();
	if (!items.empty())
	{
		std::cout << "\nYou see: ";
		for (size_t i = 0; i < items.size(); ++i)
		{
			if (i > 0)
			{
				std::cout << ", ";
			}
			std::cout << items[i]->getName();
		}
		std::cout << ".\n";
	}

	std::cout << "\nExits:\n";
	for (const Exit* exit : room->getExits())
	{
		std::cout << "  " << directionToString(exit->getDirection()) << ": " << exit->getDescription() << "\n";
	}
}

// Lists what the player carries, one item per line, including what is inside each container
void Player::inventory() const
{
	if (m_contains.empty())
	{
		std::cout << "You are not carrying anything.\n";
		return;
	}

	std::cout << "You are carrying:\n";
	for (const Entity* item : m_contains)
	{
		std::cout << "  - " << item->getName();

		const std::list<Entity*>& inside = item->getContains();
		if (!inside.empty())
		{
			std::cout << " (inside: ";
			bool first = true;
			for (const Entity* content : inside)
			{
				if (!first)
				{
					std::cout << ", ";
				}
				std::cout << content->getName();
				first = false;
			}
			std::cout << ")";
		}
		std::cout << "\n";
	}
}

// Picks up an item (in the current room)
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

// Puts an item the player is carrying inside a container
bool Player::put(const std::string& itemName, const std::string& containerName)
{
	Entity* item = findByName(itemName, EntityType::ITEM);
	if (item == nullptr)
	{
		std::cout << "You don't have any " << itemName << ".\n";
		return false;
	}

	Item* container = findContainer(containerName);
	if (container == nullptr)
	{
		std::cout << "There is no " << containerName << " you can put things in.\n";
		return false;
	}

	if (item == container)
	{
		std::cout << "You can't put the " << itemName << " inside itself.\n";
		return false;
	}

	item->moveTo(container);
	std::cout << "You put the " << itemName << " in the " << containerName << ".\n";
	return true;
}

// Takes an item out of a container
bool Player::takeFrom(const std::string& itemName, const std::string& containerName)
{
	Item* container = findContainer(containerName);
	if (container == nullptr)
	{
		std::cout << "There is no " << containerName << " you can take things from.\n";
		return false;
	}

	Entity* item = container->findByName(itemName, EntityType::ITEM);
	if (item == nullptr)
	{
		std::cout << "There is no " << itemName << " in the " << containerName << ".\n";
		return false;
	}

	item->moveTo(this);
	std::cout << "You take the " << itemName << " from the " << containerName << ".\n";
	return true;
}

bool Player::wait()
{
	const Room* room = getCurrentRoom();
	if (room == nullptr || room->getRoomType() != RoomType::BATHROOM)
	{
		std::cout << "You can only wait inside the bathroom.\n";
		return false;
	}

	std::cout << "You wait on the toilet.\n";
	return true;
}

Item* Player::findContainer(const std::string& containerName) const
{
	Entity* found = findByName(containerName, EntityType::ITEM);

	const Room* room = getCurrentRoom();
	if (found == nullptr && room != nullptr)
	{
		found = room->findByName(containerName, EntityType::ITEM);
	}

	if (found == nullptr)
	{
		return nullptr;
	}

	Item* item = static_cast<Item*>(found);
	if (!item->isContainer())
	{
		return nullptr;
	}

	return item;
}

// Asking an NPC for information doesn't take a turn
void Player::talkTo(const std::string& npcName)
{
	NPC* npc = findNPCHere(npcName);
	if (npc == nullptr)
	{
		std::cout << "There is nobody called " << npcName << " here.\n";
		return;
	}

	printSpeech(npc);
	npc->onTalkedTo(this);
}

// Gives an item the player has in hand to an NPC in the same room
bool Player::give(const std::string& itemName, const std::string& npcName)
{
	Entity* item = findByName(itemName, EntityType::ITEM);
	if (item == nullptr)
	{
		std::cout << "You don't have any " << itemName << " in your hands.\n";
		return false;
	}

	NPC* npc = findNPCHere(npcName);
	if (npc == nullptr)
	{
		std::cout << "There is nobody called " << npcName << " here.\n";
		return false;
	}

	if (!npc->receiveItem(item))
	{
		std::cout << npc->getName() << " doesn't want the " << itemName << ".\n";
		return false;
	}

	std::cout << "You give the " << itemName << " to " << npc->getName() << ".\n";
	printSpeech(npc);
	return true;
}

// Finds an NPC in the current room by name (skipping the player, who is also a CREATURE)
NPC* Player::findNPCHere(const std::string& npcName) const
{
	const Room* room = getCurrentRoom();
	if (room == nullptr)
	{
		return nullptr;
	}

	Entity* found = room->findByName(npcName, EntityType::CREATURE);
	if (found == nullptr || found == this)
	{
		return nullptr;
	}

	return static_cast<NPC*>(found);
}

// Shows what an NPC says, split in lines of at most LINE_WIDTH characters so it is easy to read
void Player::printSpeech(const NPC* npc) const
{
	std::cout << "\n" << npc->getName() << " says:\n";

	const std::string text = "\"" + npc->talk() + "\"";
	std::string line;
	std::string word;

	for (size_t i = 0; i <= text.size(); ++i)
	{
		if (i < text.size() && text[i] != ' ')
		{
			word += text[i];
			continue;
		}

		// A word has ended: start a new line if it does not fit in the current one
		if (!line.empty() && line.size() + 1 + word.size() > LINE_WIDTH)
		{
			std::cout << "  " << line << "\n";
			line.clear();
		}

		if (!line.empty())
		{
			line += " ";
		}
		line += word;
		word.clear();
	}

	std::cout << "  " << line << "\n";
}

void Player::update()
{
}

