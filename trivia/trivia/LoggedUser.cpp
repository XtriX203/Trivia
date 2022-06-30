#include "LoggedUser.h"

/*
CONSTRUCTOR
The constructor init an object
input:		none
output:		none
*/
LoggedUser::LoggedUser(const string username): m_username(username) { }

/*
DISTRUCTOR
This method releases dynamically allocated memory
input:		none
output:		none
*/
LoggedUser::~LoggedUser() { }

/*
GET USERNAME
Getter method - m_username
input:		none
output:		m_username
*/
void LoggedUser::setUsername(string username)
{
	this->m_username = username;
}
string LoggedUser::getUsername() const { return this->m_username; }

bool LoggedUser::operator<(const LoggedUser& other) const { return this->m_username < other.m_username; }

bool LoggedUser::operator>(const LoggedUser& other) const { return this->m_username > other.m_username; }

bool LoggedUser::operator==(const LoggedUser& other) const { return this->m_username == other.m_username; }