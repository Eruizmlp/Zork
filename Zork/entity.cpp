#include "entity.h"

Entity::Entity(EntityType type, const std::string& name, const std::string& description)
	: m_type(type), m_name(name), m_description(description)
{

}

Entity::~Entity()
{
}
