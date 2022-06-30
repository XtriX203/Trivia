#pragma once
#include <iostream>
#include <vector>
#include "RequestStructs.h"
#include "RequestHandlerFactory.h"
#include "Room.h"
#include <mutex>

class RoomMemberRequestHandler : public IRequestHandler
{
private:
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult leaveRoom(const RequestInfo info);
	RequestResult startGame(const RequestInfo info);
	RequestResult getRoomState(const RequestInfo info);

public:
	RoomMemberRequestHandler(Room room, LoggedUser user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);
	virtual bool isRequestRelevant(const RequestInfo info) ;
	virtual RequestResult handleRequest(const RequestInfo info);
};