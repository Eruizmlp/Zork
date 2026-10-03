#include "exit.h"

Exit::Exit(const std::string& name, const std::string& description,
	Direction direction, Room* source, Room* destination)
	: Entity(EntityType::EXIT, name, description),
	m_direction(direction), m_source(source), m_destination(destination)
{
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
