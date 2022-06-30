#include "Room.h"

Room::Room() { }

Room::Room(LoggedUser user, RoomData data):m_metadata(data)
{
	//m_users.push_back(user);
}
/// <summary>
/// adding user to room
/// </summary>

void Room::addUser(LoggedUser user)
{
	this->m_users.push_back(user);
}
/// <summary>
/// removing user from room
/// </summary>
void Room::RemoveUser(LoggedUser user)
{
	for (vector<LoggedUser>::iterator it = this->m_users.begin(); it != this->m_users.end(); it++)
	{//check if the user exists
		if (it->getUsername().compare(user.getUsername())==0)
		{
			this->m_users.erase(it);
			break;
		}
	}
}

void Room::setIsGameBegan(bool ans)
{
	this->m_metadata.isGameBegun = ans;
}

void Room::setIsActive(unsigned int num)
{
	if (num == 0 || num == 1)
	{
		this->m_metadata.isActive = num;
	}
}
/// <summary>
/// return all the users in room
/// </summary>
vector<string> Room::getAllUsers()
{
	vector<string> names;
	for (vector<LoggedUser>::iterator it = this->m_users.begin(); it != this->m_users.end(); it++)
	{
		names.push_back(it->getUsername());
	}
	return names;
}

RoomData Room::getRoomData()
{
	return this->m_metadata;
}

vector<LoggedUser> Room::getLoggedUsers(void) { return this->m_users; }