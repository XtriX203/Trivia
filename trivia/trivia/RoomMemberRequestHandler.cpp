#include "RoomMemberRequestHandler.h"

std::mutex memberMtx;

/*
LEAVE ROOM 
This method removed a user from the room.
input:		info - a Request Info struct, contains the information by the user choice
output:		ret - result from the server (it will send to the client)
*/
RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo info)
{
	RequestResult ret;
	LeaveRoomResponse res;
	memberMtx.lock();
	//this->m_room.RemoveUser(this->m_user);
	this->m_roomManager.removeFromRoom(this->m_user, this->m_room.getRoomData().id);
	memberMtx.unlock();
	res.status = 1;
	Buffer buff = JsonResponsePacketSerializer::serializeResponse(res);
	for (int i = 0; i < buff.size(); i++) ret.buffer.push_back(buff[i]);
	ret.newHandler = (IRequestHandler*)this->m_handlerFactory.createMenuRequestHandler(this->m_user);
	return ret;
}

/// <summary>
/// method that enter the player to new game screen
/// </summary>
RequestResult RoomMemberRequestHandler::startGame(const RequestInfo info)
{
	RequestResult ret;
	StartGameResponse res;
	res.status = 1;
	Buffer buff = JsonResponsePacketSerializer::serializeResponse(res);
	for (int i = 0; i < buff.size(); i++) ret.buffer.push_back(buff[i]);
	ret.newHandler = (IRequestHandler*)this->m_handlerFactory.createGameRequestHandler(this->m_handlerFactory.getGameManager().createGame(this->m_room), this->m_user);//change the handler to game handler 
	return ret;
}

/*
GET ROOM STATE
This method returns an importent data about the current room (players, number of questions, time for a question, has game begun)
input:		info - a Request Info struct, contains the information by the user choice
output:		ret - result from the server (it will send to the client)
*/
RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo info)
{
	RequestResult ret;
	GetRoomStateResponse res;
	ErrorResponse err;
	Buffer buff;
	try
	{
		res.status = 1;
		memberMtx.lock();
		Room room = this->m_roomManager.getRoom(this->m_room.getRoomData().id);//get object of the room of the player
		res.players = room.getAllUsers();//get all players in room
		RoomData roomData = room.getRoomData();//save the room data to variable
		memberMtx.unlock();
		//set all parameters of the room to return
		res.questionCount = roomData.numOfQuestionsInGame;
		res.answerTimeout = roomData.timePerQuestion;
		res.hasGameBegun = roomData.isGameBegun;
		buff = JsonResponsePacketSerializer::serializeResponse(res);
		ret.newHandler = this;
	}
	catch (...)
	{
		memberMtx.unlock();
		err.message = "Error";
		buff = JsonResponsePacketSerializer::serializeResponse(err);
		ret.newHandler = (IRequestHandler*)this->m_handlerFactory.createMenuRequestHandler(this->m_user);//set the handler to menu handler(if room closed the player will returned to the menu page)
	}
	for (int i = 0; i < buff.size(); i++) ret.buffer.push_back(buff[i]);
	return ret;
}

/*
CONSTRUCTOR
The constructor init an object
input:		room, user, room manager and handlerFactory
output:		RoomMemberRequestHandler object
*/
RoomMemberRequestHandler::RoomMemberRequestHandler(Room room, LoggedUser user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory): m_room(room), m_user(user), m_roomManager(roomManager), m_handlerFactory(handlerFactory) { }

/*
IS REQUEST RELEVANT
This method checks if the request id is relevant (start game, room state, leave room)
*/
bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo info) { return (info.requestId == STARTGAME_CODE) || (info.requestId == GETROOMSTATE_CODE) || (info.requestId == LEAVEROOM_CODE); }

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo info)
{
	if (isRequestRelevant(info))
	{
		switch (info.requestId)
		{
		case LEAVEROOM_CODE:
			return leaveRoom(info);
			break;

		case STARTGAME_CODE:
			return startGame(info);
			break;

		case GETROOMSTATE_CODE:
			return getRoomState(info);
			break;

		default:
			break;
		}
	}
	RequestResult ret;
	ErrorResponse err;
	err.message = "Error";
	ret.buffer = JsonResponsePacketSerializer::serializeResponse(err);
	ret.newHandler = this;
	return ret;
}
