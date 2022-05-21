#pragma once
#include "olcPGEX_Network.h"
#include "game_data.h"

namespace gr
{
	const int SERVER_PORT = 52418;
	const float GAME_UPDATE_DELAY = 0.01f;
	const float GAME_UPDATE_WORLD_DELAY = 0.005f;
	const float GAME_MOVING_TO_DELAY = 0.01f;
}

enum class GameMsg : uint32_t
{
	Server_GetStatus,
	Server_GetPing,

	Client_Accepted,
	Client_AssignID,
	Client_RegisterWithServer,
	Client_UnregisterWithServer,

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer,
	Game_PlayerMovingTo,
	Game_World,

    Chat_NewMessage,
};

inline void pushPlayerToMessage(olc::net::message<GameMsg> &msg, const gr::data::Player &player)
{
	for(size_t i=0; i<player.cells.size(); i++)
		msg << player.cells[i];
	msg << player.cells.size() << player.last_moving_to << player.id;
}

gr::data::Player popPlayerFromMessage(olc::net::message<GameMsg> &msg)
{
	gr::data::Player p;
	size_t sz;
	gr::data::Cell c;
	msg >> p.id >> p.last_moving_to >> sz;
	for(size_t i=0; i<sz; i++)
	{
		msg >> c;
		p.cells.push_back(c);
	}
	reverse(p.cells.begin(), p.cells.end());
	return p;
		
}

inline void pushGameToMessage(olc::net::message<GameMsg> &msg, const gr::data::Game &game)
{
	for(size_t i=0; i<game.players.size(); i++)
		pushPlayerToMessage(msg, game.players[i]);
	msg << game.players.size();
}

gr::data::Game popGameFromMessage(olc::net::message<GameMsg> &msg)
{
	gr::data::Game g;
	size_t sz;
	gr::data::Player p;
	msg >> sz;
	for(size_t i=0; i<sz; i++)
		g.players.push_back(popPlayerFromMessage(msg));
	reverse(g.players.begin(), g.players.end());
	return g;
}




/*
void pushMessageString(olc::net::message<GameMsg> &msg, sf::String str){
	for(int i=0; i<str.getSize(); i++){
		msg << str[i];
	}
	msg << str.getSize();
}

sf::String getMessageString(olc::net::message<GameMsg> &msg){
	sf::String str;
	size_t sz;
	msg >> sz;
	sf::Uint32 ch;
	for(int i=0; i<sz; i++){
		msg >> ch;
		str += ch;
	}
	reverse(str.begin(), str.end());
	return str;
}
*/


