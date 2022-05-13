#include <SFML/Graphics.hpp>
#include <iostream>
#include "gui_mmo.h"





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
        
        //window.setFramerateLimit(1);

        // initing scenes
        main_scene.init(window);

        gr::data::Cell c;
        c.x = 0;
        c.y = 0;
        c.mass = 100000;
        player.cells.push_back(c);
        player.id = 101;
        player.name = "GidRadium";
        game_data.players.push_back(player);
        main_scene.game_world_.setPlayerID(101);
        main_scene.game_world_.updateData(game_data);
        //window.mapPixelToCoords(s)
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




int main()
{
    Client client;
    client.start();

    return 0;
}