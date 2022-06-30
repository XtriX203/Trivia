#include "JsonRequestPacketDeserializer.h"

/*
CREATE REQUEST INFO OBJECT
This method read from the buffer and create an requestInfo object
input:      buffer
output:     a requestInfo object
*/
RequestInfo JsonRequestPacketDeserializer::createRequestInfoObject(const Buffer buffer)
{
    RequestInfo requestStruct;
    if (buffer.empty()) return requestStruct;
    int code = 0;
    string time = currentDateTime();
    int temp = 10;
    vector<unsigned char> data;
    
    for (int i = 0; i < 2; i++)
    {
        code += ((int)buffer[i] - 48) * temp;
        temp /= 10;
    }
    for (int i = 6; i < buffer.size(); i++) data.push_back(buffer[i]);

    // create object
    requestStruct.requestId = code; 
    requestStruct.receivalTime = time;
    requestStruct.buffer = data;
    return requestStruct;
}

/*
CURRRENT DATE TIME 
This method returns the current time
credit:     https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm
input:      none
ouput:      string (current time)
*/
string JsonRequestPacketDeserializer::currentDateTime(void)
{
    time_t now = time(nullptr);
    string a = ctime(&now);
    return a;
}

/*
DESERIALIZE LOGIN REQUEST
This method creates a LoginRequest object
input:      buffer
output:     LoginRequest object
*/
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer buffer)
{
    json j = json::parse(buffer.begin(), buffer.end());
    LoginRequest login;
    login.username = j["username"];
    login.password = j["password"];
    return login;
}

/*
DESERIALIZE LOGIN REQUEST
This method creates a SignupRequest object
input:      buffer
output:     SignupRequest object
*/
SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const Buffer buffer)
{
    json j = json::parse(buffer.begin(), buffer.end());
    SignupRequest signup;
    signup.email = j["email"];
    signup.password = j["password"];
    signup.username = j["username"];
    return signup;
}

/*
DESERIALIZE GET PLAYER REQUEST
This method creates a GetPlayersInRoomRequest object
input:      buffer
output:     GetPlayersInRoomRequest object
*/
GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(const Buffer buffer)
{
    json j = json::parse(buffer.begin(), buffer.end());
    GetPlayersInRoomRequest getPlayersInRoom;
    getPlayersInRoom.roomId = j["id"];
    return getPlayersInRoom;
}

/*
DESERIALIZE JOIN ROOM REQUEST
This method creates a JoinRoomRequest object
input:      buffer
output:     JoinRoomRequest object
*/
JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const Buffer buffer)
{
    json j = json::parse(buffer.begin(), buffer.end());
    JoinRoomRequest joinRoom;
    joinRoom.roomId = j["id"]; // or j["rooms"]["json"];
    return joinRoom;
}

/*
DESERIALIZE CREATE ROOM REQUEST
This method creates a CreateRoomRequest object
input:      buffer
output:     CreateRoomRequest object
*/
CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const Buffer buffer , int id)
{
    json j = json::parse(buffer.begin(), buffer.end());
    CreateRoomRequest createRoom;
    createRoom.data.timePerQuestion= j["timePerQuestion"];
    createRoom.data.maxPlayers = j["maxPlayers"];
    createRoom.data.numOfQuestionsInGame= j["numOfQuestionInGame"];
    createRoom.data.name= j["name"];
    createRoom.data.isActive = 1;
    createRoom.data.id = id;
    return createRoom;
}

/*
DESERIALIZE SUBMIT ANSWER REQUEST
This method creates a SubmitAnswerRequest object
input:      buffer
output:     SubmitAnswerRequest object
*/
SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(const Buffer buffer)
{
    json j = json::parse(buffer.begin(), buffer.end());
    SubmitAnswerRequest submitAnswer;
    submitAnswer.answerId = j["answerId"];
    return submitAnswer;
}
