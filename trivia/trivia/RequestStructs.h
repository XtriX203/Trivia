#pragma once

#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include "Room.h"

using std::string;
using std::vector;
using Buffer = vector<unsigned char>;
class IRequestHandler;

/*
STRUCT:		LOGIN REQUEST
This struct contains the information needed to login
string:		username
string:		password
*/
typedef struct LoginRequest
{
	string username;
	string password;
} LoginRequest;

/*
STRUCT:		SINGIP REQUEST
This struct contains the information needed to signup
string:		username
string:		password
string:		email
*/
typedef struct SignupRequest
{
	string username;
	string password;
	string email;
}SignupRequest;

/*
STRUCT:		REQUEST INFO
This struct contains the request information
int:		request id (code)
string:		receivalTime
vector:		buffer
*/
typedef struct RequestInfo
{
	int requestId;
	string receivalTime;
	vector<unsigned char> buffer;
} RequestInfo;

/*
STRUCT: REQUEST RESULT
This sturct contains the request result (data and hendler)
vector:				buffer
IRequestHandler*:	newHandler
*/
typedef struct RequestResult
{
	vector<unsigned char> buffer;
	IRequestHandler* newHandler = 0;
} RequestResult;

typedef struct GetPlayersInRoomRequest
{
	unsigned int roomId;
} GetPlayersInRoomRequest;

typedef struct JoinRoomRequest
{
	unsigned int roomId;
} JoinRoomRequest;

typedef struct CreateRoomRequest
{
	RoomData data;
} CreateRoomRequest;

typedef struct SubmitAnswerRequest
{
	int answerId;
} SubmitAnswerRequest;

/* I REQUEST HANDLER CLASS */
class IRequestHandler {

public:
	virtual bool isRequestRelevant(const RequestInfo requestInfoStruct) = 0;		// Abstract method - This method checks if the code is relevant - login or signup
	virtual RequestResult handleRequest(const RequestInfo requestInfoStruct) = 0;	// Abstract method - This method receives RequestInfo and returns a response after serialization, including a Handler that handles the client.
};