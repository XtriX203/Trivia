#include "MenuRequestHandler.h"

std::mutex roomMtx;

MenuRequestHandler::MenuRequestHandler(LoggedUser user, RoomManager& manager, RequestHandlerFactory& factory, StatisticsManager& stat):m_user(user),m_roomManager(manager),m_handlerFactory(factory),m_statisticsManager(stat)
{
}
/// <summary>
/// function that signout a user
/// </summary>
RequestResult MenuRequestHandler::signout(RequestInfo info)
{
    RequestResult ret;
    LogoutResponse res;
    res.status = 0;
    if (isRequestRelevant(info))
    {
        this->m_handlerFactory.getLoginManager().logout(this->m_user.getUsername());//get the login manager from the handle factory and there log out the user
        res.status = 1;
        Buffer buff = JsonResponsePacketSerializer::serializeResponse(res);//serialize the buffer
        for (int i = 0; i < buff.size(); i++)
        {
            ret.buffer.push_back(buff[i]);
        }
    }
    ret.newHandler = (IRequestHandler*)this->m_handlerFactory.createLoginRequestHandler();//change the handler to Login request handler
    return ret;
}
/// <summary>
/// function that return all the rooms data
/// </summary>
RequestResult MenuRequestHandler::getRooms(RequestInfo info)
{
    RequestResult ret;
    GetRoomResponse res;
    res.status = 0;
    if (isRequestRelevant(info))
    {
        res.status = 1;
        roomMtx.lock();
        res.rooms = this->m_roomManager.getRooms();//get all rooms
        roomMtx.unlock();
        Buffer buff = JsonResponsePacketSerializer::serializeResponse(res);
        for (int i = 0; i < buff.size(); i++)
        {
            ret.buffer.push_back(buff[i]);
        }

    }
    ret.newHandler = this;
    return ret;
}
/// <summary>
/// function that return the players in requested room
/// </summary>
RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo info)
{
    RequestResult ret;
    GetPlayersInRoomResponse res;
    if (isRequestRelevant(info))
    {
        GetPlayersInRoomRequest req = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(info.buffer);
        roomMtx.lock();
        res.players = this->m_roomManager.getPlayersInRoom(req.roomId);//get all the players in the room
        roomMtx.unlock();
        Buffer buff = JsonResponsePacketSerializer::serializeResponse(res);
        for (int i = 0; i < buff.size(); i++)
        {
            ret.buffer.push_back(buff[i]);
        }
    }
    ret.newHandler = this;
    return ret;
}

/// <summary>
/// function that return personal stats of user
/// </summary>
RequestResult MenuRequestHandler::getPersonalStats(RequestInfo info)
{
    RequestResult ret;
    getPersonalStatsResponse res;
    if (isRequestRelevant(info))
    {
        vector<std::string> stats = this->m_statisticsManager.getUserStatistics(this->m_user.getUsername());//get the statistics of user
        res.statistics = stats;
        Buffer buff = JsonResponsePacketSerializer::serializeResponse(res);
        for (int i = 0; i < buff.size(); i++)
        {
            ret.buffer.push_back(buff[i]);
        }
    }
    ret.newHandler = this;
    return ret;
}

/// <summary>
/// function that return the top 5 score players
/// </summary>
RequestResult MenuRequestHandler::getHighScore(RequestInfo info)
{
    RequestResult ret;
    getHighScoreResponse res;
    res.status = 0;
    if (isRequestRelevant(info))
    {
        res.statistics = this->m_statisticsManager.getHighScore();
        res.status = 1;
        Buffer buff = JsonResponsePacketSerializer::serializeResponse(res);
        for (int i = 0; i < buff.size(); i++)
        {
            ret.buffer.push_back(buff[i]);
        }
    }
    ret.newHandler = this;
    return ret;
}

