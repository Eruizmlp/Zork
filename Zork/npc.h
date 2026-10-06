#pragma once

#include "creature.h"
#include <string>
#include <vector>

class Player;
class Room;


class NPC : public Creature
{
public:
	NPC(const std::string& name, const std::string& description, const std::vector<Room*>& route);

	// True if the player is in the same room as this NPC
	bool spottedPlayer(const Player* player) const;
	
	virtual void onPlayerSpotted();
	virtual std::string talk() const;
	virtual bool receiveItem(Entity* item);

	void update() override;

protected:
	void followRoute();

private:
	const std::vector<Room*> m_route;
	size_t m_routeStep = 0;
};