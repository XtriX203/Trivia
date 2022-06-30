#pragma once
#pragma comment (lib,"ws2_32.lib") 

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <map>
#include <thread>
#include "RequestStructs.h"
#include "LoginRequestHandler.h"
#include <exception>
#include <ctype.h>
#include"JsonRequestPacketDeserializer.h"
#include "LoggedUser.h"
#include"JsonResponsePacketSerializer.h"
#include "MenuRequestHandler.h"
#include "SqliteDataBase.h"
#include "RequestHandlerFactory.h"

#define PORT 8876
#define FIRST_MSG "HELLO"
#define ASCII_VALUE_OF_THE_FIRST_VALID_CHAR 32
#define ASCII_VALUE_OF_THE_LAST_VALID_CHAR 126
#define LEN 5

using std::cout;
using std::endl;
using std::string;

class Communicator
{
private:
	SOCKET m_serverSocket;															// Server socket
	std::map<SOCKET, IRequestHandler*> m_clients;									// clients map. key - client socket, value - username
	RequestHandlerFactory& m_handlerFactory;


	void HandleNewClient(SOCKET sc);												// This method handle a new client
	void bindAndListen();															// This method is responsible for bind and listen 
	vector<unsigned char> ignoreUnvalidChars(const SOCKET sc, const int len) const;	// This method receives a msg from the client and return a string without unvalid chars
	static string vectorToString(const vector<unsigned char> v);					// This method gets a vetor and returns it as string
	vector<unsigned char> getData(const vector<unsigned char> buffer) const;		// This method gets the data from the buffer
	IDatabase* getDB();

public:
	Communicator(RequestHandlerFactory& handler);																	// Constructor
	~Communicator();																// Distructor
	void startHandleRequests();														// This method starts handle requests
};