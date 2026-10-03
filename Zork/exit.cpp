#include "exit.h"

Exit::Exit(const std::string& name, const std::string& description,
	Direction direction, Room* source, Room* destination, const Entity* key)
	: Entity(EntityType::EXIT, name, description),
	m_direction(direction), m_source(source), m_destination(destination), m_key(key)
{
}

// True if the exit is open, or if the player carries the key in hand
bool Exit::canPass(const Entity* player) const
{
	return m_key == nullptr || player->contains(m_key);
}

void Exit::update()
{
}

std::string directionToString(Direction direction)
{
	switch (direction)
	{
	case Direction::NORTH:
		return "north";
	case Direction::SOUTH:
		return "south";
	case Direction::EAST:
		return "east";
	case Direction::WEST:
		return "west";
	}
	return "";
}
