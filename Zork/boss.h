#pragma once

#include "npc.h"

enum class BossState
{
	CALM,                // walking his usual route
	GUARDING_ENTRANCE,   // a snitch told him someone is leaving
	SEARCHING_BATHROOM,  // someone told him the player is hiding in the bathroom
	SMOKING_OUTSIDE      // he found a box of cigars and went out to the street to smoke one
};

class Boss : public NPC
{
public:
	Boss(const std::string& name,
		const std::string& description,
		const std::vector<Room*>& route,
		const std::vector<Room*>& alertRoute,
		const std::vector<Room*>& smokeRoute,
		Entity* favoriteItem);

	// A snitch told him someone is leaving: he goes to guard the entrance
	void alert();
	// Someone told him where the player is hiding: he goes to check that room
	void search(const std::vector<Room*>& route);

	BossState getState() const { return m_state; }
	// Alerted means he is looking for the player (smoking outside doesn't count)
	bool isAlerted() const
	{
		return m_state == BossState::GUARDING_ENTRANCE || m_state == BossState::SEARCHING_BATHROOM;
	}
	bool hasCaughtPlayer() const { return m_hasCaughtPlayer; }

	void onPlayerSpotted(const Player* player) override;
	std::string talk() const override;
	std::string describePresence() const override;
	void update() override;

private:
	BossState m_state = BossState::CALM;
	bool m_hasCaughtPlayer = false;

	// He needs a turn to react
	static const int REACTION_TURNS = 1;
	int m_reactionTurns = 0;

	// Path to guard the entrance
	const std::vector<Room*> m_alertRoute;

	// Path to the street and back when he finds his favorite item (the cigars)
	const std::vector<Room*> m_smokeRoute;

	// Route he is following while alerted 
	std::vector<Room*> m_activeRoute;
	size_t m_alertStep = 0;

	void startAlert(BossState state, const std::vector<Room*>& route);
};

