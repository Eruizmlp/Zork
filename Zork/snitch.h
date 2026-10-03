#pragma once

#include "npc.h"

class Boss;

// A coworker who warns the boss as soon as he sees the player
class Snitch : public NPC
{
public:
	Snitch(const std::string& name, const std::string& description,
		const std::vector<Room*>& route, Boss* boss);

	void onPlayerSpotted() override;

private:
	Boss* const m_boss;
};