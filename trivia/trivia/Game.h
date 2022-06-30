#pragma once
#include <iostream>
#include "Question.h"
#include <map>
#include <vector>
#include "LoggedUser.h"
#include "Room.h"
#include <time.h>
#include <ctime>

using std::string;
using std::vector;
using std::map;

struct GameData
{
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averangeAnswerTime;
};

class Game
{
private:
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData>m_players;
	int m_id;
	clock_t m_start;
	static int _counter;

public:
	Game(const vector<Question> questions, const map<LoggedUser, GameData> players);
	Question getQuestionForUser(const LoggedUser user);
	void submitAnswer(const LoggedUser user, const std::string answer);
	void removePlayer(const LoggedUser user);
	int getID(void);
	GameData returnGameData(LoggedUser user);
	map<LoggedUser, GameData> getPlayers();
};