#pragma once

#include <iostream>
#include <vector>
#include "ResponseStructs.h"
#include <map>
#include "json.hpp"

#define LENGTH_MSG_BYTES 4

using json = nlohmann::json;
using std::string;
using Buffer = std::vector<unsigned char>;

// this enum stores the codes for each response type
enum resCodes
{
	GETUSERSTATS_CODE = 10,
	GETHIGHSCORE_CODE = 11,
	CREATEROOM_CODE = 20,
	CLOSEROOM_CODE = 21,
	STARTGAME_CODE = 22,
	GETROOMSTATE_CODE = 23,
	LEAVEROOM_CODE = 24,
	LEAVEGAME_CODE = 25,
	GETQUESTION_CODE=26,
	SUBMITANSWER_CODE = 27,
	GETGAMERESULTS_CODE =28,
	JOINROOM_CODE = 30,
	GETPLAYERINROOM_CODE = 40,
	GETROOM_CODE = 50,
	LOGOUT_CODE = 60,
	ERROR_CODE = 70,
	LOGIN_CODE = 80,
	SIGNUP_CODE = 90
};

//class to handle responses in json format
class JsonResponsePacketSerializer
{
private:
	static Buffer pushToBuffer(int code, string jsonStr); // this function get code and json format string and return buffer with the information

public:
	static Buffer serializeResponse(ErrorResponse res);	 // override to the function, that returnes buffer with Error response type
	static Buffer serializeResponse(LoginResponse res);	 // override to the function, that returnes buffer with Login response type
	static Buffer serializeResponse(SignupResponse res); // override to the function, that returnes buffer with Signup response type
	static Buffer serializeResponse(LogoutResponse res);
	static Buffer serializeResponse(GetRoomResponse res);
	static Buffer serializeResponse(GetPlayersInRoomResponse res);
	static Buffer serializeResponse(JoinRoomResponse res);
	static Buffer serializeResponse(CreateRoomResponse res);
	static Buffer serializeResponse(getPersonalStatsResponse res);
	static Buffer serializeResponse(getHighScoreResponse res);
	static Buffer serializeResponse(CloseRoomResponse res);
	static Buffer serializeResponse(StartGameResponse res);
	static Buffer serializeResponse(GetRoomStateResponse res);
	static Buffer serializeResponse(LeaveRoomResponse res);
	static Buffer serializeResponse(GetGameResultsResponse res);
	static Buffer serializeResponse(SubmitAnswerResponse res);
	static Buffer serializeResponse(GetQuestionResponse res);
	static Buffer serializeResponse(LeaveGameResponse res);
};