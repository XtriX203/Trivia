#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include "json.hpp"
#include "Question.h"
#include <io.h>
#include <vector>
#include <mutex>

using json = nlohmann::json;
using std::cout;
using std::endl;
using std::vector;

class SqliteDataBase : public IDatabase
{
private:
	sqlite3* _db;

	static int callback(void* data, int argc, char** argv, char** azColName);
	static int callbackPushVec(void* data, int argc, char** argv, char** azColName);
	static int callbackQuestion(void* data, int argc, char** argv, char** azColName);

public:
	SqliteDataBase();																					// Constructor
	~SqliteDataBase();																					// Distructor

	virtual void openDB();
	virtual bool doesUserExist(const string username) const;											// Abstract method - this method checks if the user is exist
	virtual bool doesPasswordMatch(const string username, const string password) const;					// Abstract method - this method gets a password and a username, and checks if this is the password of the user
	virtual void addNewUser(const string username, const string password, const string email) const;	// Abstract method - this method adds to the database a new user
	virtual vector<Question> getQuestions(int numOfQuestions);
	virtual float getPlayerAverageAnswerTime(string name);
	virtual int getNumOfCorrectAnswers(string name);
	virtual int getNumOfPlayerGames(string name);
	virtual int getNumOfTotalAnswers(string name);
	virtual void addQuestionsWithJson(string jsonStr);
	virtual vector<string> getTopFive(void);
};
