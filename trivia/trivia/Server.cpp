#include "Server.h"

/*
CONSTRUCTOR
The constructor init an object
input:		none
output:		none
*/
Server::Server():m_database(new SqliteDataBase()), m_handlerFactory(this->m_database),m_communicator(this->m_handlerFactory) { }

/*
DISTRUCTOR
This method releases dynamically allocated memory
input:		none
output:		none
*/
Server::~Server() { }

/*
RUN
This method handles the client
input:		none
output:		none
*/
void Server::run()
{
	try
	{
		this->m_database->openDB();
		
		int cmdCommand =1;
		std::thread t_connector(&Communicator::startHandleRequests, this->m_communicator);
		t_connector.detach();
		while (cmdCommand != 0) {
		
			std::cout << "Options:\n[1] add questions(string in json format)\n[0] Exit\n";
			std::cin >> cmdCommand;
			handleMenu(cmdCommand);
			if (std::cin.fail())
			{
				throw string("error in cin");
			}
		}
	}
	catch (std::exception& e) { std::cout << "Error occured: " << e.what() << std::endl; };
	system("pause");
}

void Server::handleMenu(int choice)
{

	switch (choice)
	{
	case(1):
	{
		std::string str;
		std::cout << "Enter json formated string of questions:\nlike here:(https://opentdb.com/api.php?amount=10&difficulty=easy&type=multiple)\n";

		//this cin is only get the first 105 chars of input and its cause a problem

		//std::cin >> str;
		//std::cin.ignore(); // clean buffer
		getchar();
		std::getline(std::cin, str);

		try {
			this->m_database->addQuestionsWithJson(str);
		}
		catch (std::exception e)
		{
			std::cout << e.what();
		}
		break; 
		
	}
	}
}
