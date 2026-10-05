#pragma once

#include <string>
#include <vector>
#include "exit.h"

enum class CommandType
{
	GO,
	LOOK,
	TAKE,
	TAKE_FROM,
	DROP,
	PUT,
	INVENTORY,
	WAIT,
	QUIT,
	UNKNOWN
};

struct Command
{
	CommandType type = CommandType::UNKNOWN;
	Direction direction = Direction::NORTH;   // only meaningful for GO
	std::string target;                       // item name for TAKE, TAKE_FROM, DROP and PUT
	std::string container;                    // container name for PUT and TAKE_FROM
};

// Translates a line typed by the user into a Command 
class Parser
{
public:
	Command parse(const std::string& line) const;

private:
	std::string toLower(const std::string& text) const;
	std::vector<std::string> split(const std::string& line) const;
	bool parseDirection(const std::string& word, Direction& direction) const;
};
