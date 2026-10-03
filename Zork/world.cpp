#include "world.h"
#include "entity.h"
#include "room.h"
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
		<< "Get out of the building without being caught.\n\n";

	for (const Entity* entity : m_entities)
	{
		std::cout << entity->getName() << ": " << entity->getDescription() << "\n";
	}
}

void World::createWorld()
{
	Room* street = new Room("Street",
		"A sunny day, and your motorbike is waiting for you.", RoomType::STREET);
	m_entities.push_back(street);

	Room* entrance = new Room("Entrance",
		"Glass doors and a badge reader.Freedom is just a few steps away.", RoomType::ENTRANCE);
	m_entities.push_back(entrance);

	Room* reception = new Room("Reception",
		"The heart of the building. Everyone has to walk through here.", RoomType::RECEPTION);
	m_entities.push_back(reception);

	Room* office = new Room("Office",
		"Rows of desks and humming monitors. Some coworkers smile at you; others watch you a little too closely.", RoomType::OFFICE);
	m_entities.push_back(office);

	Room* cafeteria = new Room("Cafeteria",
		"Coffee machine, vending machine and a lot of people eating.", RoomType::CAFETERIA);
	m_entities.push_back(cafeteria);

	Room* bathroom = new Room("Bathroom",
		"Quiet, tiled and slightly too cold. A good place to wait.", RoomType::BATHROOM);
	m_entities.push_back(bathroom);
}