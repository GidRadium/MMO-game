#include "gui_mmo.h"
//#include <iostream>

namespace gr 
{
    namespace gui 
    {

        void Chat::onResize() 
        {

        }
        
        void Chat::draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            setObjectView(target);

            sf::RectangleShape rs;
            rs.setFillColor(sf::Color::Red);
            rs.setSize(sf::Vector2f(local_bounds_.width, local_bounds_.height));
            rs.setPosition(sf::Vector2f(0, 0));
            target.draw(rs);
            sf::CircleShape circle;
            circle.setFillColor(sf::Color::Green);
            circle.setRadius(std::min(local_bounds_.width, local_bounds_.height) / 2);
            circle.setPosition(sf::Vector2f(0, 0));
            target.draw(circle);
        }

        
        void MainScene::init(sf::RenderTarget& target)
        {
            linkObject(&chat_);
            setSize(sf::Vector2i(target.getSize().x, target.getSize().y));
        }

        void MainScene::handleEvent(sf::Event event, sf::RenderTarget& target)
        {
            if(event.type == sf::Event::Resized)
            {
                setSize(sf::Vector2i(target.getSize().x, target.getSize().y));
                
            }
        }

        void MainScene::onResize()
        {
            chat_.setSize(sf::Vector2i(100, 200));
            chat_.setLocalPosition(sf::Vector2i(0, getSize().y - chat_.getSize().y));
            updateGlobalBounds(sf::Vector2i(0, 0));
        }

        void MainScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            target.clear(sf::Color::Black);
            //target.draw(chat_);
            chat_.draw(target, states);
            
        }
        


    }
}