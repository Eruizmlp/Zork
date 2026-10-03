#pragma once

#include <string>
#include <vector>
#include "parser.h"

class Entity;
class Room;
class Player;

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

	//Easy access to check if the game is finished
	Player* m_player = nullptr;
	Room* m_street = nullptr;

	// Each action consumes a turn the player must escape before MAX_TURNS
	int m_currTurn = 0;
	static const int MAX_TURNS = 15;
	bool m_gameOver = false;

	Parser m_parser;

	void createWorld();
	void addExit(const std::string& name, const std::string& description,
		Direction direction, Room* source, Room* destination);
	void executeCommand(const Command& command);

	void update();
	bool hasWon() const; //Checks if player in the street 
	bool isGameOver() const; 
};
