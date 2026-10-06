#pragma once

#include "npc.h"

class Boss;

// A coworker who warns the boss when he sees the player carrying a suspicious item
class Snitch : public NPC
{
public:
	Snitch(const std::string& name, const std::string& description,
		const std::vector<Room*>& route, Boss* boss, const std::vector<const Entity*>& suspiciousItems,
		Entity* favoriteItem);

	void onPlayerSpotted(const Player* player) override;
	std::string talk() const override;
	bool receiveItem(Entity* item) override;

private:
	// Returns the first suspicious item the player has in hand, or nullptr
	const Entity* findSuspiciousItem(const Player* player) const;

	Boss* const m_boss;
	const std::vector<const Entity*> m_suspiciousItems;
	bool m_isBribed = false;
};

