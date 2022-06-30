#include "GameRequestHandler.h"
std::mutex gameMtx;


GameRequestHandler::GameRequestHandler(LoggedUser user, Game game, RequestHandlerFactory& handlerFacroty, GameManager& gameManager)
    :m_game(game), m_gameManager(gameManager), m_handlerFacroty(handlerFacroty), m_user(user)
{
}

/// <summary>
/// get the current question of the player
/// </summary>
/// <param name="info"></param>
/// <returns></returns>
RequestResult GameRequestHandler::getQuestion(RequestInfo info)
{
    GetQuestionResponse res;
    Question q;
    RequestResult ret;
    gameMtx.lock();
    q = this->m_game.getQuestionForUser(this->m_user);//get the current question
    gameMtx.unlock();
    res.question = q.getQuestion();
    res.answers.insert(std::pair<unsigned int, std::string>(0, q.getCorrectAnswer()));//push the right answer to the answers map
    for (int i = 1; i < q.getPossibleAnswers().size(); i++)
    {
        res.answers.insert(std::pair<unsigned int, std::string>(i, q.getPossibleAnswers()[i]));//push each wrong answer
    }
    res.status = 1;
    Buffer buff = JsonResponsePacketSerializer::serializeResponse(res);
    for (int i = 0; i < buff.size(); i++)
    {
        ret.buffer.push_back(buff[i]);
    }
    ret.newHandler = this;
    return ret;
}

/// <summary>
/// submitting answer of user and return response
/// </summary>
RequestResult GameRequestHandler::submitAnswer(RequestInfo info)
{
    SubmitAnswerResponse res;
    RequestResult ret;
    SubmitAnswerRequest req = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(info.buffer);
    gameMtx.lock();
    this->m_game.submitAnswer(this->m_user, this->m_game.getQuestionForUser(this->m_user).getPossibleAnswers()[req.answerId]);//call the function that checks if the answer is true or wrong
    gameMtx.unlock();
    res.correctAnswerId = 0;//alweys the first answer is the true one 
    res.status = 1;
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(res);
    for (int i = 0; i < buffer.size(); i++)
    {
        ret.buffer.push_back(buffer[i]);
    }
    ret.newHandler = this;
    return ret;
}

/// <summary>
/// method to get the results of all users in the end of game
/// </summary>
RequestResult GameRequestHandler::getGameResults(RequestInfo info)
{
    RequestResult ret;
    GetGameResultsResponse res;
    GameData data;
    PlayerResults playerRes;
    map<LoggedUser, GameData> players = this->m_game.getPlayers();//get all players reults

    for (map< LoggedUser, GameData >::iterator it = players.begin(); it != players.end(); it++)
    {//push results of each user to results vector
        data = this->m_game.returnGameData(it->first);
        playerRes.username = it->first.getUsername();
        playerRes.averageAnswerTime = data.averangeAnswerTime;
        playerRes.correctAnswerCount = data.correctAnswerCount;
        playerRes.wrongAnswerCount = data.wrongAnswerCount;
        res.results.push_back(playerRes);
    }
    res.status = 1;
    Buffer buff = JsonResponsePacketSerializer::serializeResponse(res);
    for (int i = 0; i < buff.size(); i++)
    {
        ret.buffer.push_back(buff[i]);
    }
    ret.newHandler = this;
    return ret;
}

/// <summary>
/// method to leave game(by player)
/// </summary>
RequestResult GameRequestHandler::leaveGame(RequestInfo info)
{
    RequestResult ret;
    LeaveGameResponse res;
    gameMtx.lock();
    this->m_game.removePlayer(this->m_user);//remove the player from the game
    gameMtx.unlock();
    res.status = 1;
    Buffer buffer=JsonResponsePacketSerializer::serializeResponse(res);
    for (int i = 0; i < buffer.size(); i++)
    {
        ret.buffer.push_back(buffer[i]);
    }
    ret.newHandler = (IRequestHandler*)this->m_handlerFacroty.createMenuRequestHandler(this->m_user);//set the handler to menu page handler
    return ret;
}

bool GameRequestHandler::isRequestRelevant(RequestInfo info)
{
    return info.requestId==LEAVEGAME_CODE || info.requestId== GETGAMERESULTS_CODE || info.requestId== SUBMITANSWER_CODE || info.requestId == GETQUESTION_CODE;
}

RequestResult GameRequestHandler::handleRequest(RequestInfo info)
{
    if (isRequestRelevant(info))
    {
        switch (info.requestId)
        {
        case(LEAVEGAME_CODE):
        {
            return (leaveGame(info));
        }
        case(GETGAMERESULTS_CODE):
        {
            return getGameResults(info);
        }
        case(SUBMITANSWER_CODE):
        {
            return submitAnswer(info);
        }
        case (GETQUESTION_CODE):
        {
            return getQuestion(info);
        }
        }
    }
    return RequestResult();
}
