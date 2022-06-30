#pragma once
#include <iostream>
#include <vector>
#include "IDatabase.h"

using std::vector;
using std::string;
using std::to_string;


enum statCodes
{
	GET_HIGH_SCORE = 10,
	GET_PERSON_STATS = 11
};

class StatisticsManager
{
private:
	IDatabase* m_database;

public:
	StatisticsManager(IDatabase* db);							// Constructor
	~StatisticsManager();										// Distructor

	vector<string> getHighScore(void);							// This method returns the names of the top 5 players
	vector<string> getUserStatistics(const string username);	// This method gets all the statisctics of the user (avg time, num of correct ansers, num of total answers and num of games)
};
