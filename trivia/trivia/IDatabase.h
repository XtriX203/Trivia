#pragma once
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "Question.h"

using std::string;
using std::vector;

class IDatabase
{
public:

	virtual void openDB() = 0;
	virtual bool doesUserExist(const string username) const = 0;											// Abstract method - this method checks if the user is exist
	virtual bool doesPasswordMatch(const string username, const string password) const = 0;					// Abstract method - this method gets a password and a username, and checks if this is the password of the user
	virtual void addNewUser(const string username, const string password , const string email) const = 0;	// Abstract method - this method adds to the database a new user
	virtual vector<Question> getQuestions(int) = 0;
	virtual float getPlayerAverageAnswerTime(string) = 0;
	virtual int getNumOfCorrectAnswers(string) = 0;
	virtual int getNumOfPlayerGames(string) = 0;
	virtual int getNumOfTotalAnswers(string) = 0;
	virtual void addQuestionsWithJson(string jsonStr)=0;
	virtual vector<string> getTopFive(void) = 0;
};
