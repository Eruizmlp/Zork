#pragma once

#include "creature.h"
#include <string>
#include <vector>

class Player;
class Room;

// Subclasses (Boss, Snitch, Friendly) redefine how they react, what they say and what they accept.
class NPC : public Creature
{
public:
	NPC(const std::string& name, const std::string& description, const std::vector<Room*>& route);

	// True if the player is in the same room as this NPC
	bool spottedPlayer(const Player* player) const;

	virtual void onPlayerSpotted(const Player* player);
	virtual std::string talk() const;

	virtual std::string describePresence() const;
	virtual bool receiveItem(Entity* item);

	Room* getNextRoom() const;
	void update() override;

protected:

	void followRoute();
	virtual void onBathroomBlocked(Room* bathroom, int tries);

private:
	const std::vector<Room*> m_route;
	size_t m_routeStep = 0;
	int m_bathroomTries = 0;
};
