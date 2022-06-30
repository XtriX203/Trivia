#pragma once
#include <iostream>
#include "RequestHandlerFactory.h"
#include "GameManager.h"
#include "Game.h"
#include "RequestStructs.h"
#include <mutex>
class RequestHandlerFactory;

class GameRequestHandler : IRequestHandler
{
private:
	Game m_game;
	LoggedUser m_user;
	GameManager &m_gameManager;
	RequestHandlerFactory &m_handlerFacroty;

	RequestResult getQuestion(RequestInfo info);
	RequestResult submitAnswer(RequestInfo info);
	RequestResult getGameResults(RequestInfo info);
	RequestResult leaveGame(RequestInfo info);

public:
	GameRequestHandler(LoggedUser user, Game game, RequestHandlerFactory& handlerFacroty, GameManager& gameManager);
	virtual bool isRequestRelevant(RequestInfo info);
	virtual RequestResult handleRequest(RequestInfo info);
};