/// <summary>
/// function that join a user to room
/// </summary>
RequestResult MenuRequestHandler::joinRoom(RequestInfo info)
{
    RequestResult ret;
    JoinRoomResponse res;
    Buffer buff;
    res.status = 0;
    if (isRequestRelevant(info))
    {
        res.status = 1;
        JoinRoomRequest req = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);
        if (this->m_roomManager.getRoom(req.roomId).getRoomData().maxPlayers > this->m_roomManager.getRoom(req.roomId).getAllUsers().size())//check if there is still place in the room
        {
            roomMtx.lock();
            this->m_roomManager.addToRoom(this->m_user, req.roomId);
            roomMtx.unlock();
            buff = JsonResponsePacketSerializer::serializeResponse(res);
            ret.newHandler = this->m_handlerFactory.createRoomMemberRequestHandler(this->m_roomManager.getRoom(req.roomId), this->m_user);//set the handler to room member handler
        }
        else
        {
            ErrorResponse err;
            err.message = "No space in room";
            buff = JsonResponsePacketSerializer::serializeResponse(err);
            ret.newHandler = this;
        }
        for (int i = 0; i < buff.size(); i++)
        {
            ret.buffer.push_back(buff[i]);
        }
        
    }
    else
    {
        ret.newHandler = this;
    }
    return ret;
}

/// <summary>
/// function that creates room
/// </summary>
RequestResult MenuRequestHandler::createRoom(RequestInfo info)
{
    RequestResult ret;
    CreateRoomResponse res;
    Buffer buff;
    res.status = 0;
    if (isRequestRelevant(info))
    {
        
        res.status = 1;
        CreateRoomRequest req = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer , this->m_roomManager.getRooms().size());
        if (!this->m_roomManager.doesNameExist(req.data.name))//check if name of room is not exist
        {
            roomMtx.lock();//we access the room list so we need to lock the access to it
            Room room(this->m_user, req.data);
            room.addUser(this->m_user);
            this->m_roomManager.createRoom(room);
            roomMtx.unlock();
            buff = JsonResponsePacketSerializer::serializeResponse(res);
            ret.newHandler = this->m_handlerFactory.createRoomAdminRequestHandler(room, this->m_user);
        }
        else
        {
            ErrorResponse err;
            err.message = "Name is taken";
            buff = JsonResponsePacketSerializer::serializeResponse(err);
            ret.newHandler = this;
        }
        for (int i = 0; i < buff.size(); i++)
        {
            ret.buffer.push_back(buff[i]);
        }
    }
    else
    {
        ret.newHandler = this;
    }
    return ret;
}




/// <summary>
/// function that check if the request in relevant according to the request codes
/// </summary>
bool MenuRequestHandler::isRequestRelevant(RequestInfo info)
{
    return (info.requestId == LOGOUT_CODE) || (info.requestId == CREATEROOM_CODE) || (info.requestId == GETROOM_CODE) || (info.requestId == JOINROOM_CODE) || (info.requestId == GETPLAYERINROOM_CODE) || (info.requestId == GET_HIGH_SCORE) || (info.requestId == GET_PERSON_STATS);
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo info)
{
    switch (info.requestId)
    {
        case(GETUSERSTATS_CODE):
        {
            return getPersonalStats(info);
            break;
        }
        case(GETHIGHSCORE_CODE):
        {
            return getHighScore(info);
            break;
        }
        case(CREATEROOM_CODE):
        {
            return createRoom(info);
            break;
        }
        case(JOINROOM_CODE):
        {
            return joinRoom(info);
            break;
        }
        case(GETPLAYERINROOM_CODE):
        {
            return getPlayersInRoom(info);
            break;
        }
        case(GETROOM_CODE):
        {
            return getRooms(info);
            break;
        }

        case(LOGOUT_CODE):
        {
            return signout(info);
            break;
        }
        default:
        {
            RequestResult ret;
            ErrorResponse err;
            err.message = "Error";
            ret.buffer = JsonResponsePacketSerializer::serializeResponse(err);
            ret.newHandler= this;
            return ret;
        }
    }
    //return error message
    RequestResult ret;
    ErrorResponse err;
    err.message = "Invalid request";
    ret.buffer = JsonResponsePacketSerializer::serializeResponse(err);
    ret.newHandler = this;
    return ret;
}
