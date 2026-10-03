#pragma once

#include "entity.h"
#include "exit.h"

class Room;


class Creature : public Entity
{
public:
	Creature(const std::string& name, const std::string& description);

	Room* getCurrentRoom() const;

};
