#include "GameManager.h"

GameManager::GameManager(IDatabase* db, vector<Game> games) : m_database(db), m_games(games) { }

GameManager::GameManager(IDatabase* db) : m_database(db) { }

/// <summary>
/// method that creates new game from given room
/// </summary>
/// <returns>object of the new game</returns>
Game GameManager::createGame(Room room)
{
	vector<Question> quetions = this->m_database->getQuestions(room.getRoomData().numOfQuestionsInGame);//get questions for this room
	vector<LoggedUser> users = room.getLoggedUsers();//get all users in the room
	map<LoggedUser, GameData> gameMap; 
	GameData data;
	data.averangeAnswerTime = 0;
	data.correctAnswerCount = 0;
	data.currentQuestion = quetions[0];
	data.wrongAnswerCount = 0;
	for (int i = 0; i < users.size(); i++) gameMap.insert(pair<LoggedUser, GameData>(users[i], data));//isert each user to the game
	return Game(this->m_database->getQuestions(room.getRoomData().numOfQuestionsInGame), gameMap);//return the new game object (call the constractor)
}


//delete game from room manager
void GameManager::deleteGame(Game game)
{
	vector<Game>::iterator it = this->m_games.begin();
	bool flag = true;
	//find the given game
	for (; flag && it != this->m_games.end(); it++) flag = it->getID() != game.getID();
	it--;
	this->m_games.erase(it);
}

