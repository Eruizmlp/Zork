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

	// Each action takes one minute; the player must escape before MAX_TURNS (when lunch ends)
	int m_currTurn = 0;
	static const int MAX_TURNS = 20;

	// The game starts at 13:40, so lunch ends at 14:00
	static const int START_MINUTES = 13 * 60 + 40;

	// Set to true by the quit command
	bool m_gameOver = false;

	Parser m_parser;

	//Print helpers
	void printIntro() const;
	void printHelp() const;
	void printEnding() const;
	void printTime() const;
	std::string clockText(int turn) const;

	//World logic
	bool readLine(std::string& line) const;
	void playTurn(const std::string& line);
	bool tryAction(const Command& command);  // Returns true if it's a command that advances a turn
	void update();
	void checkNPCs();
	void printReceptionSounds() const;
	void printAtmosphere() const;
	bool isBossNearby() const;
	bool isPlayerHidden() const;

	//Win/Lose conditions
	bool hasWon() const;
	bool isCarrying(const Entity* item) const;
	bool hasRunOutOfTurns() const;
	bool isGameOver() const;

	//Helpers 
	void createWorld();
	Room* addRoom(const std::string& name, const std::string& description, RoomType roomType);
	Item* addItem(const std::string& name, const std::string& description,
		Entity* location, bool isContainer = false);
	void addNPC(NPC* npc, Room* startRoom);
	void addExit(const std::string& name, const std::string& description,
		Direction direction, Room* source, Room* destination, const Entity* key = nullptr);
};
