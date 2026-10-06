#pragma once

#include "npc.h"

// The boss walks his usual route. When a snitch alerts him, he follows his alert route

class Boss : public NPC
{
public:
	Boss(const std::string& name, const std::string& description,
		const std::vector<Room*>& route, const std::vector<Room*>& alertRoute);

	// He needs a turn to react (finish his coffee) before rushing off
	void alert();
	bool isAlerted() const { return m_isAlerted; }
	bool hasCaughtPlayer() const { return m_hasCaughtPlayer; }

	void onPlayerSpotted(const Player* player) override;
	std::string talk() const override;
	void update() override;

private:
	bool m_isAlerted = false;
	bool m_hasCaughtPlayer = false;

	static const int REACTION_TURNS = 1;
	int m_reactionTurns = 0;

	// Path followed once alerted; it ends in a room next to every room of his usual route
	const std::vector<Room*> m_alertRoute;
	size_t m_alertStep = 0;
};

