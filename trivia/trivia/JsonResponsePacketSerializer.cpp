#include "JsonResponsePacketSerializer.h"

/// <summary>
/// this function get code and json format string and return buffer with the information
/// </summary>
/// <param name="code">integer that represent the response type
/// <param name="jsonStr">string in json type
Buffer JsonResponsePacketSerializer::pushToBuffer(int code, string jsonStr)
{
    Buffer msg;

    string strCode = std::to_string(code); //convert the code to string
    for (int i = 0; i < strCode.length(); i++)
    { //push each code digit
        msg.push_back(strCode[i]);
    }

    string len = std::to_string(jsonStr.length()); //convert the length of the message to tring
    for (int i = 0; i < LENGTH_MSG_BYTES - len.length(); i++)
    { //push zeros before the length if the length is less then 4 bytes
        msg.push_back('0');
    }

    for (int i = 0; i < len.length(); i++)
    { //push each length digit
        msg.push_back(len[i]);
    }

    for (int i = 0; i < jsonStr.length(); i++)
    { //push each character from the message to the vector
        msg.push_back((unsigned char)jsonStr[i]);
    }
    return msg;
}

/// <summary>
/// override to the function, that returnes buffer with Error response type
/// </summary>
/// <param name="res"> struct that contains error message</param>
Buffer JsonResponsePacketSerializer::serializeResponse(ErrorResponse res)
{
    json j;                            //create object of json
    j["message"] = res.message;        //create a key named message
    std::string jsonString = j.dump(); //dump the json object to string

    return pushToBuffer(ERROR_CODE, jsonString);
}

/// <summary>
/// override to the function, that returnes buffer with Login response type
/// </summary>
/// <param name="res"> struct that contains login status</param>
Buffer JsonResponsePacketSerializer::serializeResponse(LoginResponse res)
{
    json j;
    j["status"] = res.status;
    std::string jsonString = j.dump();

    return pushToBuffer(LOGIN_CODE, jsonString);
}

/// <summary>
/// override to the function, that returnes buffer with Signup response type
/// </summary>
/// <param name="res"> struct that contains signup status</param>
Buffer JsonResponsePacketSerializer::serializeResponse(SignupResponse res)
{
    json j;
    j["status"] = res.status;
    std::string jsonString = j.dump();

    return pushToBuffer(SIGNUP_CODE, jsonString);
}

/// <summary>
/// override to the function, that returnes buffer with Logout response type
/// </summary>
/// <param name="res"> struct that contains logout status</param>
Buffer JsonResponsePacketSerializer::serializeResponse(LogoutResponse res)
{
    json j;
    j["status"] = res.status;
    std::string jsonString = j.dump();

    return pushToBuffer(LOGOUT_CODE, jsonString);
}

/// <summary>
/// override to the function, that returnes buffer with GetRoom response type
/// </summary>
/// <param name="res"> struct that contains rooms status and data</param>
Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomResponse res)
{
    json j;
    json j2;
    std::string jsonString;
    j["status"] = res.status;
    j["rooms"];
    j2["id", "name", "maxPlayers", "numOfQuestionsInGame", "timePerQuestion", "isActive"];
    for (int i = 0; i < res.rooms.size(); i++)
    {
        j2["id"] = res.rooms[i].id;
        j2["name"] = res.rooms[i].name;
        j2["isActive"] = res.rooms[i].isActive;
        j2["maxPlayers"] = res.rooms[i].maxPlayers;
        j2["numOfQuestionsInGame"] = res.rooms[i].numOfQuestionsInGame;
        j2["timePerQuestion"] = res.rooms[i].timePerQuestion;
        j["rooms"].push_back(j2);
    }
    jsonString = j.dump();
    return (pushToBuffer(GETROOM_CODE, jsonString));
}
/// <summary>
/// override to the function, that returnes buffer with players In Room response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse res)
{
    string str = "{\"PlayersInRoom\":\"";
    for (int i = 0; i < res.players.size(); i++)
    {
        str += res.players[i];
        if (i < res.players.size() - 1)
        {//we need to add this mark only if its not the last player
            str += " ";
        }
    }
    str += "\"}";

    return pushToBuffer(GETPLAYERINROOM_CODE, str);
}

/// <summary>
/// override to the function, that returnes buffer with join room response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse res)
{
    json j;
    j["status"] = res.status;
    std::string jsonString = j.dump();

    return pushToBuffer(JOINROOM_CODE, jsonString);
}

