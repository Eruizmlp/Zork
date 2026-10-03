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
	// key: item needed to go through this exit, nullptr means its open
	Exit(const std::string& name, const std::string& description,
		Direction direction, Room* source, Room* destination, const Entity* key = nullptr);

	Direction getDirection() const { return m_direction; }
	Room* getSource() const { return m_source; }
	Room* getDestination() const { return m_destination; }
	bool isLocked() const { return m_key != nullptr; }

	// True if the exit is open, or if the player carries the key
	bool canPass(const Entity* player) const;

	void update() override;

private:
	const Direction m_direction;
	Room* const m_source;
	Room* const m_destination;
	const Entity* const m_key;
};
