#pragma once
#include <iostream>
#include "Room.h"
#include <vector>
#include <map>
using std::vector;

/*
STRUCT:		LOGIN RESPONSE

int:		status
*/
typedef struct LoginResponse
{
	unsigned int status;
} LoginResponse;

/*
STRUCT:		SIGNUP RESPONSE

int:		status
*/
typedef struct SignupResponse
{
	unsigned int status;
} SignupResponse;

/*
STRUCT:		ERROR RESPONSE

string:		message
*/
typedef struct ErrorResponse
{
	std::string message;
} ErrorResponse;

typedef struct LogoutResponse
{
	unsigned int status;
} LogoutResponse;

typedef struct GetRoomResponse
{
	unsigned int status;
	vector<RoomData> rooms;
} GetRoomResponse;

typedef struct GetPlayersInRoomResponse
{
	vector<std::string> players;
} GetPlayersInRoomResponse;

typedef struct getHighScoreResponse
{
	unsigned int status;
	vector<std::string> statistics;
} getHighScoreResponse;

typedef struct getPersonalStatsResponse
{
	unsigned int status;
	vector<std::string> statistics;
} getPersonalStatsResponse;

typedef struct JoinRoomResponse
{
	unsigned int status;
} JoinRoomResponse;

typedef struct CreateRoomResponse
{
	unsigned int status;
} CreateRoomResponse;

typedef struct CloseRoomResponse
{
	unsigned int status;
} CloseRoomResponse;

typedef struct StartGameResponse
{
	unsigned int status;
} StartGameResponse;

typedef struct LeaveRoomResponse
{
	unsigned int status;
} LeaveRoomResponse;

typedef struct GetRoomStateResponse
{
	unsigned int status;
	bool hasGameBegun;
	vector<string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
} GetRoomStateResponse;

typedef struct LeaveGameResponse
{
	unsigned int status;
}LeaveGameResponse;

struct GetQuestionResponse
{
	unsigned int status;
	std::string question;
	std::map<unsigned int,std::string> answers;
};


struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnswerId;
};


struct PlayerResults
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
};


struct GetGameResultsResponse
{
	unsigned int status;
	vector<PlayerResults> results;
};


