#pragma once

#include "npc.h"

class Boss;

// A friendly coworker. Gives a tip and tells where the boss is and where he goes next
class Friendly : public NPC
{
public:
	Friendly(const std::string& name, const std::string& description,
		const std::vector<Room*>& route, const std::string& tip, const Boss* boss);

	std::string talk() const override;

private:
	const std::string m_tip;
	const Boss* const m_boss;
};
