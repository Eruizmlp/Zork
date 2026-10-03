#pragma once
#include <string>
#include <list>

// Base class of everything that exists in the game.
// Knows its type, name and description, what it contains and where it is.
enum class EntityType
{
	ROOM,
	EXIT,
	CREATURE,
	ITEM,
};

class Entity
{
public:
	Entity(EntityType type, const std::string& name, const std::string& description);
	Entity(const Entity&) = delete;
	virtual ~Entity();
	Entity& operator=(const Entity&) = delete;

	EntityType getType() const { return m_type; }
	const std::string& getName() const { return m_name; }
	const std::string& getDescription() const { return m_description; }
	const std::list<Entity*>& getContains() const { return m_contains; }

	virtual void update() = 0;


protected:
	EntityType m_type;
	const std::string m_name;
	const std::string m_description;
	std::list<Entity*> m_contains;

};
