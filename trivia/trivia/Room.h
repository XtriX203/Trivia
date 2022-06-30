#pragma once
#include <iostream>
#include <vector>
#include "LoggedUser.h"

using std::string;
using std::vector;

typedef struct RoomData
{
	unsigned int id;
	string name="";
	unsigned int maxPlayers=0;
	unsigned int numOfQuestionsInGame=0;
	unsigned int timePerQuestion=0;
	unsigned int isActive=0;
	bool isGameBegun = false;
}RoomData;


class Room
{
private:
	RoomData m_metadata;
	vector<LoggedUser> m_users;

public:
	Room();
	Room(LoggedUser user , RoomData data);
	void addUser(LoggedUser user);
	void RemoveUser(LoggedUser user);
	void setIsGameBegan(bool ans);
	void setIsActive(unsigned int num);
	vector<string> getAllUsers();
	RoomData getRoomData();
	vector<LoggedUser> getLoggedUsers(void);
};