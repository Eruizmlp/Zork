#pragma once

#include "npc.h"

class Boss;

// A coworker on the player's side: gives a tip and tells where the boss is and where he goes next.
class Friendly : public NPC
{
public:
	Friendly(const std::string& name, const std::string& description,
		const std::vector<Room*>& route, const std::string& tip, Boss* boss,
		Entity* favoriteItem = nullptr, const std::string& secret = "", Entity* giftItem = nullptr);

	std::string talk() const override;
	bool receiveItem(Entity* item) override;
	void onTalkedTo(Player* player) override;

protected:

	void onBathroomBlocked(Room* bathroom, int tries) override;

private:
	bool isNextTo(const Room* room) const;

	const std::string m_tip;
	static const int BATHROOM_PATIENCE = 3; //If it gets to 0, alerts the boss

	Boss* const m_boss;

	Entity* const m_giftItem;
	const std::string m_secret;
	bool m_hasGift = false;
};

