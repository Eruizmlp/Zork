#include "world.h"
#include "entity.h"
#include "room.h"
#include "exit.h"
#include "player.h"
#include "item.h"
#include "npc.h"
#include "boss.h"
#include "snitch.h"
#include "friendly.h"
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

	std::cout << "\nPress Enter to exit...";
	std::cin.get();
}

void World::printIntro() const
{
	std::cout
		<< "==============================================\n"
		<< "              ESCAPE THE OFFICE\n"
		<< "==============================================\n\n"
		<< "It's Friday, " << clockText(0) << ".\n"
		<< "Your shift doesn't end until 17:00, but you've decided to leave early.\n"
		<< "Lunch ends at " << clockText(MAX_TURNS) << ", and every action you take costs one minute.\n\n"
		<< "There's just one problem: you're not carrying your motorbike keys.\n"
		<< "Find them and get out of the building before lunch is over,\n"
		<< "without anybody catching you.\n\n"
		<< "Type 'look' to see where you are and 'help' to see all the commands.\n";
}

void World::printHelp() const
{
	std::cout
		<< "\nAvailable commands:\n"
		<< "  go <direction>               Move north, south, east or west (or just n, s, e, w).\n"
		<< "  look                         Look around the room.\n"
		<< "  inventory                    Check what you carry and your watch.\n"
		<< "  take <item>                  Pick up an item.\n"
		<< "  take <item> from <object>    Take an item out of a container.\n"
		<< "  drop <item>                  Leave an item in the room.\n"
		<< "  put <item> in <object>       Put an item inside a container.\n"
		<< "  talk <person>                Talk to someone.\n"
		<< "  give <item> to <person>      Give an item to someone.\n"
		<< "  wait                         Wait for one minute (only in the bathroom).\n"
		<< "  help                         Show this list.\n"
		<< "  quit                         Give up and end the game.\n\n"
		<< "Looking, checking your inventory, talking and asking for help don't cost any time.\n";
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

		// The new room is described after the NPCs have moved
		if (command.type == CommandType::GO && !hasWon())
		{
			m_player->look();
		}

		checkNPCs();

		if (!isGameOver())
		{
			printAtmosphere();
		}
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
		std::cout << "\nIt's " << clockText(MAX_TURNS) << ", lunch is over. Your boss spots you and asks for \"a quick favour\".\n";
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
		printTime();
		return false;

	case CommandType::WAIT:
		return m_player->wait();

	case CommandType::TALK:
		m_player->talkTo(command.target);
		return false;

	case CommandType::GIVE:
		return m_player->give(command.target, command.container);

	case CommandType::TAKE:
		return m_player->take(command.target);

	case CommandType::DROP:
		return m_player->drop(command.target);

	case CommandType::PUT:
		return m_player->put(command.target, command.container);

	case CommandType::TAKE_FROM:
		return m_player->takeFrom(command.target, command.container);

	case CommandType::HELP:
		printHelp();
		return false;

	case CommandType::QUIT:
		std::cout << "You sigh and go back to your desk. Maybe next Friday.\n";
		m_gameOver = true;
		return false;

	case CommandType::UNKNOWN:
		std::cout << "I don't understand that.\n";
		printHelp();
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

bool World::isPlayerHidden() const
{
	const Room* room = m_player->getCurrentRoom();
	return room != nullptr && room->getRoomType() == RoomType::BATHROOM;
}

void World::checkNPCs()
{
	if (isPlayerHidden())
	{
		// Hiding works with everyone except the boss walking into the bathroom
		if (m_boss->spottedPlayer(m_player))
		{
			m_boss->onPlayerSpotted(m_player);
			return;
		}

		printReceptionSounds();
		return;
	}

	for (NPC* npc : m_npcs)
	{
		if (npc->spottedPlayer(m_player))
		{
			npc->onPlayerSpotted(m_player);
		}
	}
}

void World::printReceptionSounds() const
{
	std::string voices;
	for (const NPC* npc : m_npcs)
	{
		const Room* room = npc->getCurrentRoom();
		if (npc == m_boss || room == nullptr || room->getRoomType() != RoomType::RECEPTION)
		{
			continue;
		}

		if (!voices.empty())
		{
			voices += ", ";
		}
		voices += npc->getName();
	}

	const Room* bossRoom = m_boss->getCurrentRoom();
	if (bossRoom != nullptr && bossRoom->getRoomType() == RoomType::RECEPTION)
	{
		if (m_boss->isAlerted())
		{
			std::cout << "You hear THE BOSS SHOUTING in the reception: \"WHERE IS EVERYONE?!\"\n";
		}
		else
		{
			std::cout << "You hear the boss in the reception...\n";
		}
	}

	if (!voices.empty())
	{
		std::cout << "Voices in the reception: " << voices << ".\n";
	}
}

void World::printAtmosphere() const
{
	if (!isPlayerHidden() && isBossNearby())
	{
		if (m_boss->isAlerted())
		{
			std::cout << "YOU HEAR THE BOSS STOMPING AROUND IN THE NEXT ROOM!\n";
		}
		else if (m_boss->getNextRoom() == m_player->getCurrentRoom())
		{
			std::cout << "You hear the boss's footsteps coming your way...\n";
		}
	}

	const int minutesLeft = MAX_TURNS - m_currTurn;
	if (minutesLeft == 10)
	{
		std::cout << "Half of the lunch break is gone.\n";
	}
	else if (minutesLeft == 5)
	{
		std::cout << "ONLY FIVE MINUTES LEFT! People are starting to come back from lunch.\n";
	}
	else if (minutesLeft == 2)
	{
		std::cout << "TWO MINUTES! YOU CAN ALREADY HEAR YOUR COWORKERS IN THE HALL!\n";
	}
}

// True if the boss is in a room connected to the player's room
bool World::isBossNearby() const
{
	const Room* playerRoom = m_player->getCurrentRoom();
	const Room* bossRoom = m_boss->getCurrentRoom();
	if (playerRoom == nullptr || bossRoom == nullptr)
	{
		return false;
	}

	for (const Exit* exit : playerRoom->getExits())
	{
		if (exit->getDestination() == bossRoom)
		{
			return true;
		}
	}

	return false;
}

bool World::hasWon() const
{
	// Reaching the street doesn't count if the boss is out there smoking and catches you
	return m_player->getLocation() == m_street && isCarrying(m_keys) && !m_boss->hasCaughtPlayer();
}

// True if the player has the item in hand or inside something the player carries
bool World::isCarrying(const Entity* item) const
{
	const Entity* holder = item->getLocation();
	if (holder == m_player)
	{
		return true;
	}

	return holder != nullptr && holder->getLocation() == m_player;
}

bool World::hasRunOutOfTurns() const
{
	return m_currTurn >= MAX_TURNS;
}

// Time on the clock after the given number of turns, as "13:45"
std::string World::clockText(int turn) const
{
	const int minutes = START_MINUTES + turn;
	const int hour = minutes / 60;
	const int minute = minutes % 60;

	std::string text = std::to_string(hour) + ":";
	if (minute < 10)
	{
		text += "0";
	}
	return text + std::to_string(minute);
}

// Shows the current time and how many minutes are left before lunch ends
void World::printTime() const
{
	std::cout << "Your watch says " << clockText(m_currTurn) << ". Lunch ends at " << clockText(MAX_TURNS)
		<< " (" << MAX_TURNS - m_currTurn << " minutes left).\n";
}

bool World::isGameOver() const
{
	return m_gameOver || m_boss->hasCaughtPlayer() || hasWon() || hasRunOutOfTurns();
}
void World::createWorld()
{
	// Rooms
	m_street = addRoom("Street",
		"A sunny day. Your motorbike is parked here, but you need its keys to ride away.", RoomType::STREET);
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

	// Items (names in lowercase, like everything the player types)
	Item* backpack = addItem("backpack", "Your backpack, hanging from your chair.", office, true);
	Item* badge = addItem("badge", "Your access badge. The glass doors only open if you hold it in your hand.", backpack);
	Item* chocolate = addItem("chocolate", "A chocolate bar. Some people would do anything for one.", cafeteria);
	Item* coffee = addItem("coffee", "A cup of coffee from the machine, still hot.", cafeteria);

	// The cigars start nowhere: marta holds them, so the boss cannot find them by himself
	Item* cigars = addItem("cigars", "A box of cigars addressed to the boss. He can never resist one.", nullptr);
	Item* jacket = addItem("jacket", "Your jacket. You hung it in the bathroom this morning.", bathroom, true);
	m_keys = addItem("keys", "Your motorbike keys.", jacket);

	// NPC routes: one room per turn 
	const std::vector<Room*> bossRoute = { cafeteria, cafeteria, cafeteria, reception,
		entrance, entrance, entrance, reception, office, office, reception };
	const std::vector<Room*> bossAlertRoute = { reception, entrance, entrance, entrance, reception };
	const std::vector<Room*> bossSmokeRoute = { reception, entrance, m_street, m_street, m_street, m_street, entrance, reception };
	const std::vector<Room*> marcRoute = { office, office, reception, reception };
	const std::vector<Room*> nuriaRoute = { cafeteria, cafeteria, cafeteria, cafeteria, cafeteria, reception };
	const std::vector<Room*> pabloRoute = { reception, bathroom, reception, office, reception, cafeteria, cafeteria };

	// NPC Dialogues
	const std::string martaDialogue =
		"Looking for your keys? You hung your jacket in the bathroom this morning. "
		"And careful: after his coffee, the boss always goes out to the entrance for a smoke. "
		"Oh, and he left this box of cigars stored here in reception... take it, the smell is driving me crazy! "
		"If you give it to him, he'll head straight outside to smoke.";

	const std::string lauraDialogue =
		"Marc is always eyeing everyone's business... but leave a chocolate bar on his desk "
		"and he suddenly forgets what he was doing.";

	const std::string pabloCoffeeDialogue =
		"Ah, sweet caffeine... Listen, the badge reader won't pick up your card inside a backpack, "
		"so hold it out. As for Marc and Nuria? Keep everything else in your bag—if you carry "
		"items in your hands, they'll think you're packing up to leave early.";

	// NPCs 
	m_boss = new Boss(
		"boss",
		"Your boss, holding a coffee and looking for someone to give work to.",
		bossRoute, bossAlertRoute, bossSmokeRoute, cigars
	);
	addNPC(m_boss, cafeteria);

	// Snitches
	addNPC(new Snitch("marc", "A coworker who loves telling the boss what everyone does.",
		marcRoute, m_boss, { m_keys, jacket }, chocolate), office);

	addNPC(new Snitch("nuria", "A coworker who notices everything people carry around.",
		nuriaRoute, m_boss, { backpack, jacket }, chocolate), cafeteria);

	// Friendly NPCs
	Friendly* marta = new Friendly(
		"marta", "The receptionist. She knows everything and tells nobody.",
		{ reception }, martaDialogue, m_boss, nullptr, "", cigars
	);
	addNPC(marta, reception);
	cigars->moveTo(marta);

	addNPC(new Friendly(
		"laura", "A coworker having lunch. She can't stand Marc.",
		{ cafeteria }, lauraDialogue, m_boss
	), cafeteria);

	addNPC(new Friendly(
		"pablo", "The IT guy. He looks like he hasn't slept in days.",
		pabloRoute, "I'd kill for a coffee right now...", m_boss, coffee, pabloCoffeeDialogue
	), reception);

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
	addExit("glass doors", "The glass doors lead north back into the building.", Direction::NORTH, m_street, entrance, badge);
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

