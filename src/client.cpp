#include <SFML/Graphics.hpp>
#include <iostream>
#include "gui_mmo.h"





class Client 
{
    sf::RenderWindow window;
    sf::CircleShape shape;
    gr::gui::MainScene main_scene;

public:
    void start(){
        
    }
    void run(){
        window.create(sf::VideoMode(600, 600), "SFML works! WOW!");
        //shape.setRadius(100.f);
        //shape.setFillColor(sf::Color::Green);
        window.setVerticalSyncEnabled(true);
        //window.setFramerateLimit(1);

        main_scene.init(window);

        //std::cout << main_scene.chat_.getSize().x << " " << main_scene.chat_.getSize().y << "\n";

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                else main_scene.handleEvent(event, window);
            }

            window.clear();
            //window.draw(main_scene);
            main_scene.draw(window, sf::RenderStates::Default);
            window.display();
        }
    }
private:
    

    
    
};









int main()
{
    Client client;
    client.run();

    return 0;
}