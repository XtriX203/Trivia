#include "LoginRequestHandler.h"

/*
CONSTRUCTOR
The constructor init an object
input:		none
output:		none
*/
LoginRequestHandler::LoginRequestHandler(LoginManager& loginManager, RequestHandlerFactory& HandlerFactory) :m_handleFactory( HandlerFactory ), m_loginManager( loginManager ) { }

/// <summary>
/// fuction that create RequestResult struct with login response
/// </summary>
/// <param name="info"> struct that contains the information </param>
RequestResult LoginRequestHandler::login(RequestInfo info)
{
	RequestResult ret;
	LoginResponse logResponse;
	LoginRequest logRequest;
	Buffer buff;
	logResponse.status = 0;
	if (isRequestRelevant(info))
	{

		logRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer); // deserialize the buffer recieved by the client
		if (this->m_loginManager.login(logRequest.username, logRequest.password)) // login a user by his username and password (db)
		{
			logResponse.status = 1;
			buff = JsonResponsePacketSerializer::serializeResponse(logResponse); // serialize the buffer that will send to the client
			ret.newHandler = (IRequestHandler*)this->m_handleFactory.createMenuRequestHandler(LoggedUser(logRequest.username));
		}
		else
		{
			ErrorResponse err;
			err.message = "Error in login";
			buff = JsonResponsePacketSerializer::serializeResponse(err); // serialize the buffer that will send to the client
			ret.newHandler = this;
		}
		for (int i = 0; i < buff.size(); i++)
		{
			ret.buffer.push_back(buff[i]);
		}
	}
	
	return ret;
}

/// <summary>
/// fuction that create RequestResult struct with signin response
/// </summary>
/// <param name="info"> struct that contains the information </param>
RequestResult LoginRequestHandler::signup(RequestInfo info)
{
	RequestResult ret;
	SignupResponse signRes;
	signRes.status = 0;
	Buffer buff;
	if (isRequestRelevant(info))
	{	
		SignupRequest signReq = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);
		if (this->m_loginManager.signup(signReq.username, signReq.password, signReq.email) && (signReq.username != "") && (signReq.password != "") && (signReq.email != ""))//check if parameters are valid
		{
			signRes.status = 1;
			buff = JsonResponsePacketSerializer::serializeResponse(signRes);// serialize the buffer that will send to the client
		}
		else
		{
			ErrorResponse err;
			err.message = "Error in signup";
			buff = JsonResponsePacketSerializer::serializeResponse(err); // serialize the buffer that will send to the client with err message
		}
		for (int i = 0; i < buff.size(); i++)
		{
			ret.buffer.push_back(buff[i]);
		}
	}
	ret.newHandler = this;
	return ret;
}

/*
IS REQUEST RELEVANT
This method checks if the code is relevant - login or signup
input:		requestInfoStruct
output:		true (relevant) or false
*/
bool LoginRequestHandler::isRequestRelevant(const RequestInfo requestInfoStruct)
{
	return (requestInfoStruct.requestId == LOGIN_CODE) || (requestInfoStruct.requestId == SIGNUP_CODE);
}


/*
HANDLE REQUEST
This method receives RequestInfo and returns a response after serialization, including a Handler that handles the client.
input:		requestInfoStruct
output:		requestResultStruct
*/
RequestResult LoginRequestHandler::handleRequest(const RequestInfo requestInfoStruct)
{
	RequestResult requestResultStruct;
	ErrorResponse error;
	Buffer buffer;

	switch (requestInfoStruct.requestId)
	{
		case LOGIN_CODE:
			return(login(requestInfoStruct));

		case SIGNUP_CODE:
			return(signup(requestInfoStruct));
			
		default:
			error.message = "ERROR";
			buffer = JsonResponsePacketSerializer::serializeResponse(error);
			break;
	}
	requestResultStruct.newHandler = this;
	requestResultStruct.buffer = buffer;
	return requestResultStruct;
}
