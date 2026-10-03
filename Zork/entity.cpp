#include "entity.h"
#include <algorithm>

Entity::Entity(EntityType type, const std::string& name, const std::string& description)
	: m_type(type), m_name(name), m_description(description), m_location(nullptr)
{
}

Entity::~Entity()
{

}

bool Entity::contains(const Entity* entity) const
{
	return std::find(m_contains.begin(), m_contains.end(), entity) != m_contains.end();
}


Entity* Entity::findByName(const std::string& name, EntityType type) const
{
	for (Entity* entity : m_contains)
	{
		if (entity->getType() == type && entity->getName() == name)
		{
			return entity;
		}
	}

	return nullptr;
}

bool Entity::moveTo(Entity* destination)
{
	if (destination == nullptr || destination == this || destination == m_location)
	{
		return false;
	}

	if (m_location != nullptr)
	{
		m_location->removeEntity(this);
	}

	destination->addEntity(this);
	m_location = destination;
	return true;
}


void Entity::addEntity(Entity* entity)
{
	m_contains.push_back(entity);
}

void Entity::removeEntity(Entity* entity)
{
	m_contains.remove(entity);
}