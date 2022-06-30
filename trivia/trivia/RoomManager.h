#pragma once
#include "Room.h"
#include <map>

using std::map;
using std::vector;
using std::string;

class RoomManager
{
private:
	map<int, Room> m_rooms;//roomId , Room

public:
	RoomManager();
	void createRoom(LoggedUser user ,RoomData data );
	void createRoom(Room room);
	void addToRoom(LoggedUser user, int room_id);
	void deleteRoom(int ID);
	bool getRoomState(int ID);
	Room getRoom(int id);
	void setIsGameBegun(bool con, int id);
	vector<string> getPlayersInRoom(int id);
	vector<RoomData> getRooms();
	bool doesNameExist(string name);
	unsigned int getRoomId(Room room);
	void removeFromRoom(const LoggedUser user, const unsigned int id);
};