#pragma once

#include <string>
#include <vector>
#include "parser.h"

class Entity;
class Room;
class Item;
class Player;
class NPC;
class Boss;
enum class RoomType;

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
	Item* m_keys = nullptr;
	std::vector<NPC*> m_npcs;
	Boss* m_boss = nullptr;

	// Each action consumes a turn the player must escape before MAX_TURNS
	int m_currTurn = 0;
	static const int MAX_TURNS = 20;

	// Set to true by the quit command
	bool m_gameOver = false;

	Parser m_parser;

	void printIntro() const;
	bool readLine(std::string& line) const;
	void playTurn(const std::string& line);
	void printEnding() const;

	// Returns true if it's a command that advances a turn
	bool tryAction(const Command& command);
	void update();
	void checkNPCs();
	bool isPlayerHidden() const;

	bool hasWon() const;
	bool isCarrying(const Entity* item) const;
	bool hasRunOutOfTurns() const;
	bool isGameOver() const;

	void createWorld();
	Room* addRoom(const std::string& name, const std::string& description, RoomType roomType);
	Item* addItem(const std::string& name, const std::string& description,
		Entity* location, bool isContainer = false);
	void addNPC(NPC* npc, Room* startRoom);
	void addExit(const std::string& name, const std::string& description,
		Direction direction, Room* source, Room* destination, const Entity* key = nullptr);
};

