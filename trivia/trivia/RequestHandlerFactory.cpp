#include "RequestHandlerFactory.h"

//constractor to RequestHandlerFactory class that recieve database pointer and initialize the veriables of the class
RequestHandlerFactory::RequestHandlerFactory(IDatabase* db):m_database(db), m_StatisticsManager(db),m_loginManager(db), m_roomManager(), m_gameManager(db)
{
}



/// <summary>
/// function that create a LoginRequestHandler object and return it
/// </summary>
/// <returns> LoginRequestHandler object </returns>
LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(this->m_loginManager, *this);
}

/// <summary>
/// Getter method - m_loginManager
/// </summary>
/// <returns> m_loginManager </returns>
LoginManager& RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}

/// <summary>
/// getter method- dataBase
/// </summary>
IDatabase* RequestHandlerFactory::getDB()
{
    return this->m_database;
}

/// <summary>
/// create new menu handler
/// </summary>
/// <param name="user">the user that ownes the handler</param>
MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
    return new MenuRequestHandler(user, this->m_roomManager,*this,this->m_StatisticsManager);
}
/// <summary>
/// getter method- statistics manager
/// </summary>
StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return this->m_StatisticsManager;
}

/// <summary>
/// getter to room manager
/// </summary>
/// <returns></returns>
RoomManager& RequestHandlerFactory::getRoomManager()
{
    return this->m_roomManager;
}
/// <summary>
/// create new room admin handler
/// </summary>
RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(Room room, LoggedUser user)
{
    return new RoomAdminRequestHandler(user,this->m_roomManager,*this,room);
}

/// <summary>
/// create new room member handler
/// </summary>
RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(Room room, LoggedUser user)
{
    return new RoomMemberRequestHandler(room,user, this->m_roomManager,*this);
}

GameManager& RequestHandlerFactory::getGameManager()
{
    return this->m_gameManager;
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(Game game, LoggedUser user)
{
    return new GameRequestHandler(user,game,*this, this->getGameManager());
}
