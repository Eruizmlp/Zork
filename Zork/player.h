#pragma once

#include "creature.h"
#include <string>

// The character controlled by the user.
class Player : public Creature
{
public:
	Player(const std::string& name, const std::string& description);

	// Describes the current room: name, description, people, items and exits
	void look() const;
	void inventory() const;

	bool take(const std::string& itemName); //Puts items in the backpack
	bool drop(const std::string& itemName);

	// TODO: talk to NPCs, put items inside containers

	void update() override;
};
