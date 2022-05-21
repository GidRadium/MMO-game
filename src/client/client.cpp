#include <SFML/Graphics.hpp>
#include <iostream>
#include "gui_mmo.h"




/*
class Client 
{
    sf::RenderWindow window;
    sf::Clock clock;
    gr::gui::MainScene main_scene;
    gr::data::Game game_data;
    gr::data::Player player;

public:

    void start(){
        // initing window
        sf::ContextSettings settings;
	    settings.antialiasingLevel = 8;
        window.create(sf::VideoMode(600, 600), "MMO by GidRadium", sf::Style::Default, settings);
        window.setVerticalSyncEnabled(true);
        // initing scenes
        main_scene.init(window);

        gr::data::Cell c;
        c.x = 0;
        c.y = 0;
        c.mass = 1000000;
        player.cells.push_back(c);
        player.id = 101;
        player.name = "GidRadium";
        game_data.players.push_back(player);
        main_scene.game_world_.setPlayerID(101);
        main_scene.game_world_.updateData(game_data);
        //main_scene.game_world_.updateCamera();

        clock.restart();
        run();
    }

    void run(){
        
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                else main_scene.handleEvent(event, window);
            }

            game_data.players[0].last_moving_to = main_scene.game_world_.getUnderMouseCoords(window);
            game_data.update(clock.restart().asSeconds());
            main_scene.game_world_.updateData(game_data);

            window.clear();
            window.draw(main_scene);
            window.display();
        }
    }

private:
    

};




*/



#include "../common/net_common.h"


class CircleGameClient : olc::net::client_interface<GameMsg> 
{
public:

    //gr::gui::MainScene main_scene;
    //gr::gui::Scene* active_scene = &main_scene;
    //sf::RenderWindow window;

    sf::RenderWindow window;
    sf::Clock clock;
    gr::gui::MainScene main_scene;
    gr::data::Game game_data;
    

