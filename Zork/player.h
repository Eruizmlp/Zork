#pragma once

#include "creature.h"
#include <string>

class Item;

// The character controlled by the user
class Player : public Creature
{
public:
	Player(const std::string& name, const std::string& description);

	// Describes the current room: name, description, people, items and exits
	//DOES NOT CONSUME TURNS!
	void look() const;
	void inventory() const;

	// Each action returns true if it succeeded and all of them consume turns
	bool take(const std::string& itemName);
	bool drop(const std::string& itemName);
	bool put(const std::string& itemName, const std::string& containerName);
	bool takeFrom(const std::string& itemName, const std::string& containerName);

	// TODO: talk to NPCs

	void update() override;

private:
	Item* findContainer(const std::string& containerName) const;
};
