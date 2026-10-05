#include "parser.h"
#include <algorithm>
#include <cctype>

Command Parser::parse(const std::string& line) const
{
	Command cmd;
	const std::vector<std::string> words = split(toLower(line));

	if (words.empty())
	{
		return cmd;
	}

	const std::string& verb = words[0];

	if (verb == "quit" || verb == "exit" || verb == "q")
	{
		cmd.type = CommandType::QUIT;
	}
	else if (verb == "look" || verb == "l")
	{
		cmd.type = CommandType::LOOK;
	}
	else if (verb == "inventory" || verb == "i")
	{
		cmd.type = CommandType::INVENTORY;
	}

	else if (verb == "wait" || verb == "z")
	{
		cmd.type = CommandType::WAIT;
	}

	else if (verb == "go")
	{
		if (words.size() >= 2 && parseDirection(words[1], cmd.direction))
		{
			cmd.type = CommandType::GO;
		}
	}
	else if (verb == "take" || verb == "get")
	{
		// take item from container
		if (words.size() >= 4 && words[2] == "from")
		{
			cmd.type = CommandType::TAKE_FROM;
			cmd.target = words[1];
			cmd.container = words[3];
		}
		// take item
		else if (words.size() >= 2)
		{
			cmd.type = CommandType::TAKE;
			cmd.target = words[1];
		}
	}
	else if (verb == "put")
	{
		// put item in container
		if (words.size() >= 4 && words[2] == "in")
		{
			cmd.type = CommandType::PUT;
			cmd.target = words[1];
			cmd.container = words[3];
		}
	}
	else if (verb == "drop")
	{
		if (words.size() >= 2)
		{
			cmd.type = CommandType::DROP;
			cmd.target = words[1];
		}
	}
	else if (parseDirection(verb, cmd.direction))
	{
		cmd.type = CommandType::GO;
	}

	return cmd;
}

std::string Parser::toLower(const std::string& text) const
{
	std::string result = text;
	std::transform(result.begin(), result.end(), result.begin(),
		[](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	return result;
}

// Splits a line into words
std::vector<std::string> Parser::split(const std::string& line) const
{
	std::vector<std::string> result;
	std::string word;

	for (unsigned char c : line)
	{
		if (std::isalpha(c))
		{
			word += static_cast<char>(c);
		}
		else if (!word.empty())
		{
			result.push_back(word);
			word.clear();
		}
	}

	if (!word.empty())
	{
		result.push_back(word);
	}

	return result;
}

bool Parser::parseDirection(const std::string& word, Direction& direction) const
{
	if (word == "north" || word == "n")
	{
		direction = Direction::NORTH;
		return true;
	}
	else if (word == "south" || word == "s")
	{
		direction = Direction::SOUTH;
		return true;
	}
	else if (word == "east" || word == "e")
	{
		direction = Direction::EAST;
		return true;
	}
	else if (word == "west" || word == "w")
	{
		direction = Direction::WEST;
		return true;
	}

	return false;
}
