#pragma once
#include <iostream>
#include <thread>
#include "Communicator.h"
#include "IDatabase.h"
#include "RequestHandlerFactory.h"
#include "SqliteDataBase.h"

class Server
{
private:
	Communicator m_communicator;
	IDatabase* m_database;
	RequestHandlerFactory m_handlerFactory;


public:
	Server();	// Constructor
	~Server();	// Distructor
	void run();	// This method handles the client
	void handleMenu(int choice);//function that handle the admin options
};