#include "item.h"

Item::Item(const std::string& name, const std::string& description, bool isContainer)
	: Entity(EntityType::ITEM, name, description), m_isContainer(isContainer)
{
}

void Item::update()
{
}
