#include "creature.h"
#include "room.h"

Creature::Creature(const std::string& name, const std::string& description)
	: Entity(EntityType::CREATURE, name, description)
{
}

Room* Creature::getCurrentRoom() const
{
	Entity* location = getLocation();

	if (location != nullptr && location->getType() == EntityType::ROOM)
	{
		return static_cast<Room*>(location);
	}

	return nullptr;
}

bool Creature::move(Direction direction)
{
	Room* room = getCurrentRoom();
	if (room == nullptr)
	{
		return false;
	}

	Exit* exit = room->getExit(direction);
	if (exit == nullptr)
	{
		return false;
	}

	return moveTo(exit->getDestination());
}
