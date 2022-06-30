#include "LoginManager.h"

/*
CONSTRUCTOR
The constructor init an object
*/

/*
input:		database (pointer)
output:		none
*/
LoginManager::LoginManager(IDatabase* database): m_database(database) { }

/*
input:		none
output:		none
*/
LoginManager::LoginManager() { this->m_database = nullptr; }

/*
DISTRUCTOR
This method releases dynamically allocated memory
input:		none
output:		none
*/
LoginManager::~LoginManager()
{
	this->m_loggedUser.clear();
}

/*
SIGNUP
This method signs up a user by his username, password and mail
input:		username, password, email
output:		none
*/
bool LoginManager::signup(const string username, const string password, const string email) const
{	
	try {

		if (this->m_database->doesUserExist(username)) return false;
		this->m_database->addNewUser(username, password, email);
		return true;
	}
	catch (...) { }
}

/*
LOGIN 
This method logs in a user by his username and password
input:		username, password
output:		none
*/
bool LoginManager::login(const string username, const string password)
{	
	try
	{
		if (! this->m_database->doesUserExist(username))
		{
			return false;
		}
		if (!this->m_database->doesPasswordMatch(username, password))
		{
			return false;
		}
		//check if the user is already logged in
		for (std::vector<LoggedUser>::iterator it= this->m_loggedUser.begin(); it != this->m_loggedUser.end() ; ++it)
		{
			if (it->getUsername() == username)
			{
				return false;
			}
		}
		this->m_loggedUser.push_back(LoggedUser(username));
		return true;
	}
	catch (...) {}
}

/*
LOGOUT
This method logs out a user by his username
input:		username
output:		none
*/
void LoginManager::logout(const string username) 
{
	std::vector<LoggedUser>::iterator it;

	for (it = this->m_loggedUser.begin(); it != this->m_loggedUser.end(); ++it) {
		if (it->getUsername() == username)
		{
			this->m_loggedUser.erase(it);
			return;
		}
	}
	throw string("Error: there is not any user with this username");
	
}
