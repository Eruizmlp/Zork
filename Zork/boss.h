#pragma once

#include "npc.h"

class Boss : public NPC
{
public:
	Boss(const std::string& name, const std::string& description, const std::vector<Room*>& route);

	void alert() { m_isAlerted = true; }
	bool isAlerted() const { return m_isAlerted; }
	bool hasCaughtPlayer() const { return m_hasCaughtPlayer; }

	void onPlayerSpotted() override;

private:
	bool m_isAlerted = false;
	bool m_hasCaughtPlayer = false;
};