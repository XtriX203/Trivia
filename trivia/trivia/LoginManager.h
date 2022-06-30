#pragma once

#include <iostream>
#include <vector>
#include "IDatabase.h"
#include "RequestStructs.h"
#include <exception>
#include "json.hpp"
#include "SqliteDataBase.h"
#include "LoggedUser.h"

using std::vector;
using std::string;
using json = nlohmann::json;
using std::cout;
using std::endl;

class LoginManager
{
private:
	IDatabase* m_database;
	vector<LoggedUser> m_loggedUser;

public:
	LoginManager(IDatabase* database);														// Constructor
	LoginManager();																			// Constructor
	~LoginManager();																		// Distructor

	bool signup(const string username, const string password, const string email) const;	// This method signs up a user by his username, password and mail
	bool login(const string username, const string password);								// This method logs in a user by his username and password
	void logout(const string username);														// This method logs out a user by his username
};
