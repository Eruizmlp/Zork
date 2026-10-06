#pragma once

#include "entity.h"

class Room;

// Anything that lives in a room and can move around: the player and the NPCs
class Creature : public Entity
{
public:
	Creature(const std::string& name, const std::string& description);

	Room* getCurrentRoom() const;

};
