#pragma once

#include "creature.h"
#include <string>
#include <vector>

class Player;
class Room;


class NPC : public Creature
{
public:
	NPC(const std::string& name, const std::string& description,
		const std::vector<Room*>& route, Entity* favoriteItem = nullptr);

	// True if the player is in the same room as this NPC
	bool spottedPlayer(const Player* player) const;

	virtual void onPlayerSpotted(const Player* player);
	virtual std::string talk() const;
	// Called after the player talks to this NPC
	virtual void onTalkedTo(Player* player);

	virtual std::string describePresence() const;
	virtual bool receiveItem(Entity* item);
	Entity* getFavoriteItem() const;

	Room* getNextRoom() const;
	void update() override;

protected:

	void followRoute();
	virtual void onBathroomBlocked(Room* bathroom, int tries);

private:
	const std::vector<Room*> m_route;
	size_t m_routeStep = 0;
	int m_bathroomTries = 0;

	Entity* const m_favoriteItem;
};

