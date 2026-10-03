#pragma once
#include "creature.h"

//AI Creatures roaming through the office
//TODO: Different types of NPCs: Friendly/snitch/Boss
enum class NPCType
{
	FRIENDLY,
	SNITCH,
	BOSS

};

class NPC : public Creature
{
public:

	void update();
private:
	NPCType m_type;

};
