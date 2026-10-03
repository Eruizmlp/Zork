#pragma once

#include "entity.h"

class Room;

enum class Direction
{
	NORTH,
	SOUTH,
	EAST,
	WEST
};

// Text shown to the player for a direction
std::string directionToString(Direction direction);

class Exit : public Entity
{
public:
	Exit(const std::string& name, const std::string& description,
		Direction direction, Room* source, Room* destination);

	Direction getDirection() const { return m_direction; }
	Room* getSource() const { return m_source; }
	Room* getDestination() const { return m_destination; }

	void update() override;

private:
	const Direction m_direction;
	Room* const m_source;
	Room* const m_destination;
};
