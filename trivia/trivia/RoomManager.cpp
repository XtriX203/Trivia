#include "RoomManager.h"

RoomManager::RoomManager()
{
}

/// <summary>
/// method that creates room by data and user(admin)
/// </summary>
void RoomManager::createRoom(LoggedUser user , RoomData data)
{
	Room newRoom = Room(user,data);//create new room object
	newRoom.addUser(user);
	std::pair<int, Room> pair1((int)this->m_rooms.size(), newRoom);//create pait to insert to the map of rooms
	this->m_rooms.insert(pair1);//insert the pair
}

/// <summary>
/// override to method that creates room
/// </summary>
void RoomManager::createRoom(Room room)
{
	std::pair<int, Room> pair1((int)this->m_rooms.size(), room);//create pait to insert to the map of rooms
	this->m_rooms.insert(pair1);//insert the pair
}

/// <summary>
/// add player to room by id
/// </summary>
void RoomManager::addToRoom(LoggedUser user, int room_id)
{	
	this->m_rooms.find(room_id)->second.addUser(user);
}

/// <summary>
/// delete room from rooms list
/// </summary>
void RoomManager::deleteRoom(int ID)
{
	if (this->m_rooms.size() == 1)
	{
		this->m_rooms.clear();
		return;
	}
	try { this->m_rooms.erase(this->m_rooms.find(ID)); }
	catch (const std::exception& e) { throw(e); }
}

/// <summary>
/// returnt if the game has begun
/// </summary>
/// <param name="ID">id of room</param>
bool RoomManager::getRoomState(int ID)
{
	map<int, Room>::iterator it = this->m_rooms.find(ID);
	RoomData data = it->second.getRoomData();
	return data.isGameBegun;
}

/// <summary>
/// returns room by id
/// </summary>
Room RoomManager::getRoom(int id)
{
	auto a = this->m_rooms.find(id);
	if (a != this->m_rooms.end()) return a->second;
	throw(":)");
}

void RoomManager::setIsGameBegun(bool con, int id)
{
	this->m_rooms[id].setIsGameBegan(con);
}

vector<string> RoomManager::getPlayersInRoom(int id)
{
	return this->m_rooms.find(id)->second.getAllUsers();//get all users of specific room
}

/// <summary>
/// return all rooms data
/// </summary>
/// <returns></returns>
vector<RoomData> RoomManager::getRooms()
{
	vector<RoomData> ret;
	RoomData data;

	for(map<int, Room>::iterator it = this->m_rooms.begin(); it != this->m_rooms.end(); ++it)
	{
		data = it->second.getRoomData();
		ret.push_back(data);
	}
	return ret;
}

/// <summary>
/// check if name of rom already exists
/// </summary>
bool RoomManager::doesNameExist(string name)
{
	map<int, Room>::iterator it;
	for (it = this->m_rooms.begin(); it != this->m_rooms.end(); ++it)
	{
		if (it->second.getRoomData().name == name)
		{
			return true;
		}
	}
	return false;
}

/// <summary>
/// get id of room by its data
/// </summary>
/// <param name="room"></param>
unsigned int RoomManager::getRoomId(Room room)
{
	map<int, Room>::iterator it;
	for (it = this->m_rooms.begin(); it != this->m_rooms.end(); ++it)
	{
		if (room.getRoomData().name == it->second.getRoomData().name)//if the name is found
		{
			return it->first;
		}
	}
	return 0;
}

/// <summary>
/// remove player from room
/// </summary>
/// <param name="user">object of user</param>
void RoomManager::removeFromRoom(const LoggedUser user, const unsigned int id)
{
	try { if (this->m_rooms.count(id)) this->m_rooms[id].RemoveUser(user); }
	catch (...) { }
}
