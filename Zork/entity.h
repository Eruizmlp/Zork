#pragma once

#include <string>
#include <list>

enum class EntityType
{
	ROOM,
	EXIT,
	CREATURE,
	ITEM
};

// Base class of everything that exists in the game. Knows its type, name and
// description, what it contains and where it is; moveTo() is the only way to move it.
class Entity
{
public:
	Entity(EntityType type, const std::string& name, const std::string& description);
	virtual ~Entity();

	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	EntityType getType() const { return m_type; }
	const std::string& getName() const { return m_name; }
	const std::string& getDescription() const { return m_description; }
	const std::list<Entity*>& getContains() const { return m_contains; }

	Entity* getLocation() const { return m_location; } // Items can be inside creatures or other items (locatiion must work for them

	bool contains(const Entity* entity) const;

	Entity* findByName(const std::string& name, EntityType type) const;

	bool moveTo(Entity* destination);

	virtual void update() = 0;

protected:
	const EntityType m_type;
	const std::string m_name;
	const std::string m_description;
	std::list<Entity*> m_contains;

private:
	Entity* m_location;

	void addEntity(Entity* entity);
	void removeEntity(Entity* entity);
};
