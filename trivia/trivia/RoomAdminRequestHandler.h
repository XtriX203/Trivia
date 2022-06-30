#pragma once
#include <iostream>
#include <vector>
#include "RequestStructs.h"
#include "RequestHandlerFactory.h"
#include "Room.h"
#include <mutex>

class RoomAdminRequestHandler : public IRequestHandler
{
private:
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory &m_handlerFactory;

	RequestResult closeRoom(RequestInfo info);
	RequestResult startGame(RequestInfo info);
	RequestResult getRoomState(RequestInfo info);

public:
	RoomAdminRequestHandler(LoggedUser user, RoomManager &manager, RequestHandlerFactory &factory, Room room);
	virtual bool isRequestRelevant(RequestInfo info);
	virtual RequestResult handleRequest(RequestInfo info);
};
