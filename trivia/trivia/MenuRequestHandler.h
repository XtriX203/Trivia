#pragma once
#include <iostream>
#include <vector>
#include "RequestStructs.h"
#include "RequestHandlerFactory.h"
#include "Room.h"
#include <mutex>

using std::vector;

class RequestHandlerFactory;


class MenuRequestHandler : IRequestHandler
{
private:
	LoggedUser m_user;
	RoomManager& m_roomManager;
	StatisticsManager& m_statisticsManager;
	RequestHandlerFactory& m_handlerFactory;


	RequestResult signout(RequestInfo info) ; 
	RequestResult getRooms(RequestInfo info) ;
	RequestResult getPlayersInRoom(RequestInfo info) ;
	RequestResult getPersonalStats(RequestInfo info) ;
	RequestResult getHighScore(RequestInfo info) ;
	RequestResult joinRoom(RequestInfo info) ;
	RequestResult createRoom(RequestInfo info) ;


public:
	MenuRequestHandler(LoggedUser user, RoomManager& manager, RequestHandlerFactory& factory, StatisticsManager& stat);
	virtual bool isRequestRelevant(RequestInfo info);
	virtual RequestResult handleRequest(RequestInfo info);
	
};