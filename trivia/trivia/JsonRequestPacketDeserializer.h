#pragma once

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include "RequestStructs.h"
#include "json.hpp"
#include <vector>
#include <string>

using std::vector;
using std::memcpy;
using std::int32_t;
using json = nlohmann::json;
using std::stoi;

#define CODE_BYTES 1
#define LENGTH_MSG_BYTES 4

class JsonRequestPacketDeserializer
{
private:
	static string currentDateTime(void);									// This method returns the current time 

public:
	static RequestInfo createRequestInfoObject(const Buffer buffer);					// This method reads from the buffer and create an requestInfo object
	static LoginRequest deserializeLoginRequest(const Buffer buffer);					// This method creates a LoginRequest object
	static SignupRequest deserializeSignupRequest(const Buffer buffer);					// This method creates a SignupRequest object
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(const Buffer buffer);	// This method creates a GetPlayersInRoomRequest object
	static JoinRoomRequest deserializeJoinRoomRequest(const Buffer buffer);				// This method creates a JoinRoomRequest object
	static CreateRoomRequest deserializeCreateRoomRequest(const Buffer buffer ,int id);	// This method creates a CreateRoomRequest object
	static SubmitAnswerRequest deserializeSubmitAnswerRequest(const Buffer buffer);		// This method creates a SubmitAnswerRequest object
};
