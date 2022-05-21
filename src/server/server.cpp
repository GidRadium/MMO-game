
#include "../common/net_common.h"

#include <iostream>
#include <unordered_map>
#include <SFML/System/Clock.hpp>

class GameServer : public olc::net::server_interface<GameMsg>
{
public:
	GameServer(uint16_t nPort) : olc::net::server_interface<GameMsg>(nPort)
	{
	}

	float game_send_timer = 0;
	float game_update_timer = 0;
	sf::Clock game_upd_clock;
	sf::Clock game_send_clock;
	gr::data::Game game_data;


	void updateWorld()
	{
		game_update_timer += game_upd_clock.restart().asSeconds();
		if(game_update_timer >= gr::GAME_UPDATE_WORLD_DELAY)
		{
			game_data.update(game_update_timer);
			game_update_timer -= gr::GAME_UPDATE_WORLD_DELAY;
		}
		
	}
	void sendGameWorld()
	{
		game_send_timer += game_send_clock.restart().asSeconds();
		//std::cout << game_send_clock.restart().asSeconds() << "\n";
		//std::cout << game_send_timer << "\n";
		if(game_send_timer >= gr::GAME_UPDATE_DELAY)
		{
			game_send_timer -= gr::GAME_UPDATE_DELAY;
			olc::net::message<GameMsg> msg;
			msg.header.id = GameMsg::Game_World;
			pushGameToMessage(msg, game_data);
			MessageAllClients(msg);
			//std::cout << "$";
		}
	}
	

	//std::unordered_map<uint32_t, PlayerDescription> m_mapPlayerRoster;
	//std::vector<uint32_t> m_vGarbageIDs;

	

protected:
	bool OnClientConnect(std::shared_ptr<olc::net::connection<GameMsg>> client) override
	{
		// For now we will allow all 
		return true;
	}

	void OnClientValidated(std::shared_ptr<olc::net::connection<GameMsg>> client) override
	{
		// Client passed validation check, so send them a message informing
		// them they can continue to communicate
		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Client_Accepted;
		client->Send(msg);
	}

	void OnClientDisconnect(std::shared_ptr<olc::net::connection<GameMsg>> client) override
	{
		/*
		if (client)
		{
			if (m_mapPlayerRoster.find(client->GetID()) == m_mapPlayerRoster.end())
			{
				// client never added to roster, so just let it disappear
			}
			else
			{
				auto& pd = m_mapPlayerRoster[client->GetID()];
				std::cout << "[UNGRACEFUL REMOVAL]:" + std::to_string(pd._uniqueID) + "\n";
				m_mapPlayerRoster.erase(client->GetID());
				m_vGarbageIDs.push_back(client->GetID());
			}
		}
		*/

	}

	void OnMessage(std::shared_ptr<olc::net::connection<GameMsg>> client, olc::net::message<GameMsg>& msg) override
	{
		/*
		if (!m_vGarbageIDs.empty())
		{
			for (auto pid : m_vGarbageIDs)
			{
				olc::net::message<GameMsg> m;
				m.header.id = GameMsg::Game_RemovePlayer;
				m << pid;
				std::cout << "Removing " << pid << "\n";
				MessageAllClients(m);
			}
			m_vGarbageIDs.clear();
		}
		*/


		switch (msg.header.id)
		{
		case GameMsg::Server_GetPing:
        {
            MessageClient(client, msg);
			break;
        }
		case GameMsg::Client_RegisterWithServer:
		{
			//PlayerDescription desc;
			//msg >> desc;
			//desc._uniqueID = client->GetID();
			//m_mapPlayerRoster.insert_or_assign(desc._uniqueID, desc);

			olc::net::message<GameMsg> msgSendID;
			msgSendID.header.id = GameMsg::Client_AssignID;
			msgSendID << client->GetID();
			MessageClient(client, msgSendID);


			gr::data::Player p;
			p.id = client->GetID();
			gr::data::Cell c;
			c.x = 0;
        	c.y = 0;
        	c.mass = 1000000;
			p.cells.push_back(c);
			game_data.players.push_back(p);

			//olc::net::message<GameMsg> msgAddPlayer;
			//msgAddPlayer.header.id = GameMsg::Game_AddPlayer;
			//msgAddPlayer << desc;
			//MessageAllClients(msgAddPlayer);

			//for (const auto& player : m_mapPlayerRoster)
			//{
			//	olc::net::message<GameMsg> msgAddOtherPlayers;
			//	msgAddOtherPlayers.header.id = GameMsg::Game_AddPlayer;
			//	msgAddOtherPlayers << player.second;
			//	MessageClient(client, msgAddOtherPlayers);
			//}

			break;
		}


		case GameMsg::Client_UnregisterWithServer:
		{
			break;
		}

		case GameMsg::Game_PlayerMovingTo:
		{
			for(size_t i=0; i<game_data.players.size(); i++)
				if(game_data.players[i].id == client->GetID())
					msg >> game_data.players[i].last_moving_to;
			break;
		}
        case GameMsg::Chat_NewMessage:
        {
            MessageAllClients(msg);
			break;
        }

		}

	}

};



int main()
{
	GameServer server(gr::SERVER_PORT);
	server.Start();

	while (1)
	{
		server.Update();
		server.updateWorld();
		server.sendGameWorld();
	}

	return 0;
}
