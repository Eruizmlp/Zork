#pragma once

#include "creature.h"

// The character controlled by the user.
class Player : public Creature
{
public:
	Player(const std::string& name, const std::string& description);

	// Describes the current room: name, description, people, items and exits
	void look() const;

	//TODO:
	void talkToNPC() const;
	void takeItem();
	void dropItem();

	void update() override;
};
