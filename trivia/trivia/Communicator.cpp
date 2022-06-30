#include "Communicator.h"


/*
CONSTRUCTOR
The constructor init an object
input:		none
output:		none
*/
Communicator::Communicator(RequestHandlerFactory& handler):m_handlerFactory(handler)
{ 

	//// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	//// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_serverSocket == INVALID_SOCKET)
	{
		throw std::exception(__FUNCTION__ " - socket");
	}
}

/*
DISTRUCTOR
This method releases dynamically allocated memory
input:		none
output:		none
*/
Communicator::~Communicator()
{
	try
	{
		closesocket(this->m_serverSocket);
	}
	catch (...) {}
}

/*
START HANDLE REQUESTS
This method starts handle requests
input:		none
output:		none
*/
void Communicator::startHandleRequests()
{
	bindAndListen();
	while (true)
	{
		SOCKET client_socket = ::accept(m_serverSocket, NULL, NULL);

		if (client_socket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__);

		std::cout << "Client accepted. Server and client can speak" << std::endl;

		// the function that handle the conversation with the client
		std::thread t(&Communicator::HandleNewClient, this, client_socket);
		t.detach();
		
		
		m_clients.insert( std::pair<SOCKET, IRequestHandler*>  (client_socket, this->m_handlerFactory.createLoginRequestHandler()));
	}
}

/*
BIND AND LISTEN
This method is responsible for bind and listen 
input:		none
output:		none
*/
void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// again stepping out to the global namespace
	// Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	
	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << PORT << std::endl;
	
	std::cout << "Waiting for client connection request" << std::endl;
}

/*
HANDLE NEW CLIENT 
This method handle a new client
input:		sc
output:		none
*/
void Communicator::HandleNewClient(SOCKET sc)
{
	vector<unsigned char> buffer;
	RequestInfo requestInfoStruct;
	RequestResult requestResultStruct;

	
	string temp = "";
	bool flag = true;

	try
	{
		requestResultStruct.newHandler = this->m_handlerFactory.createLoginRequestHandler();
		while (true)
		{
			flag = true;
			temp = "";
			try
			{
				buffer = ignoreUnvalidChars(sc, LEN); //recieve the message of the client
				if (!buffer.empty())
				{
					requestInfoStruct = JsonRequestPacketDeserializer::createRequestInfoObject(buffer);
				}
				else send(sc, "ERROR", strlen("ERROR"), 0);
			}
			catch (...) { }
			if (!flag) requestInfoStruct.requestId = ERROR_CODE;
			requestResultStruct = requestResultStruct.newHandler->handleRequest(requestInfoStruct);//send the info to handleRequest function and stores in in the result
																									//(will change from login manager to Menu manager if logged in)
			temp = vectorToString(requestResultStruct.buffer); //convert the buffer to string
			send(sc, temp.c_str(), strlen(temp.c_str()), 0);
		}
	}
	catch (const std::exception& e) { cout << e.what() << endl; }
}

/*
IGNORE UNVALID CHARS
This method receives a msg from the client and return a string without unvalid chars
input:		socket, msg length
output:		msg without unvalid chars
*/
vector<unsigned char> Communicator::ignoreUnvalidChars(const SOCKET sc, const int a) const
{
	char* length = new char[6];
	vector<unsigned char> buffer;
	int temp = 1000;
	int len = 0;

	recv(sc, length, 6, 0);
	
	buffer.push_back(length[0]);
	buffer.push_back(length[1]);

	for (int i = 2; i < 7; i++)
	{
		if (length[i] >= ASCII_VALUE_OF_THE_FIRST_VALID_CHAR && length[i] <= ASCII_VALUE_OF_THE_LAST_VALID_CHAR)
		{
			buffer.push_back(length[i]);
			len += ((int)length[i] - 48) * temp;
			temp /= 10;
		}
	}

	char* data = new char[len];
	if (len != 0)//if there is a data
	{
		recv(sc, data, len, 0);

		for (int i = 0; i < len; i++) buffer.push_back(data[i]);
	}
	return buffer;
}

/*
VECTOR TO STRING
This method gets a vetor and returns it as string
input:		v
output:		msg
*/
string Communicator::vectorToString(const vector<unsigned char> v)
{
	string msg = "";
	for (int i = 0; i < v.size(); i++) msg += v[i];
	return msg;
}


/*
GET DATA
This method gets the data from the buffer
input:		buffer
output:		temp
*/
vector<unsigned char> Communicator::getData(const vector<unsigned char> buffer) const
{
	vector<unsigned char> temp;
	for (int i = 6; i < buffer.size(); i++) {
		temp.push_back(buffer[i]);
	}
	return temp;
}

IDatabase* Communicator::getDB()
{
	return this->m_handlerFactory.getDB();
}
