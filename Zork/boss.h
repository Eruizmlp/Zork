#pragma once

#include "npc.h"

// What the boss is doing right now
enum class BossState
{
	CALM,                // walking his usual route
	GUARDING_ENTRANCE,   // a snitch told him someone is leaving
	SEARCHING_BATHROOM   // someone told him the player is hiding in the bathroom
};

// The boss walks his usual route. When someone alerts him, he follows an alert route
class Boss : public NPC
{
public:
	Boss(const std::string& name, const std::string& description,
		const std::vector<Room*>& route, const std::vector<Room*>& alertRoute);

	// A snitch told him someone is leaving: he goes to guard the entrance
	void alert();
	// Someone told him where the player is hiding: he goes to check that room
	void search(const std::vector<Room*>& route);

	BossState getState() const { return m_state; }
	bool isAlerted() const { return m_state != BossState::CALM; }
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

	// Path to guard the entrance;
	const std::vector<Room*> m_alertRoute;

	// Route he is following while alerted 
	std::vector<Room*> m_activeRoute;
	size_t m_alertStep = 0;

	void startAlert(BossState state, const std::vector<Room*>& route);
};
