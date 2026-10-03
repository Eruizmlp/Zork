#include "world.h"
#include "entity.h"
#include "room.h"
#include "exit.h"
#include "player.h"
#include "item.h"
#include "npc.h"
#include "boss.h"
#include "snitch.h"
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
		checkNPCs();
	}

	if (hasWon())
	{
		std::cout << "\nYou made it out. Enjoy your long weekend!\n";
	}
	else if (m_boss->hasCaughtPlayer())
	{
		std::cout << "\n\"Ah, there you are! Do you have five minutes?\" Game over.\n";
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
		switch (m_player->move(command.direction))
		{
		case MoveResult::MOVED:
			++m_currTurn;
			m_player->look();
			break;
		case MoveResult::LOCKED:
			std::cout << "It's locked. You need something to open it.\n";
			break;
		case MoveResult::NO_EXIT:
			std::cout << "You can't go that way.\n";
			break;
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

	case CommandType::PUT:
		if (m_player->put(command.target, command.container))
		{
			++m_currTurn;
		}
		break;

	case CommandType::TAKE_FROM:
		if (m_player->takeFrom(command.target, command.container))
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

//Activates the onPlayerSpotted method of the different NPCs
void World::checkNPCs()
{
	for (NPC* npc : m_npcs)
	{
		if (npc->spottedPlayer(m_player))
		{
			npc->onPlayerSpotted();
		}
	}
}

void World::addNPC(NPC* npc, Room* startRoom)
{
	m_entities.push_back(npc);
	m_npcs.push_back(npc);
	npc->moveTo(startRoom);
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
	return m_gameOver || m_boss->hasCaughtPlayer() || hasWon() || m_currTurn >= MAX_TURNS;
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

	//NPCS
	m_boss = new Boss("boss", "Your boss, holding a coffee and looking for someone to give work to.",
		{ cafeteria, cafeteria, cafeteria, reception, office, reception });
	addNPC(m_boss, cafeteria);

	addNPC(new Snitch("marc", "A coworker who loves telling the boss what everyone does.",
		{ office, office, reception, reception }, m_boss), office);

	addNPC(new NPC("marta", "The receptionist. She knows everything and tells nobody.",
		{ reception }), reception);

	// Add exits to rooms
	addExit("corridor", "A corridor leads south to reception.", Direction::SOUTH, office, reception);
	addExit("corridor", "A corridor leads north to the office.", Direction::NORTH, reception, office);

	addExit("door", "A door leads west to the bathroom.", Direction::WEST, reception, bathroom);
	addExit("door", "The door leads east back to reception.", Direction::EAST, bathroom, reception);

	addExit("archway", "An archway leads east to the cafeteria.", Direction::EAST, reception, cafeteria);
	addExit("archway", "The archway leads west back to reception.", Direction::WEST, cafeteria, reception);

	addExit("hall", "A short hall leads south to the entrance.", Direction::SOUTH, reception, entrance);
	addExit("hall", "The hall leads north back to reception.", Direction::NORTH, entrance, reception);

	
	addExit("glass doors", "The glass doors lead south to the street.", Direction::SOUTH, entrance, m_street, badge);
}

void World::addExit(const std::string& name, const std::string& description,
	Direction direction, Room* source, Room* destination, const Entity* key)
{
	Exit* exit = new Exit(name, description, direction, source, destination, key);
	m_entities.push_back(exit);
	exit->moveTo(source);
}
