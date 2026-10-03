#pragma once

#include "entity.h"
#include "exit.h"
#include <vector>

enum class RoomType
{
	STREET,
	ENTRANCE,
	RECEPTION,
	CAFETERIA,
	BATHROOM,
	OFFICE
};

// A place in the map. Contains exits, items and creatures.
class Room : public Entity
{
public:
	Room(const std::string& name, const std::string& description, RoomType roomType);

	RoomType getRoomType() const { return m_roomType; }
	Exit* getExit(Direction direction) const;
	std::vector<Exit*> getExits() const;
	std::vector<Entity*> getCreatures() const;
	std::vector<Entity*> getItems() const;

	void update() override;

private:
	const RoomType m_roomType;
};
