#pragma once

#include <string>
#include <vector>
#include "parser.h"

class Entity;
class Room;
class Player;
class NPC;
class Boss;

// Owns every entity, builds the map and runs the game loop.
class World
{
public:
	World();
	~World();

	World(const World&) = delete;
	World& operator=(const World&) = delete;

	void runWorld();

private:
	std::vector<Entity*> m_entities;

	Player* m_player = nullptr;
	Room* m_street = nullptr;
	std::vector<NPC*> m_npcs; 
	Boss* m_boss = nullptr;

	// Each action consumes a turn the player must escape before MAX_TURNS
	int m_currTurn = 0;
	static const int MAX_TURNS = 15;

	// Set to true by the quit command
	bool m_gameOver = false;

	Parser m_parser;

	void createWorld();
	void addExit(const std::string& name, const std::string& description,
		Direction direction, Room* source, Room* destination, const Entity* key = nullptr);
	void executeCommand(const Command& command);


	void checkNPCs();
	void addNPC(NPC* npc, Room* startRoom);

	void update();
	bool hasWon() const;
	bool isGameOver() const;
};
