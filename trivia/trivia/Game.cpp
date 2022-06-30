#include "Game.h"

int Game::_counter = 0;

Game::Game(const vector<Question> questions, const map<LoggedUser, GameData> players) : m_questions(questions), m_players(players), m_id(_counter), m_start(clock()) { Game::_counter++; }

//getter to the current question of user
Question Game::getQuestionForUser(const LoggedUser user) { return this->m_players[user].currentQuestion; }

/// <summary>
/// method that get answwr and check if it true of wrong
/// </summary>
void Game::submitAnswer(const LoggedUser user, const std::string answer)
{
	(this->m_players[user].currentQuestion.getCorrectAnswer() == answer) ? this->m_players[user].correctAnswerCount++ : this->m_players[user].wrongAnswerCount++;//increase the true answer or wrond answer paramneter depend on the answer
	if (this->m_players[user].correctAnswerCount + this->m_players[user].wrongAnswerCount < this->m_questions.size())//if its not the last question
	{
		this->m_players[user].currentQuestion = this->m_questions[this->m_players[user].correctAnswerCount + this->m_players[user].wrongAnswerCount];//set the next question of the user
	}
	else this->m_players[user].averangeAnswerTime = (int(clock() - this->m_start) / CLOCKS_PER_SEC / (this->m_players[user].correctAnswerCount+this->m_players[user].wrongAnswerCount));//get avarage time per qustion
}

//remove player from game
void Game::removePlayer(const LoggedUser user)
{ 
	this->m_players.erase(this->m_players.find(user));
}

//getter to room id
int Game::getID(void) { return this->m_id; }

//getter to game data
GameData Game::returnGameData(LoggedUser user) { return this->m_players[user]; }

//getter to results of players
map<LoggedUser, GameData> Game::getPlayers()
{
	return this->m_players;
}
