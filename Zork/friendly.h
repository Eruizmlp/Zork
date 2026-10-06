#pragma once

#include "npc.h"

class Boss;

class Friendly : public NPC
{
public:
	Friendly(const std::string& name, const std::string& description,
		const std::vector<Room*>& route, const std::string& tip, Boss* boss,
		const Entity* wantedItem = nullptr, const std::string& secret = "");

	std::string talk() const override;
	bool receiveItem(Entity* item) override;

protected:
	// Knocks on the door and, after BATHROOM_PATIENCE turns, tells the boss someone is hiding
	void onBathroomBlocked(Room* bathroom, int tries) override;

private:
	bool isNextTo(const Room* room) const;

	const std::string m_tip;
	static const int BATHROOM_PATIENCE = 3;

	Boss* const m_boss;

	const Entity* const m_wantedItem;
	const std::string m_secret;
	bool m_hasGift = false;
};
