#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Server.h"
#include "SqliteDataBase.h"
#include <iostream>
#include <exception>

int main()
{
    WSAInitializer wsaInit;
    Server myServer = Server();
    myServer.run();
    return 0;
}