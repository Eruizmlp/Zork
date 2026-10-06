#pragma once

#include "npc.h"

class Boss;

/*A coworker who warns the boss when he sees the player carrying a suspicious item
 in hand (items inside a container cannot be seen), unless he has been bribed*/

class Snitch : public NPC
{
public:
	Snitch(const std::string& name, const std::string& description,
		const std::vector<Room*>& route, Boss* boss, const Entity* suspiciousItem);

	void onPlayerSpotted(const Player* player) override;
	std::string talk() const override;
	bool receiveItem(Entity* item) override;

private:
	Boss* const m_boss;
	const Entity* const m_suspiciousItem;
	bool m_isBribed = false;
};
