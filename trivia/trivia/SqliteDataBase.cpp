#include "SqliteDataBase.h"

string temp = "";
vector<string> vec;
std::mutex mtx;
vector<Question> questions;

int SqliteDataBase::callback(void* data, int argc, char** argv, char** azColName)
{
    temp = "";//empty the temp variable if didn't get result
    temp = argv[0];
    return 0;
}

int SqliteDataBase::callbackPushVec(void* data, int argc, char** argv, char** azColName)
{
    vec.push_back(argv[0]);
    return 0;
}

/// <summary>
/// callbeck function to get question
/// </summary>
int SqliteDataBase::callbackQuestion(void* data, int argc, char** argv, char** azColName)
{
    Question q;
    for (int i = 0; i < argc; i++)
    {
        //add each parameter to question object
        if (string(azColName[i]) == "question") q.setQuestion(argv[i]);
        else if (string(azColName[i]) == "correct_answer") q.addAnswer(argv[i]);
        else if (string(azColName[i]) == "incorrect_answer_1") q.addAnswer(argv[i]);
        else if (string(azColName[i]) == "incorrect_answer_2") q.addAnswer(argv[i]);
        else if (string(azColName[i]) == "incorrect_answer_3") q.addAnswer(argv[i]);
    }
    //push each question to the global questions vector
    questions.push_back(q);
    return 0;
}

/*
CONSTRUCTOR
The constructor inits an object
input:		none
output:		none
*/
SqliteDataBase::SqliteDataBase()
{
    this->_db = nullptr;
}

/*
DISTRUCTOR
This method releases dynamically allocated memory
input:		none
output:		none
*/
SqliteDataBase::~SqliteDataBase()
{
    sqlite3_close(this->_db); // close the database
    this->_db = nullptr;
}

/// <summary>
/// method that opens the db
/// </summary>
void SqliteDataBase::openDB()
{
    string dbFileName = "triviaDB.db";
    int doesFileExist = _access(dbFileName.c_str(), 0);
    int res = sqlite3_open(dbFileName.c_str(), &this->_db); // open the database

    if (res != SQLITE_OK)
    {
        this->_db = nullptr;
        throw string("Failed to open DB\n");
    }

    if (doesFileExist == -1)
    {
        //if file not exist create all new tables
        string sqlStatement = "CREATE TABLE users ( username TEXT NOT NULL, password TEXT NOT NULL, email TEXT NOT NULL);";
        sqlStatement += "CREATE TABLE questions(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
                        question TEXT NOT NULL,\
                        correct_answer TEXT NOT NULL,\
                        incorrect_answer_1 TEXT NOT NULL\
                        ,incorrect_answer_2 TEXT NOT NULL, incorrect_answer_3 TEXT NOT NULL);";
        sqlStatement += "CREATE TABLE statistics (\
            user_id	INTEGER NOT NULL, \
            games_count	INTEGER, \
            answers	INTEGER, \
            correct_answers	INTEGER, \
            total_time	REAL, \
            FOREIGN KEY(user_id) REFERENCES users(id) \
            );";
        char** errMessage = nullptr;
        res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, errMessage);
    }
    else
    {
        std::cout << "all ok";
    }
}

/*
DOES USER EXIST
Abstract method - this method checks if the user is exist
input:      username
output:     true or false
*/
bool SqliteDataBase::doesUserExist(const string username) const
{
    temp = "";//empty the temp variable if didn't get result
    char* errMessage = nullptr;
    mtx.lock();
    int res = sqlite3_exec(this->_db, (("SELECT username FROM users WHERE username = '" + username + "';").c_str()), callback, nullptr, &errMessage);
    if (res != SQLITE_OK) throw string("error");
    mtx.unlock();
    return temp != "";
}

/*
DOES PASSWORD MATCH
Abstract method - this method gets a password and a username, and checks if this is the password of the user
input:      username, password
output:     true or false
*/
bool SqliteDataBase::doesPasswordMatch(const string username, const string password) const
{
    temp = "";//empty the temp variable if didn't get result
    char* errMessage = nullptr;
    mtx.lock();
    int res = sqlite3_exec(this->_db, (("SELECT password FROM users WHERE username = '" + username + "';").c_str()), callback, nullptr, &errMessage);
    if (res != SQLITE_OK) throw string("could not find your password");
    mtx.unlock();
    return temp == password;
}

/*
ADD NEW USER
Abstract method - this method adds to the database a new user
input:      username, password, email
output:     none
*/
void SqliteDataBase::addNewUser(const string username, const string password, const string email) const
{
    temp = "";//empty the temp variable if didn't get result
    char* errMessage = nullptr;
    mtx.lock();
    int res = sqlite3_exec(this->_db, (("INSERT INTO users (username, password, email) VALUES ('" + username + "', '" + password + "', '" + email + "');").c_str()), nullptr, nullptr, &errMessage);
    if (res != SQLITE_OK) throw string("could not add a new user");
    mtx.unlock();
}

