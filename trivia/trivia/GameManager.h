#pragma once
#include <iostream>
#include "SqliteDataBase.h"
#include <vector>
#include "Game.h"
#include "Room.h"

using std::vector;
using std::pair;

class GameManager
{
private:
	IDatabase* m_database;
	std::vector<Game> m_games;

public:
	GameManager(IDatabase* db, vector<Game> games);
	GameManager(IDatabase* db);
	Game createGame(Room room);
	void deleteGame(Game game);
};