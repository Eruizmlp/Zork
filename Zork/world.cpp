#include "world.h"
#include "entity.h"
#include "room.h"
#include "exit.h"
#include "player.h"
#include "item.h"
#include <iostream>

World::World()
{
	createWorld();
}

World::~World()
{
	for (Entity* entity : m_entities)
	{
		delete entity;
	}
}

void World::runWorld()
{
	std::cout << "It is Friday, 12:50. Your shift ends at 17:00, but you plan to leave at lunch time.\n"
		<< "Get out of the building without being caught. Type 'quit' to give up.\n\n";
	m_player->look();

	std::string line;
	while (!isGameOver())
	{
		std::cout << "\n> ";
		if (!std::getline(std::cin, line))
		{
			break;
		}

		executeCommand(m_parser.parse(line));
		update();
	}

	if (hasWon())
	{
		std::cout << "\nYou made it out. Enjoy your long weekend!\n";
	}
	else if (m_currTurn >= MAX_TURNS)
	{
		std::cout << "\nToo late. Your boss spots you and asks for \"a quick favour\".\n";
	}
}

void World::executeCommand(const Command& command)
{
	switch (command.type)
	{
	case CommandType::GO:
		if (m_player->move(command.direction))
		{
			++m_currTurn;
			m_player->look();
		}
		else
		{
			std::cout << "You can't go that way.\n";
		}
		break;

	case CommandType::LOOK:
		m_player->look();
		break;

	case CommandType::INVENTORY:
		m_player->inventory();
		break;

	case CommandType::TAKE:
		if (m_player->take(command.target))
		{
			++m_currTurn;
		}
		break;

	case CommandType::DROP:
		if (m_player->drop(command.target))
		{
			++m_currTurn;
		}
		break;

	case CommandType::QUIT:
		std::cout << "You sigh and go back to your desk. Maybe next Friday.\n";
		m_gameOver = true;
		break;

	default:
		std::cout << "I don't understand that.\n";
		break;
	}
}

// Updates every entity in the world
void World::update()
{
	for (Entity* entity : m_entities)
	{
		entity->update();
	}
}

bool World::hasWon() const
{
	return m_player->getLocation() == m_street;
}

bool World::isGameOver() const
{
	return m_gameOver || hasWon() || m_currTurn >= MAX_TURNS;
}

void World::createWorld()
{
	// Rooms
	m_street = new Room("Street",
		"A sunny day, and your motorbike is waiting for you.", RoomType::STREET);
	m_entities.push_back(m_street);

	Room* entrance = new Room("Entrance",
		"Glass doors and a badge reader. Freedom is just a few steps away.", RoomType::ENTRANCE);
	m_entities.push_back(entrance);

	Room* reception = new Room("Reception",
		"The heart of the building. Everyone has to walk through here.", RoomType::RECEPTION);
	m_entities.push_back(reception);

	Room* office = new Room("Office",
		"Rows of desks and humming monitors. Some coworkers smile at you; others watch you a little too closely.",
		RoomType::OFFICE);
	m_entities.push_back(office);

	Room* cafeteria = new Room("Cafeteria",
		"Coffee machine, vending machine and a lot of people eating.", RoomType::CAFETERIA);
	m_entities.push_back(cafeteria);

	Room* bathroom = new Room("Bathroom",
		"Quiet, tiled and slightly too cold. A good place to wait.", RoomType::BATHROOM);
	m_entities.push_back(bathroom);

	// Add exits to rooms
	addExit("corridor", "A corridor leads south to reception.", Direction::SOUTH, office, reception);
	addExit("corridor", "A corridor leads north to the office.", Direction::NORTH, reception, office);

	addExit("door", "A door leads west to the bathroom.", Direction::WEST, reception, bathroom);
	addExit("door", "The door leads east back to reception.", Direction::EAST, bathroom, reception);

	addExit("archway", "An archway leads east to the cafeteria.", Direction::EAST, reception, cafeteria);
	addExit("archway", "The archway leads west back to reception.", Direction::WEST, cafeteria, reception);

	addExit("hall", "A short hall leads south to the entrance.", Direction::SOUTH, reception, entrance);
	addExit("hall", "The hall leads north back to reception.", Direction::NORTH, entrance, reception);

	addExit("glass doors", "The glass doors lead south to the street.", Direction::SOUTH, entrance, m_street);

	// Player
	m_player = new Player("You", "An employee with a strong desire to leave early.");
	m_entities.push_back(m_player);
	m_player->moveTo(office);

	// Items (names in lowercase because of the parser) 
	Item* badge = new Item("badge", "Your access badge. It opens the glass doors.");
	m_entities.push_back(badge);
	badge->moveTo(m_player);

	Item* backpack = new Item("backpack", "Your backpack, hanging from your chair.", true);
	m_entities.push_back(backpack);
	backpack->moveTo(office);

	Item* chocolate = new Item("chocolate", "A chocolate bar. Some people would do anything for one.");
	m_entities.push_back(chocolate);
	chocolate->moveTo(cafeteria);
}

void World::addExit(const std::string& name, const std::string& description,
	Direction direction, Room* source, Room* destination)
{
	Exit* exit = new Exit(name, description, direction, source, destination);
	m_entities.push_back(exit);
	exit->moveTo(source);
}
