#pragma once
#include <iostream>
#include "IDatabase.h"
#include "LoginRequestHandler.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "SqliteDataBase.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameManager.h"
#include "GameRequestHandler.h"
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class GameRequestHandler;
class LoginRequestHandler;
class MenuRequestHandler;
class StatisticsManager;

class RequestHandlerFactory
{
private:
	IDatabase* m_database;//variable that stores pointer to database 
	LoginManager m_loginManager;
	RoomManager m_roomManager;
	StatisticsManager m_StatisticsManager;
	GameManager m_gameManager;

public:
	RequestHandlerFactory(IDatabase* db);				// Constructor 
	LoginRequestHandler* createLoginRequestHandler();	// function that create a LoginRequestHandler object and return it
	LoginManager& getLoginManager();					// Getter method - m_loginManager
	IDatabase* getDB();
	MenuRequestHandler* createMenuRequestHandler(LoggedUser user);
	StatisticsManager& getStatisticsManager();
	RoomManager& getRoomManager();
	RoomAdminRequestHandler* createRoomAdminRequestHandler(Room room, LoggedUser user);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(Room room, LoggedUser user);
	GameManager& getGameManager();
	GameRequestHandler* createGameRequestHandler(Game game, LoggedUser user);

};