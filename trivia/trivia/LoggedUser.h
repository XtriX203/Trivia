#pragma once

#include <iostream>
using std::string;

class LoggedUser
{
private:
	string m_username;

public:
	LoggedUser(const string username);	// Constructor
	~LoggedUser();						// Distructor

	void setUsername(string username);
	string getUsername() const;			// Getter method - m_username

	bool operator <(const LoggedUser& other) const;
	bool operator >(const LoggedUser& other) const;
	bool operator ==(const LoggedUser& other) const;
};