    void start()
    {
        //gr::gui::loadFont();
        //gr::gui::onWindowCreate(window, sf::Vector2i(500, 500), "Circle game | (by GidRadium)");
        //main_scene.onResize();

        try{
            if(Connect("localhost", gr::SERVER_PORT)){
                //LOG << "Socket created!"; LOG.flush();
                std::cout << "Socket created!\n";
            }
            else{
               //LOG << "Failed to connect!"; LOG.flush();
               std::cout << "Failed to create socket!\n";
            }
        }
        catch(asio::error_code ec){
            //LOG << L"asio error: " << gr::to_wstring(ec.message()); LOG.flush();
        }

        // initing window
        sf::ContextSettings settings;
	    settings.antialiasingLevel = 8;
        window.create(sf::VideoMode(600, 600), "MMO by GidRadium", sf::Style::Default, settings);
        window.setVerticalSyncEnabled(true);
        // initing scenes
        main_scene.init(window);

        gr::data::Player player;
        gr::data::Cell c;
        c.x = 0;
        c.y = 0;
        c.mass = 1000000;
        player.cells.push_back(c);
        player.id = 101;
        player.name = "GidRadium";
        game_data.players.push_back(player);
        main_scene.game_world_.setPlayerID(101);
        main_scene.game_world_.updateData(game_data);
        

        run();
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
    

    void run()
    {
        sf::Clock system_clock;
        sf::Clock update_system_clock;
        sf::Clock moving_to_clock;
        long double update_system_time = 0;
        long double ping_timer = 0;
        long double moving_to_timer = 0;
        bool connected = 0;
        bool accepted = 0;
        while (window.isOpen())
        {
            update_system_time = update_system_clock.restart().asSeconds();
            ping_timer += update_system_time;
            moving_to_timer += moving_to_clock.restart().asSeconds();

            if (IsConnected())
            {
                if(!connected){
                    connected = true;
                    //LOG << "Connection restored!"; LOG.flush();
                }
                if(accepted){
                    if (ping_timer >= 10.0)
                    {
                        ping_timer -= 10.0;
                        olc::net::message<GameMsg> msg;
                        msg.header.id = GameMsg::Server_GetPing;
                        msg << system_clock.getElapsedTime().asMilliseconds();
                        Send(msg);
                        //LOG << "send ping msg"; LOG.flush();
                    }
                    
                    if (moving_to_timer >= gr::GAME_MOVING_TO_DELAY)
                    {
                        moving_to_timer -= gr::GAME_MOVING_TO_DELAY;
                        olc::net::message<GameMsg> msg;
                        msg.header.id = GameMsg::Game_PlayerMovingTo;
                        msg << main_scene.game_world_.getUnderMouseCoords(window);
                        Send(msg);
                    }
                    
                }
                
                while (!Incoming().empty())
                {
                    auto msg = Incoming().pop_front().msg;

                    switch (msg.header.id)
                    {
                    case(GameMsg::Server_GetPing):
                    {
                        sf::Int32 ping_from_server_time;
                        msg >> ping_from_server_time;
                        //LOG << "Ping: " << system_clock.getElapsedTime().asMilliseconds() - ping_from_server_time << "ms"; LOG.flush();
                        std::cout << "Ping: " << system_clock.getElapsedTime().asMilliseconds() - ping_from_server_time << "ms\n";
                        break;
                    }
                    case(GameMsg::Client_Accepted):
                    {
                        //LOG << "Server accepted client - you're in!"; LOG.flush();
                        std::cout << "Server accepted client - you're in!\n";
                        olc::net::message<GameMsg> msg;
                        msg.header.id = GameMsg::Client_RegisterWithServer;
                        //descPlayer.vPos = { 3.0f, 3.0f };
                        //msg << descPlayer;
                        Send(msg);
                        accepted = 1;
                        break;
                    }
                    case(GameMsg::Client_AssignID):
                    {
                        uint32_t id;
                        msg >> id;
                        std::cout << id << "\n";
                        main_scene.game_world_.setPlayerID(id);
                        break;
                    }
                    case(GameMsg::Chat_NewMessage):
                    {
                        //main_scene._chat._messages.push_back(getMessageString(msg));
                        //NEED_TO_REDRAW = 1;
                        break;
                    }
                    
                    case(GameMsg::Game_World):
                    {
                        game_data = popGameFromMessage(msg);
                        //std::cout << game_data.players.size() << "\n";
                        break;
                    }
                    
                    default:
                        //LOG << "Unknown message!"; LOG.flush();
                        std::cout << "Unknown message!\n";

                    }
                }
            }
            else{
                if(connected){
                    connected = 0;
                    //LOG << "Connection lost! Trying to reconnect..."; LOG.flush();
                    std::cout << "Connection lost! Trying to reconnect...\n";
                    try{ Connect("localhost", gr::SERVER_PORT); }
                    catch(asio::error_code ec){ std::cout << "asio error: " << ec.message() << "\n"; }
                }
            }

            /*
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                else if (event.type == sf::Event::Resized) {
                    gr::gui::onWindowResize(window.getSize());
                    main_scene.onResize();
                    NEED_TO_REDRAW = 1;
                }
                else if (event.type == sf::Event::MouseButtonReleased) {
                    //LOG << argc; LOG.flush();
                    //LOG << argv[0];
                }
                else if (active_scene != nullptr) {
                    active_scene->onEvent(event, window);
                }
            }

            if(main_scene._outcomming_messages.size()){
                auto msg_text = main_scene._outcomming_messages[0];
                main_scene._outcomming_messages.erase(main_scene._outcomming_messages.begin());
                olc::net::message<GameMsg> msg;
                msg.header.id = GameMsg::Chat_NewMessage;
                pushMessageString(msg, msg_text);
                //msg << msg_text[0];
                Send(msg);
            }

            
            
            
            if (active_scene != nullptr) {
                active_scene->onUpdate();
                if (active_scene->_need_to_switch_to_parent && active_scene->_parent != nullptr)
                    active_scene = &(*(active_scene->_parent));
                else if (active_scene->_switch_to != nullptr) {
                    active_scene->_switch_to->_parent = &(*active_scene);
                    active_scene = active_scene->_switch_to;
                }
            }


            if (active_scene != nullptr && NEED_TO_REDRAW) {
                window.clear();
                window.draw(*active_scene);
                window.display();
                NEED_TO_REDRAW = 0;
            }
            */

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                else main_scene.handleEvent(event, window);
            }

            //game_data.players[0].last_moving_to = main_scene.game_world_.getUnderMouseCoords(window);
            game_data.update(clock.restart().asSeconds());
            main_scene.game_world_.updateData(game_data);

            window.clear();
            window.draw(main_scene);
            window.display();

        }
    }


};







int main()
{
    //Client client;
    CircleGameClient client;

    client.start();

    return 0;
}