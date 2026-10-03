#pragma once

#include "entity.h"

// An object that can be picked up, dropped or put inside a container.
class Item : public Entity
{
public:
	Item(const std::string& name, const std::string& description, bool isContainer = false);

	// To know if an item is a container (like backpack)
	bool isContainer() const { return m_isContainer; }

	void update() override;

private:
	const bool m_isContainer;
};
