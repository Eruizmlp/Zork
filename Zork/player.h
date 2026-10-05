#pragma once

#include "creature.h"
#include "exit.h"
#include <string>

class Item;

// Outcome of the player trying to walk in a direction
enum class MoveResult
{
	MOVED,
	NO_EXIT,
	LOCKED
};

// The character controlled by the user
class Player : public Creature
{
public:
	Player(const std::string& name, const std::string& description);

	// Moves through an exit and returns the outcome
	MoveResult move(Direction direction);

	// Describe the current room and the inventory. They don't consume turns!
	void look() const;
	void inventory() const;

	// Each action returns true if it succeeded. Successful actions consume a turn!
	bool take(const std::string& itemName);
	bool drop(const std::string& itemName);
	bool put(const std::string& itemName, const std::string& containerName);
	bool takeFrom(const std::string& itemName, const std::string& containerName);
	bool wait(); //You can only wait on the bathroom
	
	// TODO: talk to NPCs

	void update() override;

private:
	Item* findContainer(const std::string& containerName) const;
};