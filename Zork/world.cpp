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
	printIntro();

	std::string line;
	while (!isGameOver() && readLine(line))
	{
		playTurn(line);
	}

	printEnding();
}

void World::printIntro() const
{
	std::cout << "It is Friday, 12:50. Your shift ends at 17:00, but you plan to leave at lunch time.\n"
		<< "Get out of the building without being caught. Type 'quit' to give up.\n\n";
	m_player->look();
}

bool World::readLine(std::string& line) const
{
	std::cout << "\n> ";
	return static_cast<bool>(std::getline(std::cin, line));
}

void World::playTurn(const std::string& line)
{
	const Command command = m_parser.parse(line);

	const bool usedTurn = tryAction(command);
	if (usedTurn)
	{
		++m_currTurn;
		update();
		checkNPCs();
	}
}

void World::printEnding() const
{
	if (hasWon())
	{
		std::cout << "\nYou made it out. Enjoy your long weekend!\n";
	}
	else if (m_boss->hasCaughtPlayer())
	{
		std::cout << "\n\"Ah, there you are! Do you have five minutes?\" Game over.\n";
	}
	else if (hasRunOutOfTurns())
	{
		std::cout << "\nToo late. Your boss spots you and asks for \"a quick favour\".\n";
	}
}

bool World::tryAction(const Command& command)
{
	switch (command.type)
	{
	case CommandType::GO:
		switch (m_player->move(command.direction))
		{
		case MoveResult::MOVED:
			m_player->look();
			return true;
		case MoveResult::LOCKED:
			std::cout << "It's locked. You need something to open it.\n";
			return false;
		case MoveResult::NO_EXIT:
			std::cout << "You can't go that way.\n";
			return false;
		}
		return false;

	case CommandType::LOOK:
		m_player->look();
		return false;

	case CommandType::INVENTORY:
		m_player->inventory();
		return false;

	case CommandType::TAKE:
		return m_player->take(command.target);

	case CommandType::DROP:
		return m_player->drop(command.target);

	case CommandType::PUT:
		return m_player->put(command.target, command.container);

	case CommandType::TAKE_FROM:
		return m_player->takeFrom(command.target, command.container);

	case CommandType::QUIT:
		std::cout << "You sigh and go back to your desk. Maybe next Friday.\n";
		m_gameOver = true;
		return false;

	case CommandType::UNKNOWN:
		std::cout << "I don't understand that.\n";
		return false;
	}

	return false;
}

// Updates every entity in the world
void World::update()
{
	for (Entity* entity : m_entities)
	{
		entity->update();
	}
}

// Activates the onPlayerSpotted method of the different NPCs
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

bool World::hasWon() const
{
	return m_player->getLocation() == m_street;
}

bool World::hasRunOutOfTurns() const
{
	return m_currTurn >= MAX_TURNS;
}

bool World::isGameOver() const
{
	return m_gameOver || m_boss->hasCaughtPlayer() || hasWon() || hasRunOutOfTurns();
}

void World::createWorld()
{
	// Rooms
	m_street = addRoom("Street",
		"A sunny day, and your motorbike is waiting for you.", RoomType::STREET);
	Room* entrance = addRoom("Entrance",
		"Glass doors and a badge reader. Freedom is just a few steps away.", RoomType::ENTRANCE);
	Room* reception = addRoom("Reception",
		"The heart of the building. Everyone has to walk through here.", RoomType::RECEPTION);
	Room* office = addRoom("Office",
		"Rows of desks and humming monitors. Some coworkers smile at you; others watch you a little too closely.",
		RoomType::OFFICE);
	Room* cafeteria = addRoom("Cafeteria",
		"Coffee machine, vending machine and a lot of people eating.", RoomType::CAFETERIA);
	Room* bathroom = addRoom("Bathroom",
		"Quiet, tiled and slightly too cold. A good place to wait.", RoomType::BATHROOM);

	// Player
	m_player = new Player("You", "An employee with a strong desire to leave early.");
	m_entities.push_back(m_player);
	m_player->moveTo(office);

	// Items (names in lowercase because of the parser)
	Item* badge = addItem("badge", "Your access badge. It opens the glass doors.", m_player);
	addItem("backpack", "Your backpack, hanging from your chair.", office, true);
	addItem("chocolate", "A chocolate bar. Some people would do anything for one.", cafeteria);

	// NPCs
	m_boss = new Boss("boss", "Your boss, holding a coffee and looking for someone to give work to.",
		{ cafeteria, cafeteria, cafeteria, reception, office, reception });
	addNPC(m_boss, cafeteria);

	addNPC(new Snitch("marc", "A coworker who loves telling the boss what everyone does.",
		{ office, office, reception, reception }, m_boss), office);

	addNPC(new NPC("marta", "The receptionist. She knows everything and tells nobody.",
		{ reception }), reception);

	// Exits
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

Room* World::addRoom(const std::string& name, const std::string& description, RoomType roomType)
{
	Room* room = new Room(name, description, roomType);
	m_entities.push_back(room);
	return room;
}

Item* World::addItem(const std::string& name, const std::string& description,
	Entity* location, bool isContainer)
{
	Item* item = new Item(name, description, isContainer);
	m_entities.push_back(item);
	item->moveTo(location);
	return item;
}

void World::addNPC(NPC* npc, Room* startRoom)
{
	m_entities.push_back(npc);
	m_npcs.push_back(npc);
	npc->moveTo(startRoom);
}

void World::addExit(const std::string& name, const std::string& description,
	Direction direction, Room* source, Room* destination, const Entity* key)
{
	Exit* exit = new Exit(name, description, direction, source, destination, key);
	m_entities.push_back(exit);
	exit->moveTo(source);
}
