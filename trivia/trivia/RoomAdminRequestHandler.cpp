#include "RoomAdminRequestHandler.h"

std::mutex adminMtx;

RoomAdminRequestHandler::RoomAdminRequestHandler(LoggedUser user, RoomManager& manager, RequestHandlerFactory& factory, Room room)
    :m_handlerFactory(factory), m_user(user), m_roomManager(manager), m_room(room)
{
}

/// <summary>
/// method to close the room by admin
/// </summary>
RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo info)
{
    CloseRoomResponse res;
    RequestResult ret;
    adminMtx.lock();
    this->m_roomManager.deleteRoom(this->m_room.getRoomData().id);//delete the room from room manager
    adminMtx.unlock();
    res.status = 1;
    Buffer buff = JsonResponsePacketSerializer::serializeResponse(res);
    for (int i = 0; i < buff.size(); i++)
    {
        ret.buffer.push_back(buff[i]);
    }
    ret.newHandler = (IRequestHandler*)this->m_handlerFactory.createMenuRequestHandler(this->m_user);//set new handler to menu handler
    return ret;
}

/// <summary>
/// method to start the game
/// </summary>
RequestResult RoomAdminRequestHandler::startGame(RequestInfo info)
{
    RequestResult ret;
    StartGameResponse res;
    adminMtx.lock();
    //change the flags in room manager and room
    this->m_roomManager.setIsGameBegun(true, this->m_roomManager.getRoomId(this->m_room));//set the flag to true-game begun
    this->m_room.setIsGameBegan(true);
    adminMtx.unlock();
    res.status = 1;
    Buffer buffer=JsonResponsePacketSerializer::serializeResponse(res);
    for (int i = 0; i < buffer.size(); i++)
    {
        ret.buffer.push_back( buffer[i]);
    }
    
    ret.newHandler = (IRequestHandler*)this->m_handlerFactory.createGameRequestHandler(this->m_handlerFactory.getGameManager().createGame(this->m_room),this->m_user);//change the handler to game handler 
    return ret;
}

/// <summary>
/// method that returns the state of the current room
/// </summary>
/// <param name="info"></param>
/// <returns></returns>
RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo info)
{
    RequestResult ret;
    GetRoomStateResponse res;
    ErrorResponse err;
    Buffer buffer;
    adminMtx.lock();
    try
    {
        
        RoomData data = this->m_room.getRoomData();
        //fill all the parameters
        res.answerTimeout = data.timePerQuestion;
        res.hasGameBegun = data.isGameBegun;
        int id =this->m_roomManager.getRoomId(this->m_room);//get updataed players list from room manager
        res.players = this->m_roomManager.getRoom(id).getAllUsers();
        res.questionCount = data.numOfQuestionsInGame;
        res.status = 1;
        buffer = JsonResponsePacketSerializer::serializeResponse(res);

    }
    catch (...)
    {
        //if exception thrown
        err.message = "Room closed";
        buffer = JsonResponsePacketSerializer::serializeResponse(err);
    }
    adminMtx.unlock();
    for (int i = 0; i < buffer.size(); i++)
    {
        ret.buffer.push_back(buffer[i]);
    }
    ret.newHandler = this;
    return ret;
}

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo info)
{
    return (info.requestId==CLOSEROOM_CODE) || (info.requestId==STARTGAME_CODE) || (info.requestId==GETROOMSTATE_CODE);
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo info)
{
    if (isRequestRelevant(info))
    {
        switch (info.requestId)
        {
        case(CLOSEROOM_CODE):
        {
            return closeRoom(info);
            break;
        }
        case(STARTGAME_CODE):
        {
            return startGame(info);
            break;
        }
        case(GETROOMSTATE_CODE):
        {
            return getRoomState(info);
            break;
        }
        default:
            break;
        }
    }
    //return error message
    RequestResult ret;
    ErrorResponse err;
    err.message = "Error";
    ret.buffer = JsonResponsePacketSerializer::serializeResponse(err);
    ret.newHandler = this;
    return ret;
}
