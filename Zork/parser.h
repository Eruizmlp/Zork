#pragma once

#include <string>
#include <vector>
#include "exit.h"

enum class CommandType
{
	GO,
	LOOK,
	TAKE,
	DROP,
	INVENTORY,
	QUIT,
	UNKNOWN
};

struct Command
{
	CommandType type = CommandType::UNKNOWN;
	Direction direction = Direction::NORTH;   
	std::string target;                       // item name for TAKE and DROP
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
