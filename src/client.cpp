#include <SFML/Graphics.hpp>
#include "gui_mmo.h"





class Client 
{
    sf::RenderWindow window;
    sf::CircleShape shape;


public:
    void start(){
        
    }
    void run(){
        window.create(sf::VideoMode(200, 200), "SFML works! WOW!");
        shape.setRadius(100.f);
        shape.setFillColor(sf::Color::Green);
        window.setVerticalSyncEnabled(true);

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(shape);
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