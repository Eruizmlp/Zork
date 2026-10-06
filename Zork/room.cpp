#include "room.h"

Room::Room(const std::string& name, const std::string& description, RoomType roomType)
	: Entity(EntityType::ROOM, name, description), m_roomType(roomType)
{
}

// Looks for an exit in this room going in the given direction
Exit* Room::getExit(Direction direction) const
{
	for (Entity* entity : m_contains)
	{
		if (entity->getType() == EntityType::EXIT)
		{
			Exit* exit = static_cast<Exit*>(entity);
			if (exit->getDirection() == direction)
			{
				return exit;
			}
		}
	}

	return nullptr;
}

std::vector<Exit*> Room::getExits() const
{
	std::vector<Exit*> allExits;

	for (Entity* entity : m_contains)
	{
		if (entity->getType() == EntityType::EXIT)
		{
			allExits.push_back(static_cast<Exit*>(entity));
		}
	}

	return allExits;
}

std::vector<Entity*> Room::getCreatures() const
{
	std::vector<Entity*> allCreatures;

	for (Entity* entity : m_contains)
	{
		if (entity->getType() == EntityType::CREATURE)
		{
			allCreatures.push_back(entity);
		}
	}

	return allCreatures;
}

std::vector<Entity*> Room::getItems() const
{
	std::vector<Entity*> allItems;

	for (Entity* entity : m_contains)
	{
		if (entity->getType() == EntityType::ITEM)
		{
			allItems.push_back(entity);
		}
	}

	return allItems;
}

void Room::update()
{
}