/// <summary>
/// override to the function, that returnes buffer with create room response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse res)
{
    json j;
    j["status"] = res.status;
    std::string jsonString = j.dump();

    return pushToBuffer(CREATEROOM_CODE, jsonString);
}

/// <summary>
/// override to the function, that returnes buffer with personal stats response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(getPersonalStatsResponse res)
{
    json j,j2;
    j["status"] = res.status;
    j["statistics"];
    //add the parameters
    j2["averageAnswerTime"] = std::stof(res.statistics[0]);
    j2["correctAnswers"] = stoi(res.statistics[1]);
    j2["totalAnswers"] = stoi(res.statistics[2]);
    j2["gamesCount"] = stoi(res.statistics[3]);
    j["statistics"].push_back(j2);
    return pushToBuffer(GETUSERSTATS_CODE, j.dump());
}

/// <summary>
/// override to the function, that returnes buffer with high score response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(getHighScoreResponse res)
{
    json j;
    j["status"]= res.status;
    j["names"];
    for (int i = 0; i < res.statistics.size(); i++)
    {
        j["names"] += res.statistics[i];
    }

    return pushToBuffer(GETHIGHSCORE_CODE,j.dump());
}

/// <summary>
/// override to the function, that returnes buffer with close room response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse res)
{
    json j;
    j["status"] = res.status;
    std::string jsonString = j.dump();

    return pushToBuffer(CLOSEROOM_CODE, jsonString);
}
/// <summary>
/// override to the function, that returnes buffer with start game response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(StartGameResponse res)
{
    json j;
    j["status"] = res.status;
    std::string jsonString = j.dump();

    return pushToBuffer(STARTGAME_CODE, jsonString);
}

/// <summary>
/// override to the function, that returnes buffer with get room state response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse res)
{
    json j;
    j["status"] = res.status;
    j["hasGameBegun"] = res.hasGameBegun;
    j["questionCount"] = res.questionCount;
    j["answerTimeout"] = res.answerTimeout;
    j["players"];
    for (int i = 0; i < res.players.size(); i++)
    {
        j["players"] += res.players[i];
    }
    std::string jsonString = j.dump();
    return pushToBuffer(GETROOMSTATE_CODE, jsonString);
}

/// <summary>
/// override to the function, that returnes buffer with leave room response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse res)
{
    json j;
    j["status"] = res.status;
    std::string jsonString = j.dump();

    return pushToBuffer(LEAVEROOM_CODE, jsonString);
}
/// <summary>
/// override to the function, that returnes buffer with leave game response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse res)
{
    json j;
    j["status"] = res.status;
    std::string jsonString = j.dump();
    return pushToBuffer(LEAVEGAME_CODE, jsonString);
}

/// <summary>
/// override to the function, that returnes buffer with game result response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(GetGameResultsResponse res)
{
    json j;
    j["status"] = res.status;

    j["results"];
    //push result of each player
    for (int i = 0; i < res.results.size(); i++)
    {
        json j2;
        j2["avarageAnswerTime"] = res.results[i].averageAnswerTime;
        j2["correctAnswerCount"] = res.results[i].correctAnswerCount;
        j2["username"] = res.results[i].username;
        j2["wrongAnswerCount"] = res.results[i].wrongAnswerCount;
        j["results"].push_back(j2);
    }
    std::string jsonString = j.dump();
    return pushToBuffer(GETGAMERESULTS_CODE, jsonString);
}
/// <summary>
/// override to the function, that returnes buffer with submit answer response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse res)
{
    json j;
    j["status"] = res.status;
    j["correctAnswerId"] = res.correctAnswerId;
    std::string jsonString = j.dump();
    return pushToBuffer(SUBMITANSWER_CODE, jsonString);
}

/// <summary>
/// override to the function, that returnes buffer with Get question response type
/// </summary>
Buffer JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse res)
{
    json j;
    j["status"] = res.status;
    j["question"] = res.question;
    j["answers"];
    for (std::map<unsigned int, std::string>::iterator i = res.answers.begin(); i != res.answers.end(); ++i)
    {//push each answer
        json j2;
        j2["id"] = i->first;
        j2["answer"] = i->second;
        j["answers"].push_back(j2);
    }
    std::string jsonString = j.dump();
    return pushToBuffer(GETQUESTION_CODE, jsonString);
}