vector<Question> SqliteDataBase::getQuestions(int numOfQuestions)
{
    std::vector<Question> ret;
    vec.clear();
    questions.clear();
    string sqlStatement = "SELECT question, correct_answer, incorrect_answer_1, incorrect_answer_2, incorrect_answer_3 FROM questions LIMIT " + std::to_string(numOfQuestions) + ";";
    char* errMessage = nullptr;
    mtx.lock();
    int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackQuestion, nullptr, &errMessage);
    mtx.unlock();
    ret = questions;
    return ret;
}



/// <summary>
/// function that return average time to answer of user from the DB
/// </summary>
/// <param name="name">name of the user</param>
float SqliteDataBase::getPlayerAverageAnswerTime(string name)
{
    float avrg = 0.0;
    try {
        temp = "";//empty the temp variable if didn't get result
        string sqlStatement = "SELECT total_time FROM statistics WHERE user_id=(SELECT id FROM users WHERE username=\"" + name + "\"); ";
        char* errMessage = nullptr;
        mtx.lock();
        int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callback, nullptr, &errMessage);
        if (res != SQLITE_OK) throw string("could not get time");

        avrg = std::stof(temp);
        temp = "";
        sqlStatement = "SELECT answers FROM statistics WHERE user_id = (SELECT id FROM users WHERE username = \"" + name + "\"); ";
        errMessage = nullptr;
        res = sqlite3_exec(this->_db, sqlStatement.c_str(), callback, nullptr, &errMessage);
        if (res != SQLITE_OK) throw string("could not get total answers");
        avrg /= std::stof(temp);
        mtx.unlock();
    }
    catch (std::exception& e) { std::cout << e.what() << endl; };
    return avrg;
}

/// <summary>
/// getter to to correct answers of user
/// </summary>
/// <param name="name">name of user</param>
/// <returns></returns>
int SqliteDataBase::getNumOfCorrectAnswers(string name)
{
    temp = "";//empty the temp variable if didn't get result
    string sqlStatement = "SELECT correct_answers FROM statistics WHERE user_id=(SELECT id FROM users WHERE username=\"" + name + "\"); ";
    char* errMessage = nullptr;
    mtx.lock();
    int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callback, nullptr, &errMessage);
    if (res != SQLITE_OK) throw string("could not get time");
    mtx.unlock();
    return std::stoi(temp);
}

/// <summary>
/// getter to num of games player played
/// </summary>
int SqliteDataBase::getNumOfPlayerGames(string name)
{
    temp = "";//empty the temp variable if didn't get result
    string sqlStatement = "SELECT games_count FROM statistics WHERE user_id=(SELECT id FROM users WHERE username=\"" + name + "\"); ";
    char* errMessage = nullptr;
    mtx.lock();
    int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callback, nullptr, &errMessage);
    if (res != SQLITE_OK) throw string("could not get time");
    mtx.unlock();
    return std::stoi(temp);
}

/// <summary>
/// getter to num of total answers of player
/// </summary>
/// <param name="name">name of player</param>
int SqliteDataBase::getNumOfTotalAnswers(string name)
{
    temp = "";//empty the temp variable if didn't get result
    string sqlStatement = "SELECT answers FROM statistics WHERE user_id=(SELECT id FROM users WHERE username=\"" + name + "\"); ";
    char* errMessage = nullptr;
    mtx.lock();
    int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callback, nullptr, &errMessage);
    if (res != SQLITE_OK) throw string("could not get time");
    mtx.unlock();
    return std::stoi(temp);
}


/// <summary>
/// function that get string in format of the website: https://opentdb.com/api_config.php
/// and convert in to json and then add the questions to the DB
/// </summary>
/// <param name="jsonStr">string that contains the json struct</param>
/// 
void SqliteDataBase::addQuestionsWithJson(string jsonStr)
{
    json j = json::parse(jsonStr);//parse the string to json object
    if (j["results"].is_array())//check if there are results
    {
        for (json res : j["results"])//run over all questions in array 'result'
        {
            string sqlStatement = "INSERT INTO questions(question, correct_answer, incorrect_answer_1, incorrect_answer_2, incorrect_answer_3) VALUES ('" + string(res["question"]);//add the question
            sqlStatement += "', '" + std::string(res["correct_answer"]);//add the correct answer
            for (json::iterator it = res["incorrect_answers"].begin(); it != res["incorrect_answers"].end(); ++it)
            {//run over all incorrect answers that are stores in an array
                sqlStatement += "', '" + string(*it);//add each answer to the string 
            }
            sqlStatement += "');";
            char* errMessage = nullptr;
            mtx.lock();
            int res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
            mtx.unlock();
            if (res != SQLITE_OK)
            {
                throw string("could not add question");
            }
        }
    }
}

/// <summary>
/// get top scorers
/// </summary>
/// <param name=""></param>
/// <returns></returns>
vector<string> SqliteDataBase::getTopFive(void)
{
    vec.clear();
    string sqlStatement = "SELECT users.username FROM users INNER JOIN statistics on users.id = statistics.user_id ORDER BY statistics.correct_answers DESC LIMIT 5; ";
    char* errMessage = nullptr;
    int res = sqlite3_exec(this->_db, sqlStatement.c_str(), callbackPushVec, nullptr, &errMessage);
    if (res != SQLITE_OK) throw string("could not get best sorers");
    return vec;
}
