#pragma once

#include <vector>

class Entity;

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

	void createWorld();
};