#include "StatisticsManager.h"

/*
CONSTRUCTOR
The constructor init an object
input:		ptr db
output:		none
*/
StatisticsManager::StatisticsManager(IDatabase* db) : m_database(db) { }

/*
DISTRUCTOR
This method releases dynamically allocated memory
input:		none
output:		none
*/
StatisticsManager::~StatisticsManager() { delete this->m_database; }

/*
GET HIGH SCORE
This method returns the names of the top 5 players
input:      none
output:     names vector
*/
vector<string> StatisticsManager::getHighScore(void) { return this->m_database->getTopFive(); }

/*
GET USER STATISTICS
This method gets all the statisctics of the user (avg time, num of correct ansers, num of total answers and num of games)
input:      username
output:     vector with all the statisctics
*/
vector<string> StatisticsManager::getUserStatistics(const string username)
{
    vector<string> statistics;
    //push each statistic of player to vector 
    statistics.push_back(to_string(this->m_database->getPlayerAverageAnswerTime(username)));
    statistics.push_back(to_string(this->m_database->getNumOfCorrectAnswers(username)));
    statistics.push_back(to_string(this->m_database->getNumOfTotalAnswers(username)));
    statistics.push_back(to_string(this->m_database->getNumOfPlayerGames(username)));
    return statistics;
}
