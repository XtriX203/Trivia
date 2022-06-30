#pragma once

#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <exception>
#include "RequestStructs.h"
#include "LoginManager.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "RequestHandlerFactory.h"

#define NO_ERRORS 1

class IRequestHandler;
class RequestHandlerFactory;

class LoginRequestHandler: public IRequestHandler
{
private:
	LoginManager& m_loginManager;			
	RequestHandlerFactory& m_handleFactory;

	RequestResult login(RequestInfo info);															// 	fuction that create RequestResult struct with login response					
	RequestResult signup(RequestInfo info);															// 	fuction that create RequestResult struct with signin response					
public:
	LoginRequestHandler(LoginManager& loginManager, RequestHandlerFactory& HandlerFactory);	// 
	virtual bool isRequestRelevant(const RequestInfo requestInfoStruct);							// This method checks if the code is relevant - login or signup
	virtual RequestResult handleRequest(const RequestInfo requestInfoStruct);						// Abstract method - This method receives RequestInfo and returns a response after serialization, including a Handler that handles the client.
};